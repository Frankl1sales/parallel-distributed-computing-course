#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <omp.h>

FILE *log_file;

void log_message(const char *message) {
    if (log_file) {
        fprintf(log_file, "%s\n", message);
        fflush(log_file); // Garantir que a mensagem seja escrita imediatamente
    }
}

// Função para aplicar um filtro (exemplo simples)
void apply_filter(uint8_t* image, int width, int height, int linesize) {
    log_message("Início da aplicação do filtro.");
    #pragma omp parallel for
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int idx = y * linesize + x * 3; // Assumindo imagem RGB
            // Aplicar um filtro de média simples
            uint8_t r = (image[idx] + image[idx - 3] + image[idx + 3] + image[idx - linesize * 3] + image[idx + linesize * 3]) / 5;
            uint8_t g = (image[idx + 1] + image[idx - 2] + image[idx + 4] + image[idx - linesize * 3 + 1] + image[idx + linesize * 3 + 1]) / 5;
            uint8_t b = (image[idx + 2] + image[idx - 1] + image[idx + 5] + image[idx - linesize * 3 + 2] + image[idx + linesize * 3 + 2]) / 5;
            image[idx] = r;
            image[idx + 1] = g;
            image[idx + 2] = b;
        }
    }
    log_message("Término da aplicação do filtro.");
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        log_file = fopen("processamento_imagem.log", "w");
        if (!log_file) {
            fprintf(stderr, "Erro ao abrir arquivo de log.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        log_message("Início do processamento de vídeo.");
    }

    if (argc != 3) {
        if (rank == 0) {
            fprintf(stderr, "Uso: %s <input_file> <output_file>\n", argv[0]);
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    const char* input_filename = argv[1];
    const char* output_filename = argv[2];

    av_register_all();

    AVFormatContext* format_ctx = NULL;
    if (avformat_open_input(&format_ctx, input_filename, NULL, NULL) < 0) {
        if (rank == 0) {
            fprintf(stderr, "Não foi possível abrir o arquivo de entrada\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if (avformat_find_stream_info(format_ctx, NULL) < 0) {
        if (rank == 0) {
            fprintf(stderr, "Não foi possível encontrar informações do stream\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    AVCodec* codec = NULL;
    AVCodecContext* codec_ctx = NULL;
    AVStream* video_stream = NULL;

    for (int i = 0; i < format_ctx->nb_streams; ++i) {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream = format_ctx->streams[i];
            codec = avcodec_find_decoder(video_stream->codecpar->codec_id);
            codec_ctx = avcodec_alloc_context3(codec);
            if (!codec_ctx) {
                if (rank == 0) {
                    fprintf(stderr, "Não foi possível alocar o contexto do codec\n");
                }
                MPI_Finalize();
                return EXIT_FAILURE;
            }
            avcodec_parameters_to_context(codec_ctx, video_stream->codecpar);
            if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
                if (rank == 0) {
                    fprintf(stderr, "Não foi possível abrir o codec\n");
                }
                MPI_Finalize();
                return EXIT_FAILURE;
            }
            break;
        }
    }

    if (!video_stream) {
        if (rank == 0) {
            fprintf(stderr, "Não foi encontrado um stream de vídeo\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    AVFrame* frame = av_frame_alloc();
    AVFrame* frame_rgb = av_frame_alloc();
    if (!frame || !frame_rgb) {
        if (rank == 0) {
            fprintf(stderr, "Não foi possível alocar frames\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    int num_bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codec_ctx->width, codec_ctx->height, 1);
    uint8_t* buffer = (uint8_t*)av_malloc(num_bytes * sizeof(uint8_t));
    av_image_fill_arrays(frame_rgb->data, frame_rgb->linesize, buffer, AV_PIX_FMT_RGB24, codec_ctx->width, codec_ctx->height, 1);

    struct SwsContext* sws_ctx = sws_getContext(
        codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
        codec_ctx->width, codec_ctx->height, AV_PIX_FMT_RGB24,
        SWS_BILINEAR, NULL, NULL, NULL
    );

    AVFormatContext* output_format_ctx = NULL;
    AVStream* output_stream = NULL;
    AVCodecContext* output_codec_ctx = NULL;

    if (rank == 0) {
        avformat_alloc_output_context2(&output_format_ctx, NULL, NULL, output_filename);
        if (!output_format_ctx) {
            fprintf(stderr, "Não foi possível criar o contexto de saída\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        AVCodec* output_codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (!output_codec) {
            fprintf(stderr, "Codec H.264 não encontrado\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        output_stream = avformat_new_stream(output_format_ctx, output_codec);
        if (!output_stream) {
            fprintf(stderr, "Não foi possível criar o stream de saída\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        output_codec_ctx = avcodec_alloc_context3(output_codec);
        if (!output_codec_ctx) {
            fprintf(stderr, "Não foi possível alocar o contexto do codec de saída\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        output_codec_ctx->codec_id = output_codec->id;
        output_codec_ctx->bit_rate = 400000; // Taxa de bits
        output_codec_ctx->width = codec_ctx->width;
        output_codec_ctx->height = codec_ctx->height;
        output_codec_ctx->time_base = (AVRational){1, 25}; // 25 fps
        output_codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

        avcodec_parameters_from_context(output_stream->codecpar, output_codec_ctx);

        if (avcodec_open2(output_codec_ctx, output_codec, NULL) < 0) {
            fprintf(stderr, "Não foi possível abrir o codec de saída\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        if (avio_open(&output_format_ctx->pb, output_filename, AVIO_FLAG_WRITE) < 0) {
            fprintf(stderr, "Não foi possível abrir o arquivo de saída\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        if (avformat_write_header(output_format_ctx, NULL) < 0) {
            fprintf(stderr, "Não foi possível escrever o cabeçalho do arquivo de saída\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
    }

    AVPacket packet;
    while (av_read_frame(format_ctx, &packet) >= 0) {
        if (packet.stream_index == video_stream->index) {
            avcodec_send_packet(codec_ctx, &packet);
            while (avcodec_receive_frame(codec_ctx, frame) == 0) {
                sws_scale(sws_ctx, (const uint8_t* const*)frame->data, frame->linesize, 0, codec_ctx->height, frame_rgb->data, frame_rgb->linesize);

                // Dividir o trabalho entre os processos MPI
                int start = (frame_rgb->height / size) * rank;
                int end = (rank == size - 1) ? frame_rgb->height : (frame_rgb->height / size) * (rank + 1);

                #pragma omp parallel
                {
                    #pragma omp single nowait
                    {
                        // Processar o bloco com OpenMP
                        apply_filter(frame_rgb->data[0] + start * frame_rgb->linesize[0], codec_ctx->width, end - start, frame_rgb->linesize[0]);
                    }
                }

                if (rank == 0) {
                    AVFrame* output_frame = av_frame_alloc();
                    if (!output_frame) {
                        fprintf(stderr, "Não foi possível alocar o frame de saída\n");
                        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
                    }

                    av_image_fill_arrays(output_frame->data, output_frame->linesize, buffer, AV_PIX_FMT_YUV420P, codec_ctx->width, codec_ctx->height, 1);

                    // Converte RGB para YUV420P
                    struct SwsContext* rgb_to_yuv_ctx = sws_getContext(
                        codec_ctx->width, codec_ctx->height, AV_PIX_FMT_RGB24,
                        codec_ctx->width, codec_ctx->height, AV_PIX_FMT_YUV420P,
                        SWS_BILINEAR, NULL, NULL, NULL
                    );
                    sws_scale(rgb_to_yuv_ctx, (const uint8_t* const*)frame_rgb->data, frame_rgb->linesize, 0, codec_ctx->height, output_frame->data, output_frame->linesize);
                    sws_freeContext(rgb_to_yuv_ctx);

                    AVPacket out_packet;
                    av_init_packet(&out_packet);
                    out_packet.data = NULL;
                    out_packet.size = 0;

                    if (avcodec_send_frame(output_codec_ctx, output_frame) >= 0) {
                        while (avcodec_receive_packet(output_codec_ctx, &out_packet) >= 0) {
                            av_write_frame(output_format_ctx, &out_packet);
                            av_packet_unref(&out_packet);
                        }
                    }

                    av_frame_free(&output_frame);
                }
            }
        }
        av_packet_unref(&packet);
    }

    if (rank == 0) {
        av_write_trailer(output_format_ctx);
        avio_closep(&output_format_ctx->pb);
        avformat_free_context(output_format_ctx);
    }
    
    av_free(buffer);
    av_frame_free(&frame_rgb);
    av_frame_free(&frame);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);
    sws_freeContext(sws_ctx);
      
    if (rank == 0) {
        log_message("Finalizando processamento de vídeo.");
        fclose(log_file);
    }
    
    MPI_Finalize();
    return 0;
}

