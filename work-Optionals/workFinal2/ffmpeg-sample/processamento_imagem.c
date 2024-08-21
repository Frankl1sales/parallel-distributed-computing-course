#include <stdio.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <video_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    AVFormatContext *fmt_ctx = NULL;
    int video_stream_index;
    AVCodecContext *codec_ctx = NULL;
    AVStream *video_stream = NULL;

    avformat_network_init();

    if (avformat_open_input(&fmt_ctx, filename, NULL, NULL) < 0) {
        fprintf(stderr, "Could not open source file %s\n", filename);
        return 1;
    }

    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        avformat_close_input(&fmt_ctx);
        return 1;
    }

    video_stream_index = -1;
    for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }

    if (video_stream_index == -1) {
        fprintf(stderr, "Could not find a video stream\n");
        avformat_close_input(&fmt_ctx);
        return 1;
    }

    video_stream = fmt_ctx->streams[video_stream_index];
    AVCodecParameters *codec_params = video_stream->codecpar;
    AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
    if (!codec) {
        fprintf(stderr, "Unsupported codec!\n");
        avformat_close_input(&fmt_ctx);
        return 1;
    }

    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        fprintf(stderr, "Could not allocate audio codec context\n");
        avformat_close_input(&fmt_ctx);
        return 1;
    }

    if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0) {
        fprintf(stderr, "Failed to copy codec parameters to context\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return 1;
    }

    if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return 1;
    }

    AVFrame *frame = av_frame_alloc();
    AVPacket packet;

    while (av_read_frame(fmt_ctx, &packet) >= 0) {
        if (packet.stream_index == video_stream_index) {
            if (avcodec_send_packet(codec_ctx, &packet) == 0) {
                while (avcodec_receive_frame(codec_ctx, frame) == 0) {
                    printf("Frame %d (type=%c, size=%d)\n", codec_ctx->frame_number, av_get_picture_type_char(frame->pict_type), frame->pkt_size);
                }
            }
        }
        av_packet_unref(&packet);
    }

    av_frame_free(&frame);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);

    return 0;
}

