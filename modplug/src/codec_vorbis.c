/* codec_vorbis.c
 * Read and write Ogg Vorbis samples for ITQ files
 * Mostly ripped from example_encoder.c in libvorbisenc by Xiph.org
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vorbis/vorbisenc.h"
#include "codec.h"

#define READ 2048

/* Write a sample buffer to an ITQ file 
 *   insample: pointer to sample data
 *   len:      size in bytes of sample
 *   outfile:  pointer to file where vorbis data gets written
 *   channels: number of channels (1 = mono, 2 = stereo)
 *   rate:     sample rate of sample
 *   quality:  vorbis quality (-0.1 to 1.0)
 */
int write_vorbis_sample(signed char* insample, unsigned long len, FILE* outfile,
	long channels, long bitdepth, long rate, float quality)
{
	int eos = 0;
	int ret;
	unsigned long readpos = 0;
	unsigned long totalsamps = 0;
	ogg_stream_state os;
	ogg_page         og;
	ogg_packet       op;
	vorbis_info      vi;
	vorbis_comment   vc;
	vorbis_dsp_state vd;
	vorbis_block     vb;

	vorbis_info_init(&vi);
	ret = vorbis_encode_init_vbr(&vi, channels, rate, quality);
	if (ret)
		return ret;
	vorbis_comment_init(&vc);
	vorbis_comment_add_tag(&vc, "ENCODER", "ITQ inside hacked libmodplug");
	vorbis_analysis_init(&vd, &vi);
	vorbis_block_init(&vd, &vb);
	srand(time(NULL));
	ogg_stream_init(&os,rand());
	{
		ogg_packet header;
		ogg_packet header_comm;
		ogg_packet header_code;
		vorbis_analysis_headerout(&vd, &vc, &header, &header_comm, &header_code);
		ogg_stream_packetin(&os, &header);
		ogg_stream_packetin(&os, &header_comm);
		ogg_stream_packetin(&os, &header_code);
		while (!eos)
		{
			int result = ogg_stream_flush(&os, &og);
			if (result == 0)
				break;
			fwrite(og.header, 1, og.header_len, outfile);
			fwrite(og.body, 1, og.body_len, outfile);
		}
	}

	printf("Encoding sample...\n");
	printf("Qualtiy:   %1.1f\n", quality);
	printf("Bit depth: %ld\n", bitdepth);
	printf("Channels:  %ld\n", channels);
	printf("Bytes:     %ld\n", len);
	while (!eos)
	{
		int bytespersamp = (bitdepth / 8);
		int ch;
		unsigned long samp = 0;
		float** buffer = vorbis_analysis_buffer(&vd, READ);

		while ((readpos < len) && (samp < READ))
		{
			for (ch = 0; ch < channels; ch++)
			{
				if (bitdepth == 8)
				{
					buffer[ch][samp] = insample[readpos] / 128.f;
				}
				else if (bitdepth == 16)
				{
					buffer[ch][samp] = ((insample[readpos + 1] << 8)
						| (0x00ff & (int)insample[readpos])) / 32768.f;
				}
				else
					return -1;
				readpos += bytespersamp;
			}
			samp++;
			totalsamps++;
		}
		vorbis_analysis_wrote(&vd, samp);
		while (vorbis_analysis_blockout(&vd, &vb) == 1)
		{
			vorbis_analysis(&vb, NULL);
			vorbis_bitrate_addblock(&vb);
			while (vorbis_bitrate_flushpacket(&vd, &op))
			{
				ogg_stream_packetin(&os, &op);
				while (!eos)
				{
					int result = ogg_stream_pageout(&os, &og);
					if (result == 0)
						break;
					fwrite(og.header, 1, og.header_len, outfile);
					fwrite(og.body, 1, og.body_len, outfile);
					if (ogg_page_eos(&og))
						eos = 1;
				}
			}
		}
	}
	printf("Finished. Wrote %lu samples.\n", totalsamps);
	printf("--------------------------------\n");
	ogg_stream_clear(&os);
	vorbis_block_clear(&vb);
	vorbis_dsp_clear(&vd);
	vorbis_comment_clear(&vc);
	vorbis_info_clear(&vi);

	return 0;
}

#ifdef __cplusplus
}
#endif
