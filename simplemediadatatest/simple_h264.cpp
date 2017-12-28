#include "stdafx.h"
#include "simple_h264.h"


typedef struct
{
	int startcodeprefix_len;      //! 4 for parameter sets and first slice in picture, 3 for everything else (suggested)  
	unsigned len;                 //! Length of the NAL unit (Excluding the start code, which does not belong to the NALU)  
	unsigned max_size;            //! Nal Unit Buffer size  
	int nal_unit_type;            //! NALU_TYPE_xxxx  
	int nal_reference_idc;        //! NALU_PRIORITY_xxxx  
	int forbidden_bit;            //! should be always FALSE  
	char *buf;					  //! conjtains the first byte followed by the RBSP  
} NALU_t;


typedef enum {
	NALU_TYPE_SLICE = 1,
	NALU_TYPE_DPA = 2,
	NALU_TYPE_DPB = 3,
	NALU_TYPE_DPC = 4,
	NALU_TYPE_IDR = 5,
	NALU_TYPE_SEI = 6,
	NALU_TYPE_SPS = 7,
	NALU_TYPE_PPS = 8,
	NALU_TYPE_AUD = 9,
	NALU_TYPE_EOSEQ = 10,
	NALU_TYPE_EOSTREAM = 11,
	NALU_TYPE_FILL = 12,
} NaluType;

typedef enum {
	NALU_PRIORITY_DISPOSABLE = 0,
	NALU_PRIRITY_LOW = 1,
	NALU_PRIORITY_HIGH = 2,
	NALU_PRIORITY_HIGHEST = 3
} NaluPriority;


bool FindStartCode3(unsigned char* buf)
{
	if (buf[0] == 0 && buf[1] == 0 && buf[2] == 1) return true;
	else return false;
}

bool FindStartCode4(unsigned char* buf)
{
	if (buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 1) return true;
	else return false;
}

FILE* h264bitstream = NULL;
int GetNextNalu(NALU_t* n)
{
	unsigned char* buf = (unsigned char*)calloc(n->max_size, sizeof(char));
	if (buf == NULL) printf("GetAnnexbNALU: Could not allocate Buf memory\n");
	
	int pos = 0;
	n->startcodeprefix_len = 3;
	if (3 != fread(buf, 1, 3, h264bitstream))
	{
		free(buf);
		return -1;
	}
	// 前导字节
	bool b3, b4;
	b3 = FindStartCode3(buf);
	if (!b3)
	{
		if (1 != fread(buf+3, 1, 1, h264bitstream))
		{
			free(buf);
			return -1;
		}
		b4 = FindStartCode4(buf);
		if (!b4)
		{
			free(buf);
			return -1;
		}
		else
		{
			n->startcodeprefix_len = 4;
			pos = 4;
		}
	}
	else
	{
		n->startcodeprefix_len = 3;
		pos = 3;
	}

	
	//	查找下一个startcode
	bool bFound = false;
	while (!bFound)
	{
		if (feof(h264bitstream)){
			n->len = (pos - 1) - n->startcodeprefix_len;
			memcpy(n->buf, &buf[n->startcodeprefix_len], n->len);
			n->forbidden_bit = n->buf[0] & 0x80; //1 bit  
			n->nal_reference_idc = n->buf[0] & 0x60; // 2 bit  
			n->nal_unit_type = (n->buf[0]) & 0x1f;// 5 bit  
			free(buf);
			return pos - 1;
		}

		buf[pos++] = fgetc(h264bitstream);
		b4 = FindStartCode4(&buf[pos - 4]);
		if (b4 != 1)
			b3 = FindStartCode3(&buf[pos - 3]);
		bFound = (b3 == 1 || b4 == 1);
	}

	int rewind = (b4 == 1) ? -4 : -3;
	if (0 != fseek(h264bitstream, rewind, SEEK_CUR)){
		free(buf);
		printf("GetAnnexbNALU: Cannot fseek in the bit stream file");
	}
	n->len = (pos + rewind) - n->startcodeprefix_len;
	memcpy(n->buf, &buf[n->startcodeprefix_len], n->len);//  
	n->forbidden_bit = n->buf[0] & 0x80; //1 bit  
	n->nal_reference_idc = n->buf[0] & 0x60; // 2 bit  
	n->nal_unit_type = (n->buf[0]) & 0x1f;// 5 bit  
	free(buf);

	return (pos + rewind);
}


/**
*	Analysis H.264 Bitstream 
*
*
*
**********/
void simple_parse_h264(const char* url)
{
	h264bitstream = fopen(url, "rb");
	if (h264bitstream == NULL){
		printf("Open file error\n");
		return;
	}

	int iNalu = 100000;
	NALU_t* nalu = (NALU_t*)malloc(sizeof(NALU_t));
	nalu->max_size = iNalu;
	nalu->buf = (char*)malloc(iNalu);
	if (nalu->buf == NULL)
	{
		free(nalu);
		fclose(h264bitstream);
		return;
	}

	int num = 0, offset = 0;
	printf("-----+-------- NALU Table ------+---------+\n");
	printf(" NUM |    POS  |    IDC |  TYPE |   LEN   |\n");
	printf("-----+---------+--------+-------+---------+\n");
	while (!feof(h264bitstream))
	{
		int n = GetNextNalu(nalu);
		char type_str[20] = { 0 };
		switch (nalu->nal_unit_type){
		case NALU_TYPE_SLICE:sprintf(type_str, "SLICE"); break;
		case NALU_TYPE_DPA:sprintf(type_str, "DPA"); break;
		case NALU_TYPE_DPB:sprintf(type_str, "DPB"); break;
		case NALU_TYPE_DPC:sprintf(type_str, "DPC"); break;
		case NALU_TYPE_IDR:sprintf(type_str, "IDR"); break;
		case NALU_TYPE_SEI:sprintf(type_str, "SEI"); break;
		case NALU_TYPE_SPS:sprintf(type_str, "SPS"); break;
		case NALU_TYPE_PPS:sprintf(type_str, "PPS"); break;
		case NALU_TYPE_AUD:sprintf(type_str, "AUD"); break;
		case NALU_TYPE_EOSEQ:sprintf(type_str, "EOSEQ"); break;
		case NALU_TYPE_EOSTREAM:sprintf(type_str, "EOSTREAM"); break;
		case NALU_TYPE_FILL:sprintf(type_str, "FILL"); break;
		}
		char idc_str[20] = { 0 };
		switch (nalu->nal_reference_idc >> 5){
		case NALU_PRIORITY_DISPOSABLE:sprintf(idc_str, "DISPOS"); break;
		case NALU_PRIRITY_LOW:sprintf(idc_str, "LOW"); break;
		case NALU_PRIORITY_HIGH:sprintf(idc_str, "HIGH"); break;
		case NALU_PRIORITY_HIGHEST:sprintf(idc_str, "HIGHEST"); break;
		}

		fprintf(stdout, "%5d| %8d| %7s| %6s| %8d|\n", num, offset, idc_str, type_str, nalu->len);
		offset = offset + n;
		num++;
	}



	//Free  
	if (nalu){
		if (nalu->buf){
			free(nalu->buf);
			nalu->buf = NULL;
		}
		free(nalu);
	}
	fclose(h264bitstream);
}