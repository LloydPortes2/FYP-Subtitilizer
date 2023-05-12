#include "video_reader.hpp"

bool video_output_file(VideoReaderState* state, const char* filename, const char* out_filename);

	auto& width = state->width;
	auto& height = state->height;
	auto& time_base = state->time_base;
	auto& av_format_ctx = state->av_format_ctx;
	auto& av_codec_ctx = state->av_codec_ctx;
	auto& av_frame = state->av_frame;
	auto& av_packet = state->av_packet;
	auto& video_stream_index = state->video_stream_index;
	auto& audio_stream_index = state->audio_stream_index;

	avformat_network_init();

	AVFormatContext * out_format_ctx = NULL;
	int response;

	// Open the input file
	if ((response = avformat_open_input(&av_format_ctx, filename, NULL, NULL)) < 0) {
		fprintf(stderr, "Could not open input file '%s' (error '%s')\n", filename, av_err2str(response));
		response false;
	}

	// Retrieve stream information
	if (avformat_find_stream_info(av_format_ctx, NULL) < 0) {
		fprintf(stderr, "Could not find stream information\n");
		response false;
	}
	//Finding the best streams 
	video_stream_index = av_find_best_stream(in_format_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	audio_stream_index = av_find_best_stream(in_format_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	//subtitle_stream_index = av_find_best_stream(in_format_ctx, AVMEDIA_TYPE_SUBTITLE, -1, -1, NULL, 0);

	// Open the output file
	response = avformat_alloc_output_context2(&out_format_ctx, NULL, NULL, out_filename);
		if (response < 0) {
			fprintf(stderr, "Could not create output context (error '%s')\n", av_err2str(response));
			response false;
		}

	// Add a video and audio stream to the output file
		AVStream* in_audio_stream = in_format_ctx->streams[audio_stream_index];
		AVStream* in_video_stream = in_format_ctx->streams[video_stream_index];
	//	AVStream* in_subtitle_stream = in_format_ctx->streams[subtitle_stream_index];

		AVStream* out_audio_stream = avformat_new_stream(out_format_ctx, NULL);
		AVStream* out_video_stream = avformat_new_stream(out_format_ctx, NULL);
	//	AVStream* out_subtitle_stream = avformat_new_stream(out_format_ctx, NULL);

		avcodec_parameters_copy(out_audio_stream->codecpar, in_audio_stream->codecpar);
		avcodec_parameters_copy(out_video_stream->codecpar, in_video_stream->codecpar);
	//	avcodec_parameters_copy(out_subtitle_stream->codecpar, in_subtitle_stream->codecpar);

		out_audio_stream->time_base = in_audio_stream->time_base;
		out_video_stream->time_base = in_video_stream->time_base;
		out_subtitle_stream->time_base = in_subtitle_stream->time_base;

		// allocate the codec context for the output audio stream and configure it
		AVCodecContext* out_audio_codec_context = avcodec_alloc_context3(NULL);
		if (!out_audio_codec_context) {
			printf("Error: Could not allocate output audio codec context\n");
			return 1;
		}

		response = avcodec_parameters_to_context(out_audio_codec_context, out_audio_stream->codecpar);
		if (response < 0) {
			printf("Error: Could not initialize output audio codec context\n");
			return 1;
		}

		out_audio_codec_context->time_base = out_audio_stream->time_base;

		// find the encoder for the output audio stream and open it
		AVCodec* out_audio_codec = avcodec_find_encoder(out_audio_codec_context->codec_id);
		if (!out_audio_codec) {
			printf("Error: Could not find output audio codec\n");
			return 1;
		}

		response = avcodec_open2(out_audio_codec_context, out_audio_codec, NULL);
		if (response < 0) {
			printf("Error: Could not open output audio codec\n");
			return 1;
		}

		// allocate the frame for the output audio stream
		AVFrame* out_audio_frame = av_frame_alloc();
		if (!out_audio_frame) {
			printf("Error: Could not allocate output audio frame\n");
			return 1;
		}

		out_audio_frame->format = out_audio_codec_context->sample_fmt;
		out_audio_frame->channel_layout = out_audio_codec_context->channel_layout;
		out_audio_frame->sample_rate = out_audio_codec_context->sample_rate;

		response = av_frame_get_buffer(out_audio_frame, 0);
		if (response < 0) {
			printf("Error: Could not allocate output audio frame samples\n");
			return 1;
		}

		// allocate the resampler context and configure it
		SwrContext* resampler_context = swr_alloc_set_opts(NULL,
			out_audio_codec_context->channel_layout, out_audio_codec_context->sample_fmt, out_audio_codec_context->sample_rate,
			in_audio_stream->codecpar->channel_layout, in_audio_stream->codecpar->format, in_audio_stream->codecpar->sample_rate,
			0, NULL);
		if (!resampler_context) {
			printf("Error: Could not allocate audio resampler context\n");
			return 1;
		}

		response = swr_init(resampler_context);
		if (response < 0) {
			printf("Error: Could not initialize audio resampler context\n");
			return 1;
		}

		// allocate the packet for the output audio stream
		AVPacket* out_audio_packet = av_packet_alloc();
		if (!out_audio_packet) {
			printf("Error: Could not allocate output audio packet\n");
			return 1;
		}

		out_audio_packet->stream_index = out_audio_stream->index;

		// allocate the codec context for the output video stream and configure it
		AVCodecContext* out_video_codec_context = avcodec_alloc_context3(NULL);
		if (!out_video_codec_context) {
			printf("Error: Could not allocate output video codec context\n");
			return 1;
		}
	
		AVCodecContext* out_video_codec_context = avcodec_alloc_context3(NULL);
		if (!out_video_codec_context) {
			printf("Error: Could not allocate output video codec context\n");
			return 1;
		}

		// initialize output video codec context
		out_video_codec_context->codec_id = out_video_codec->id;
		out_video_codec_context->codec_type = AVMEDIA_TYPE_VIDEO;
		out_video_codec_context->pix_fmt = out_video_codec->pix_fmts[0];
		out_video_codec_context->width = in_video_stream->codecpar->width;
		out_video_codec_context->height = in_video_stream->codecpar->height;
		out_video_codec_context->bit_rate = in_video_stream->codecpar->bit_rate;

		AVRational framerate = { in_video_stream->r_frame_rate.den, in_video_stream->r_frame_rate.num };
		out_video_codec_context->framerate = av_inv_q(framerate);

		response = avcodec_open2(out_video_codec_context, out_video_codec, NULL);
		if (response < 0) {
			printf("Error: Could not open output video codec\n");
			return 1;
		}

		// allocate output video frame
		AVFrame* out_frame = av_frame_alloc();
		if (!out_frame) {
			printf("Error: Could not allocate output video frame\n");
			return 1;
		}
		out_frame->format = out_video_codec_context->pix_fmt;
		out_frame->width = out_video_codec_context->width;
		out_frame->height = out_video_codec_context->height;
		out_frame->pts = 0;

		// initialize subtitle codec
		AVCodec* out_subtitle_codec = avcodec_find_encoder(out_format_ctx->oformat->subtitle_codec);
		if (!out_subtitle_codec) {
			printf("Error: Could not find subtitle encoder\n");
			return 1;
		}

		// initialize subtitle codec context
		AVCodecContext* out_subtitle_codec_context = avcodec_alloc_context3(out_subtitle_codec);
		if (!out_subtitle_codec_context) {
			printf("Error: Could not allocate output subtitle codec context\n");
			return 1;
		}

		out_subtitle_codec_context->codec_id = out_subtitle_codec->id;
		out_subtitle_codec_context->codec_type = AVMEDIA_TYPE_SUBTITLE;
		out_subtitle_codec_context->time_base = (AVRational){ 1, 1000 };

		response = avcodec_open2(out_subtitle_codec_context, out_subtitle_codec, NULL);
		if (response < 0) {
			printf("Error: Could not open output subtitle codec\n");
			return 1;
		}

		// write output format header
		response = avformat_write_header(out_format_ctx, NULL);
		if (response < 0) {
			printf("Error: Could not write output file header\n");
			return 1;
		}

		// read packets from input file and write them to output file
		AVPacket packet;
		while (av_read_frame(in_format_ctx, &packet) == 0) {
			// determine which stream the packet belongs to
			AVStream* stream = in_format_ctx->streams[packet.stream_index];

			if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
				// decode and encode the video frame
				response = avcodec_send_packet(out_video_codec_context, &packet);
				if (response < 0) {
					printf("Error: Could not send video packet for decoding\n");
					return 1;
				}
				while (1) {
					response = avcodec_receive_frame(out_video_codec_context, out_frame);
					if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
						break;
					}
					else if (response < 0) {
						printf("Error: Error receiving output video frame\n");
						return 1;
					}

					// encode the output video frame
					response = avcodec_send_frame(out_video_codec_context, out_frame);
					if (response < 0) {
						printf("Error: Error sending output video frame for encoding\n");
						return 1;
					}

					while (1) {
						response = avcodec_receive_packet(out_video_codec_context, out_packet);
						if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
							break;
						}
						else if (response < 0) {
							printf("Error: Error encoding output video packet\n");
							return 1;
						}

						// rescale the output video packet's timestamps
						out_packet->stream_index = out_video_stream->index;
						av_packet_rescale_ts(out_packet, out_video_codec_context->time_base, out_video_stream->time_base);

						// write the output video packet to the output file
						response = av_interleaved_write_frame(out_format_ctx, out_packet);
						if (response < 0) {
							printf("Error: Error writing output video packet to file\n");
							return 1;
						}

						av_packet_unref(out_packet);
					}

					av_frame_unref(out_frame);
				}
			}
	}