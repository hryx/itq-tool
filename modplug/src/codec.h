/* codec.h
 * Functions for reading/writing Voribs/FLAC sample data
 */

#ifndef _ITQ_CODEC_H_
#define _ITQ_CODEC_H_

#ifdef __cplusplus
extern "C" {
#endif


int write_vorbis_sample(signed char* insample, unsigned long len, FILE* outfile,
	long channels, long bitdepth, long rate, float quality);


#ifdef __cplusplus
}
#endif

#endif /* _ITQ_CODEC_H_ */
