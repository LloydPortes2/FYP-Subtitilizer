#ifndef video_reader_hpp
#define video_reader_hpp

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
#include <libswresample/swresample.h>
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavcodec/avcodec.h"
#include "libavutil/audio_fifo.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/frame.h"
#include "libavutil/opt.h"
#include "libswresample/swresample.h"
#include "stdlib.h"

}

struct VideoReaderState {
	//Public things for other parts of the program to use
	int width, height;
	AVRational time_base;

	//Private things for the video reader to use
	AVFormatContext* av_format_contex;
	AVCodecContext* av_codec_contex;
	AVFrame* av_frames;
	AVPacket* av_packets;
	AVStream* av_stream;
	AVCodec *av_codec;
	
	int video_stream_index;
	int audio_stream_index;
	struct SwsContext* sws_scaler_context = NULL;


	
};

bool video_reader_open(VideoReaderState* state, const char* filename);
bool video_reader_read_frame(VideoReaderState* state, uint8_t* frame_buffer, int64_t* pts);
void video_reader_close(VideoReaderState* state);
#endif // !video_reader_hpp
