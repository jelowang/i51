/*
 * ===================================================================
 *  TS 26.104
 *  REL-5 V5.4.0 2004-03
 *  REL-6 V6.1.0 2004-03
 *  3GPP AMR Floating-point Speech Codec
 * ===================================================================
 *
 */

/*
 * sp_enc.h
 *
 *
 * Project:
 *    AMR Floating-Point Codec
 *
 * Contains:
 *    Defines interface to AMR encoder
 *
 */
#ifndef _SP_DEC_H_
#define _SP_DEC_H_
typedef void * pvoid;

#define  labs(x) ( (x) > (0) ? (x):-(x))

#define  abs(x) ( (x) > (0) ? (x):-(x))
/*
 * definition of enumerated types
 */

/*
 * definition of modes for decoder
 */
enum Mode { MR475 = 0,
            MR515,
            MR59,
            MR67,
            MR74,
            MR795,
            MR102,
            MR122,
            MRDTX,
            N_MODES     /* number of (SPC) modes */
};

/* Declaration recieved frame types */
enum RXFrameType { RX_SPEECH_GOOD = 0,
                   RX_SPEECH_DEGRADED,
                   RX_ONSET,
                   RX_SPEECH_BAD,
                   RX_SID_FIRST,
                   RX_SID_UPDATE,
                   RX_SID_BAD,
                   RX_NO_DATA,
                   RX_N_FRAMETYPES     /* number of frame types */
};

/*
 * Function prototypes
 */

/*
 * initialize one instance of the speech decoder
 */
void* Speech_Decode_Frame_init ();

/*
 * i51AdeOsFree status struct
 */
void Speech_Decode_Frame_exit (void *st);

/*
 * Decodes one frame from encoded parameters
 */
void Speech_Decode_Frame (void *st, enum Mode mode, short *serial,
                   enum RXFrameType frame_type, short *synth);

/*
 * reset speech decoder
 */
int Speech_Decode_Frame_reset (void *st);

//extern int amrfile_shift_pcmfile (const iU16 * rfilename,const iU16 * wfilename);
//extern int amr_pcm_buff_write_tofile(void * buff, iS32 length, const iU16 *fileName, iBOOL voxtype,iU16  sample);

extern iS32 amrbuff_shift_pcmbuff (char * rbuff, iU32 rlength, char * wbuff, iU32  wlength);

#endif

