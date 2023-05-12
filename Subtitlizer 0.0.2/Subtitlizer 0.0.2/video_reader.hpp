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
#include <libavutil/imgutils.h>
#include <math.h>
}

struct VideoReaderState {
	//Public things for other parts of the program to use
	int width, height;
	AVRational time_base;

	//Private things for the video reader to use
	AVFormatContext* av_format_ctx;
	AVCodecContext* av_codec_ctx;
	AVFrame* av_frame;
	AVPacket* av_packet;
	AVStream* av_stream;
	AVCodec* av_codec;
	AVFrame* tmp_picture = nullptr;
	int video_stream_index;
	int audio_stream_index;
	struct SwsContext* sws_scaler_ctx = NULL;
	float t, tincr, tincr2;
	int16_t* samples;
	uint8_t* audio_outbuf;
	uint8_t* video_outbuf;
	int frame_count, video_outbuf_size;
	int audio_outbuf_size;
	int audio_input_frame_size;


};

bool video_reader_open(VideoReaderState* state, const char* filename);
bool video_reader_read_frame(VideoReaderState* state, uint8_t* frame_buffer, int64_t* pts);

AVStream* add_audio_stream(AVFormatContext* av_format_ctx, enum AVCodecID codec_id);
AVStream* open_audio(AVFormatContext* av_format_ctx, AVStream* av_stream);
void get_audio_frame(int16_t* samples, int frame_size, int nb_channels); //
void write_audio_frame(AVFormatContext* av_format_ctx, AVStream* av_stream, AVPacket* av_packet, double audio_pts); //
AVStream* add_video_stream(AVFormatContext* av_format_ctx, enum AVCodecID codec_id);
AVStream* open_video(AVFormatContext* av_format_ctx, AVStream* video_stream);
bool fill_yuv_image(AVFrame* pict, int frame_index, int width, int height);//
void write_video_frame(AVFormatContext* av_format_ctx, AVStream* av_stream, AVPacket* av_packet, double audio_pts);
//void close_audio(AVFormatContext* av_format_ctx, AVStream* av_stream);
//void close_video(AVFormatContext* av_format_ctx, AVStream* av_stream);
void video_reader_close(VideoReaderState* state);
#endif // !video_reader_hpp
