#include "MyForm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include "video_reader.hpp"
#include <libavformat/avio.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/codec_id.h>
#include <libswscale/swscale.h>
}


using namespace Subtitlizer002;

int main()
{
	Application::Run(gcnew MyForm);


	printf("Beginning to encode\n");
	// declare format and codec contexts, also codec for decoding
	AVFormatContext* fmt_ctx = NULL;
	AVCodecContext* codec_ctx = NULL;
	AVCodec* Codec = NULL;
	int ret;
	char* filename = (char*)"D:\\Youtube Videos\\Needs Edtiting\\2021-06-25_08-12-42.mp4";
	int VideoStreamID = -1;



	// open input file
	if (ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL) < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "cannot open input file\n");
		goto end;
	}

	//get stream info
	if (ret = avformat_find_stream_info(fmt_ctx, NULL) < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "cannot get stream info\n");
		goto end;
	}

	// get video stream index
	for (int i = 0; i < fmt_ctx->nb_streams; i++)
	{
		if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			VideoStreamID = i;
			break;
		}
	}

	if (VideoStreamID < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "No video stream\n");
		goto end;
	}

	// dump video stream info
	av_dump_format(fmt_ctx, VideoStreamID, filename, false);

	//alloc memory for codec context
	codec_ctx = avcodec_alloc_context3(NULL);

	// retrieve codec params from format context
	if (ret = avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[VideoStreamID]->codecpar) < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "Cannot get codec parameters\n");
		goto end;
	}



	// find decoding codec
	//Codec = avcodec_find_decoder(codec_ctx->codec_id);



	if (Codec == NULL)
	{
		av_log(NULL, AV_LOG_ERROR, "No decoder found\n");
		goto end;
	}

	// try to open codec
	if (ret = avcodec_open2(codec_ctx, Codec, NULL) < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
		goto end;
	}

	printf("\nDecoding codec is : %s\n", Codec->name);


	// clear and out
end:
	if (codec_ctx)
		avcodec_close(codec_ctx);
	if (fmt_ctx)
		avformat_close_input(&fmt_ctx);

	printf("Ending decode\n");
	system("PAUSE");

	return 0;

}