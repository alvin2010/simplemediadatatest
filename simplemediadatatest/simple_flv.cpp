#include "stdafx.h"
#include "simple_flv.h"


//Important!  
#pragma pack(1)  


#define TAG_TYPE_SCRIPT 18  
#define TAG_TYPE_AUDIO  8  
#define TAG_TYPE_VIDEO  9  

typedef unsigned char byte;
typedef unsigned int uint;

typedef struct {
	byte Signature[3];
	byte Version;
	byte Flags;
	uint DataOffset;
} FLV_HEADER;


typedef struct {
	byte TagType;
	byte DataSize[3];
	byte Timestamp[3];
	uint Reserved;
} TAG_HEADER;


/*
*	turn a BigEndian byte array into a LittleEndian integer
*
*/
uint reverse_bytes(byte* p, char c)
{
	int r = 0;
	for (int i = 0; i < c; i++)
	{
		r |= (*(p + i) << (c - 1 - i) * 8);
	}
	return r;
}




void simple_parse_flv(const char* url)
{
	FILE *myout = stdout;
	FILE *ifh = NULL, *vfh = NULL, *afh = NULL;
	int output_a = 1;
	int output_v = 1;

	ifh = fopen(url, "rb");
	if (!ifh)
	{
		printf("open flv file failed!\n");
		return;
	}

	FLV_HEADER flv;
	fread(&flv, sizeof(FLV_HEADER), 1, ifh);

	fprintf(myout, "============== FLV Header ==============\n");
	fprintf(myout, "Signature:  0x %c %c %c\n", flv.Signature[0], flv.Signature[1], flv.Signature[2]);
	fprintf(myout, "Version:    0x %X\n", flv.Version);
	fprintf(myout, "Flags  :    0x %X\n", flv.Flags);
	fprintf(myout, "HeaderSize: 0x %X\n", reverse_bytes((byte *)&flv.DataOffset, sizeof(flv.DataOffset)));
	fprintf(myout, "========================================\n");

	//	move file pointor to the end of the FLV_HEADER
	fseek(ifh, reverse_bytes((byte *)&flv.DataOffset, sizeof(flv.DataOffset)), SEEK_SET);
	
	
	uint previoustagsize, previoustagsize_z = 0;
	//	video & audio
	TAG_HEADER tagheader;
	do {


		previoustagsize = _getw(ifh);
		fread(&tagheader, sizeof(TAG_HEADER), 1, ifh);
		int tagheader_datasize = tagheader.DataSize[0] * 65536 + tagheader.DataSize[1] * 256 + tagheader.DataSize[2];
		int tagheader_timestamp = tagheader.Timestamp[0] * 65536 + tagheader.Timestamp[1] * 256 + tagheader.Timestamp[2];
		
		
		char tagtype_str[10];
		switch (tagheader.TagType){
		case TAG_TYPE_AUDIO:sprintf(tagtype_str, "AUDIO"); break;
		case TAG_TYPE_VIDEO:sprintf(tagtype_str, "VIDEO"); break;
		case TAG_TYPE_SCRIPT:sprintf(tagtype_str, "SCRIPT"); break;
		default:sprintf(tagtype_str, "UNKNOWN"); break;
		}
		if (TAG_TYPE_SCRIPT == tagheader.TagType)
		{
			fprintf(myout, "[%6s] %6d %6d |\n", tagtype_str, tagheader_datasize, tagheader_timestamp);
		}
		else
		{
			fprintf(myout, "[%6s] %6d %6d |", tagtype_str, tagheader_datasize, tagheader_timestamp);
		}
		

		if (feof(ifh)) break;

		int x = 0, data_size = 0;
		char audiotag_str[100] = { 0 };
		char videotag_str[100] = { 0 };
		char tagdata_first_byte;
		switch (tagheader.TagType)
		{
		case TAG_TYPE_AUDIO:
			memset(audiotag_str, 0, 100);
			strcat(audiotag_str, "| ");
			tagdata_first_byte = fgetc(ifh);
			x = tagdata_first_byte & 0xF0;
			x = x >> 4;
			switch (x)
			{
			case 0:strcat(audiotag_str, "Linear PCM, platform endian"); break;
			case 1:strcat(audiotag_str, "ADPCM"); break;
			case 2:strcat(audiotag_str, "MP3"); break;
			case 3:strcat(audiotag_str, "Linear PCM, little endian"); break;
			case 4:strcat(audiotag_str, "Nellymoser 16-kHz mono"); break;
			case 5:strcat(audiotag_str, "Nellymoser 8-kHz mono"); break;
			case 6:strcat(audiotag_str, "Nellymoser"); break;
			case 7:strcat(audiotag_str, "G.711 A-law logarithmic PCM"); break;
			case 8:strcat(audiotag_str, "G.711 mu-law logarithmic PCM"); break;
			case 9:strcat(audiotag_str, "reserved"); break;
			case 10:strcat(audiotag_str, "AAC"); break;
			case 11:strcat(audiotag_str, "Speex"); break;
			case 14:strcat(audiotag_str, "MP3 8-Khz"); break;
			case 15:strcat(audiotag_str, "Device-specific sound"); break;
			default:strcat(audiotag_str, "UNKNOWN"); break;
			}
			strcat(audiotag_str, "| ");
			x = tagdata_first_byte & 0x0C;
			x = x >> 2;
			switch (x)
			{
			case 0:strcat(audiotag_str, "5.5-kHz"); break;
			case 1:strcat(audiotag_str, "1-kHz"); break;
			case 2:strcat(audiotag_str, "22-kHz"); break;
			case 3:strcat(audiotag_str, "44-kHz"); break;
			default:strcat(audiotag_str, "UNKNOWN"); break;
			}
			strcat(audiotag_str, "| ");
			x = tagdata_first_byte & 0x02;
			x = x >> 1;
			switch (x)
			{
			case 0:strcat(audiotag_str, "8Bit"); break;
			case 1:strcat(audiotag_str, "16Bit"); break;
			default:strcat(audiotag_str, "UNKNOWN"); break;
			}
			strcat(audiotag_str, "| ");
			x = tagdata_first_byte & 0x01;
			switch (x)
			{
			case 0:strcat(audiotag_str, "Mono"); break;
			case 1:strcat(audiotag_str, "Stereo"); break;
			default:strcat(audiotag_str, "UNKNOWN"); break;
			}
			fprintf(myout, "%s\n", audiotag_str);

			//if the output file hasn't been opened, open it.  
			if (output_a != 0 && afh == NULL){
				afh = fopen("output.mp3", "wb");
			}

			//TagData - First Byte Data  
			data_size = reverse_bytes((byte *)&tagheader.DataSize, sizeof(tagheader.DataSize)) - 1;
			if (output_a != 0){
				//TagData+1  
				for (int i = 0; i<data_size; i++)
					fputc(fgetc(ifh), afh);

			}
			else{
				for (int i = 0; i<data_size; i++)
					fgetc(ifh);
			}
			break;
		case TAG_TYPE_VIDEO:
			memset(videotag_str, 0, 100);
			strcat(videotag_str, "| ");
			tagdata_first_byte = fgetc(ifh);
			x = tagdata_first_byte & 0xF0;
			x = x >> 4;
			switch (x)
			{
			case 1:strcat(videotag_str, "key frame  "); break;
			case 2:strcat(videotag_str, "inter frame"); break;
			case 3:strcat(videotag_str, "disposable inter frame"); break;
			case 4:strcat(videotag_str, "generated keyframe"); break;
			case 5:strcat(videotag_str, "video info/command frame"); break;
			default:strcat(videotag_str, "UNKNOWN"); break;
			}
			strcat(videotag_str, "| ");
			x = tagdata_first_byte & 0x0F;
			switch (x)
			{
			case 1:strcat(videotag_str, "JPEG (currently unused)"); break;
			case 2:strcat(videotag_str, "Sorenson H.263"); break;
			case 3:strcat(videotag_str, "Screen video"); break;
			case 4:strcat(videotag_str, "On2 VP6"); break;
			case 5:strcat(videotag_str, "On2 VP6 with alpha channel"); break;
			case 6:strcat(videotag_str, "Screen video version 2"); break;
			case 7:strcat(videotag_str, "AVC"); break;
			default:strcat(videotag_str, "UNKNOWN"); break;
			}
			fprintf(myout, "%s\n", videotag_str);

			fseek(ifh, -1, SEEK_CUR);
			//if the output file hasn't been opened, open it.  
			if (vfh == NULL&&output_v != 0) {
				//write the flv header (reuse the original file's hdr) and first previoustagsize  
				vfh = fopen("output.flv", "wb");
				fwrite((char *)&flv, 1, sizeof(flv), vfh);
				fwrite((char *)&previoustagsize_z, 1, sizeof(previoustagsize_z), vfh);
			}
#if 0  
			//Change Timestamp  
			//Get Timestamp  
			ts = reverse_bytes((byte *)&tagheader.Timestamp, sizeof(tagheader.Timestamp));
			ts = ts * 2;
			//Writeback Timestamp  
			ts_new = reverse_bytes((byte *)&ts, sizeof(ts));
			memcpy(&tagheader.Timestamp, ((char *)&ts_new) + 1, sizeof(tagheader.Timestamp));
#endif  


			//TagData + Previous Tag Size  
			data_size = reverse_bytes((byte *)&tagheader.DataSize, sizeof(tagheader.DataSize)) + 4;
			if (output_v != 0){
				//TagHeader  
				fwrite((char *)&tagheader, 1, sizeof(tagheader), vfh);
				//TagData  
				for (int i = 0; i<data_size; i++)
					fputc(fgetc(ifh), vfh);
			}
			else{
				for (int i = 0; i<data_size; i++)
					fgetc(ifh);
			}
			//rewind 4 bytes, because we need to read the previoustagsize again for the loop's sake  
			fseek(ifh, -4, SEEK_CUR);
			break;

		default:
			fseek(ifh, reverse_bytes((byte *)&tagheader.DataSize, sizeof(tagheader.DataSize)), SEEK_CUR);
			break;
		}


	} while (!feof(ifh));
	_fcloseall();
}