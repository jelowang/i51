/*
 * ===================================================================
 *  TS 26.104
 *  R99   V3.4.0 2002-02
 *  REL-4 V4.3.0 2002-02
 *  3GPP AMR Floating-point Speech Codec
 * ===================================================================
 *
 */

/*
 * interf_enc.h
 *
 *
 * Project:
 *    AMR Floating-Point Codec
 *
 * Contains:
 *    Defines interface to AMR encoder
 *
 */

#ifndef _interf_enc_h_
#define _interf_enc_h_

/*
 * include files
 */
#include"sp_enc.h"

//#include <windows.h>
#include "i51KitAde.h"

/*
 * Function prototypes
 */
/*
 * Encodes one frame of speech
 * Returns packed octets
 */
int Encoder_Interface_Encode( void *st, enum Mode mode, short *speech,

#ifndef ETSI
      unsigned char *serial,  /* max size 31 bytes */

#else
      short *serial, /* size 500 bytes */
#endif

      int forceSpeech );   /* use speech mode */

/*
 * Reserve and init. memory
 */
void *Encoder_Interface_init( int dtx );

/*
 * Exit and free memory
 */
void Encoder_Interface_exit( void *state );

/*  To use this exported function of dll, include this header
 *  in your project.
 */

//	Jelo Edited 20120321
#define DLL_EXPORT 
#if 0

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void *DLL_EXPORT encode_init(int dtx);
int DLL_EXPORT encode(void *st, enum Mode mode, short *speech, unsigned char *serial, int forceSpeech);
void DLL_EXPORT encode_exit(void *st);

#ifdef __cplusplus
}

#endif

#endif
#endif 
//	Jelo Edited Finished