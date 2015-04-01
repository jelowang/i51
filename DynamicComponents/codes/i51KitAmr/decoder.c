#ifndef DECODER_H
#define DECODER_H
/*
 * ===================================================================
 *  TS 26.104
 *  REL-5 V5.4.0 2004-03
 *  REL-6 V6.1.0 2004-03
 *  3GPP AMR Floating-point Speech Codec
 * ===================================================================
 *
 */
#include "i51.h"
#include "i51KitAde.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <memory.h>
#include "interf_dec.h"
#include "sp_dec.h"
#include "typedef.h"

#ifndef ETSI
#ifndef IF2
//#include <string.h>
#define AMR_MAGIC_NUMBER "#!AMR\n"
#endif
#endif

 char WavHead[44] = {	 
	0x52, 0x49, 0x46, 0x46, 0xa2, 0x9e, 0x04, 0x00, 0x57, 0x41,
	0x56, 0x45, 0x66, 0x6d, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x01, 0x00, 0x40, 0x1f, 0x00, 0x00, 0x00, 0x7d,
	0x00, 0x00, 0x02, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61,  
	0x7e, 0x9e, 0x04, 0x00 };

void Copyright(void){
i51AdeOsLog (0,
"===================================================================\n"
" TS 26.104                                                         \n"
" REL-5 V5.4.0 2012-03                                              \n"
" REL-6 V6.1.0 2012-03                                              \n"
" 3GPP AMR Floating-point Speech Decoder                            \n"
"===================================================================\n"
);
}
/*
 * main
 *
 *
 * Function:
 *    Speech decoder main program
 *
 *    Usage: decoder bitstream_file synthesis_file
 *
 *    Format for ETSI bitstream file:
 *       1 word (2-byte) for the TX frame type
 *       244 words (2-byte) containing 244 bits.
 *          Bit 0 = 0x0000 and Bit 1 = 0x0001
 *       1 word (2-byte) for the mode indication
 *       4 words for future use, currently written as zero
 *
 *    Format for 3GPP bitstream file:
 *       Holds mode information and bits packed to octets.
 *       Size is from 1 byte to 31 bytes.
 *
 *    Format for synthesis_file:
 *       Speech is written to a 16 bit 8kHz file.
 *
 *    ETSI bitstream file format is defined using ETSI as preprocessor
 *    definition
 * Returns:
 *    0
 */
