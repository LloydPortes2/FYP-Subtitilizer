#include "video_reader.hpp"
bool video_reader_open(VideoReaderState* state, const char* filename) {
	
	//unpack members of state
	auto& width = state->width;
	auto& height = state->height;
	auto& time_base =state->time_base;
	auto& av_format_contex = state->av_format_contex;
	auto& av_codec_contex = state->av_codec_contex;
	auto& av_frames = state->av_frames;
	auto& av_packets = state->av_packets;
	auto& video_stream_index = state->video_stream_index;
	auto& audio_stream_index = state->audio_stream_index;

	

	printf("Beginning to encode\n");
	// declare format and codec contexts, also codec for decoding
	
	AVCodecParameters* av_codec_parameters =nullptr;
	const AVCodec* av_codec = nullptr;

	const AVCodec* Codec = NULL;
	video_stream_index = -1;

	AVFrame* frame = NULL;
	AVPacket* packet = NULL;

	//allocates memory for format context
	av_format_contex = avformat_alloc_context();
	if (!av_format_contex) {
		printf("Could not allocate memory for AVFormatContext\n");
		return false;
	}
	//open input file
	if (avformat_open_input(&av_format_contex, filename, NULL, NULL) != 0) {
		av_log(NULL, AV_LOG_ERROR, "File could not be opened\n");
		return false;
	}

	// get video and audio stream index
	for (int i = 0; i < av_format_contex->nb_streams; i++){
		AVStream* av_streams = av_format_contex->streams[i];
		av_codec_parameters = av_format_contex->streams[i]->codecpar;
		av_codec = avcodec_find_decoder(av_codec_parameters->codec_id);

		if (!av_codec) {
			continue;
		}
		if (av_codec_parameters->codec_type == AVMEDIA_TYPE_VIDEO) {
			video_stream_index = i;
			width = av_codec_parameters->width;
			height = av_codec_parameters->height;
			time_base =av_format_contex->streams[i]->time_base;
			break;
		}

		if (av_codec_parameters->codec_type == AVMEDIA_TYPE_AUDIO) {
			audio_stream_index = i;
			break;
		}
	}

	// if no video stream found, exit
	if (video_stream_index == -1)
	{
		av_log(NULL, AV_LOG_ERROR, "Error: No video stream found\n");
		return false;
	}
	av_dump_format(av_format_contex, video_stream_index, filename, false);

	// if no audio stream found, exit
	if (audio_stream_index == -1)
	{
		av_log(NULL, AV_LOG_ERROR, "Error: No audio stream found\n");
		return false;
	}
	//Setting up codec context for decoding
	av_codec_contex = avcodec_alloc_context3(av_codec);

	if (!av_codec_contex) {
		printf("Error: Unable to create AVCodecContext. Codec context initialization failed\n");
		return false;
	}

	if (avcodec_parameters_to_context(av_codec_contex, av_codec_parameters) < 0) {
		printf("Error: Failed to initialize AVCodecContext. Codec parameters conversion error\n");
		return false;
	}
	// open codec
	if (avcodec_open2(av_codec_contex, av_codec, NULL) < 0) {
		printf("Error: Failed to open the codec. Codec opening failed\n");
		return false;
	}

	av_frames = av_frame_alloc();
	if (!av_frames) {
		printf("Error: failed to allocate memory for AVFrame\n");
		return false;
	}
	av_packets = av_packet_alloc();
	if (!av_packets) {
		printf("Error: failed to allocate memory AVPacket\n");
		return false;
	}

	return true;
}

bool video_reader_read_frame(VideoReaderState* state, uint8_t* frame_buffer, int64_t* pts) {
	auto& width = state->width;
	auto& height = state->height;
	auto& av_format_contex = state->av_format_contex;
	auto& av_codec_contex = state->av_codec_contex;
	auto& av_frames = state->av_frames;
	auto& av_packets = state->av_packets;
	auto& video_stream_index = state->video_stream_index;
	auto& sws_scaler_context = state->sws_scaler_context;


	//Decodes one frame
	int response;
	while (av_read_frame(av_format_contex, av_packets) >= 0) {
		if (av_packets->stream_index != video_stream_index) {
			av_packet_unref(av_packets);
			continue;
		}
		response = avcodec_send_packet(av_codec_contex, av_packets);
		if (response < 0) {
			char errbuf[AV_ERROR_MAX_STRING_SIZE];
			av_strerror(response, errbuf, AV_ERROR_MAX_STRING_SIZE);
			printf("Failed to decode packet: %s\n", errbuf);
			return false;
		}

		response = avcodec_receive_frame(av_codec_contex, av_frames);
		if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
			av_packet_unref(av_packets);
			continue;
		}
		else if (response < 0) {
			char errbuf[AV_ERROR_MAX_STRING_SIZE];
			av_strerror(response, errbuf, AV_ERROR_MAX_STRING_SIZE);
			printf("Failed to decode packet: %s\n", errbuf);
			return false;
		}


		av_packet_unref(av_packets);
		break;
	}

	*pts = av_frames->pts;

	printf(
		"Frame %c (%d) pts %d dts %d key_frame %d [coded_picture_number %d, display_picture_number %d\n]",
		av_get_picture_type_char(av_frames->pict_type),
		av_codec_contex->frame_number,//represents the frame number or index of the current frame
		av_frames->pts,//represents the presentation timestamp of the frame
		av_frames->pkt_dts,//represents the decoding timestamp of the frame
		av_frames->key_frame,//indicates whether the frame is a key frame (1) or not (0)
		av_frames->coded_picture_number,//represents the coded picture number of the frame
		av_frames->display_picture_number//represents the display picture number of the frame
	);
	// Sets up sws scaler
	if (!sws_scaler_context) {
		sws_scaler_context = sws_getContext(width, height, av_codec_contex->pix_fmt,
										width, height, AV_PIX_FMT_RGB0,
										SWS_BILINEAR, NULL, NULL, NULL);
	}

	if (!sws_scaler_context) {
		printf("couldn't initialize sws scaler\n");
		return false;
	}

	uint8_t* dest[4] = { frame_buffer, NULL, NULL, NULL };
	int dest_linesize[4] = { av_frames->width * 4, 0, 0, 0 };
	sws_scale(sws_scaler_context, av_frames->data, av_frames->linesize, 0, av_frames->height, dest, dest_linesize);

	return true;

}

	void video_reader_close(VideoReaderState* state) {
		sws_freeContext(state->sws_scaler_context);
		avformat_close_input(&state->av_format_contex);
		avformat_free_context(state->av_format_contex);
		av_frame_free(&state->av_frames);
		av_packet_free(&state->av_packets);
		avcodec_free_context(&state->av_codec_contex);
	}
	