/************************************************************************
*  考虑到pcm流存放的空间不好确定,所以内存空间需要自己申请自己释放
*  建议 amr buff 小于等于 1kb* amr 转 pcm 最小约10倍，最大约64 倍. 
*  realsize 为转换后pcm的实际长度                 
*                                                       2012.3.3  Delor
/************************************************************************/
iS32 i51KitAmrDecoder ( iU8* rbuff , iU32 rlength , iU8* wbuff , iU32  wlength )
{
	iU16 synth[160];
	int frames = 0;
	int * destate;
	int read_size;
	int  wRealHead = 0;
	iU32 loop = 0;
	unsigned int Rsize = 0;
#ifndef ETSI
	unsigned char analysis[32];
	enum Mode dec_mode;
#ifdef IF2
	short block_size[16]={ 12, 13, 15, 17, 18, 20, 25, 30, 5, 0, 0, 0, 0, 0, 0, 0 };
#else
	char magic[8];
	short block_size[16]={ 12, 13, 15, 17, 19, 20, 26, 31, 5, 0, 0, 0, 0, 0, 0, 0 };
#endif
#else
	short analysis[250];
#endif
	if (rbuff == iNULL || wbuff == iNULL ||rlength < 1)
	{
		i51AdeOsLog(0, " 资源不完整,不能正常转换---- %d------%d",rlength, wlength );
		return iNULL;
	}
	// memset(wbuff, 0xff, 10*1024);

	destate = Decoder_Interface_init();

#ifndef ETSI
	while ((loop < rlength) && (wRealHead + frames*sizeof(iU16)*160 < wlength))
	{
		analysis[0] = rbuff[loop];
#ifdef IF2
		dec_mode = analysis[0] & 0x000F;
#else
		dec_mode = (analysis[0] >> 3) & 0x000F;
#endif
		read_size = block_size[dec_mode];

		i51AdeStdMemcpy16(&analysis[1], &rbuff[loop+1], sizeof(char)*read_size);
		loop += (read_size +1);
#else
	read_size = 250;
	while (loop < rlength)
	{

		i51AdeStdMemcpy16(&analysis, &rbuff[loop], sizeof(char)*read_size);
		loop += 250;
#endif

		Decoder_Interface_Decode(destate, analysis, (short *)synth, 0);

		i51AdeStdMemcpy16(&wbuff[wRealHead], synth, sizeof(iU16)*160);
		frames ++;
		wRealHead = frames *sizeof(iU16)*160;
	}
	Decoder_Interface_exit(destate);

	i51AdeOsLog(0,"解码结束 -----    - - - -- - - -- - ");
	return wRealHead;
}

iS32 i51KitAmrDecoderEx (const iU16 * rfilename,const iU16 * wfilename) {

	iFILE  file_speech, file_analysis;

	short synth[160];
	int frames = 0;
	int * destate;
	int read_size;
	unsigned int Rsize = 0;
#ifndef ETSI
	unsigned char analysis[32];
	enum Mode dec_mode;
#ifdef IF2
	short block_size[16]={ 12, 13, 15, 17, 18, 20, 25, 30, 5, 0, 0, 0, 0, 0, 0, 0 };
#else
	char magic[8];
	short block_size[16]={ 12, 13, 15, 17, 19, 20, 26, 31, 5, 0, 0, 0, 0, 0, 0, 0 };
	
#endif
#else
	short analysis[250];
#endif
	/* Process command line options */


	file_speech = i51AdeOsFileOpen (wfilename, i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE, __FILE__ , __LINE__);
	if (file_speech <= 0){
		i51AdeOsLog(0, "%s%s%s\n","Use:  input.file output.file " );
		return iNULL;
	}

	file_analysis = i51AdeOsFileOpen (rfilename, i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ, __FILE__ , __LINE__);;
	if (file_analysis <= 0 ){
		i51AdeOsLog(0, "%s%s%s\n","Use: input.file output.file " );
		i51AdeOsFileClose(file_speech);
		return iNULL;
	}
	Copyright();
	/* init decoder */
	destate = Decoder_Interface_init();
#ifndef ETSI
#ifndef IF2
	/* read and verify magic number */
	i51AdeOsFileRead( file_analysis, magic, sizeof( char )*strlen( AMR_MAGIC_NUMBER ),&Rsize  );
	if ( strncmp( magic, AMR_MAGIC_NUMBER, strlen( AMR_MAGIC_NUMBER ) ) ) {
		i51AdeOsLog(0, "%s%s\n", "Invalid magic number: ", magic );
		i51AdeOsFileClose( file_speech );
		i51AdeOsFileClose( file_analysis );
		return iNULL;
	}
	//i51AdeOsFileWrite(file_speech, AMR_MAGIC_NUMBER,  sizeof( char )*strlen( AMR_MAGIC_NUMBER ), &Rsize);
	i51AdeOsFileWrite(file_speech, WavHead, sizeof (char)*44, &Rsize);
#endif
#endif

#ifndef ETSI

	/* find mode, read file */
	while (i51AdeOsFileRead( file_analysis, analysis, sizeof (unsigned char), &Rsize)&& Rsize>0)
	{
#ifdef IF2
		dec_mode = analysis[0] & 0x000F;
#else
		dec_mode = (analysis[0] >> 3) & 0x000F;
#endif
		read_size = block_size[dec_mode];

		i51AdeOsFileRead(file_analysis,&analysis[1], sizeof (char)*read_size, &Rsize );
#else

	read_size = 250;
	/* read file */
	while (i51AdeOsFileRead(file_analysis, analysis, sizeof (short)*read_size,&Rsize ))
	{
#endif
		frames ++;

		/* call decoder */
		Decoder_Interface_Decode(destate, analysis, synth, 0);

		i51AdeOsFileWrite(file_speech, synth, sizeof (short)*160, &Rsize);
	}

	Decoder_Interface_exit(destate);

	i51AdeOsFileClose(file_speech);
	i51AdeOsFileClose(file_analysis);
	i51AdeOsLog (0, "\n%s%i%s\n","Decoded ", frames, " frames.");

	return 1;
}

static int amr_pcm_buff_write_tofile(void * buff, iS32 length, const iU16 *fileName, iBOOL voxtype,iU16  sample)
{/*  voxtype 为 文件格式 0 AMR 1 PCM*/
	iFILE saving_file;

	char wavhead[44];
	iBOOL exists = iFALSE;
	iU16 sample_rate = sample;
	unsigned int Rsize = 0;
#ifndef ETSI
#ifndef IF2
		exists = i51AdeOsFileIsExist (fileName);
		saving_file = i51AdeOsFileOpen (fileName, i51_OS_FL_ALWAYS_OPEN|i51_OS_FL_WRITE, __FILE__ , __LINE__);
		if (saving_file <= 0 || buff == iNULL){
			i51AdeOsLog(0, "%s%s%s\n","Use:  input.file output.file " );
			return iNULL;
		}
		if (!exists)
		{
			if (voxtype)
			{
				if (sample_rate > 9999)
				{
					i51AdeStdMemcpy16(wavhead, WavHead,44);
					WavHead[24] = 0x40;
					WavHead[25] = 0x1f;
					WavHead[28] = 0x80;
					WavHead[29] = 0x3e;
					i51AdeOsFileWrite(saving_file, wavhead, sizeof (char)*44, &Rsize);
                    i51AdeOsFileSetSeek(saving_file, 24, i51_OS_FL_SEEK_HEAD);
					i51AdeOsFileWrite(saving_file, &sample_rate, 2, &Rsize);
					i51AdeOsFileSetSeek(saving_file, 44, i51_OS_FL_SEEK_HEAD);
				}
				else
				{
					i51AdeOsFileWrite(saving_file, WavHead, sizeof (char)*44, &Rsize);
				}
			}
			else
			{
				i51AdeOsFileWrite(saving_file, AMR_MAGIC_NUMBER,  sizeof( char )*strlen( AMR_MAGIC_NUMBER ), &Rsize);
			}			
		}
		else
		{
			i51AdeOsFileSetSeek(saving_file, 0, i51_OS_FL_SEEK_TAIL);
		}
		i51AdeOsFileWrite(saving_file, buff, length, &Rsize);
		i51AdeOsFileClose(saving_file);
	
#endif
#endif
	return 1;
}
#endif