# Nome do executável final
TARGET = compress_video_hybrid

# Compilador
CC = mpicc

# Flags de compilação
CFLAGS = -fopenmp -Wall -O2

# Bibliotecas necessárias
LIBS = -lavformat -lavcodec -lavutil -lm

# Arquivo fonte
SRCS = compress_video_hybrid.c

# Arquivo objeto
OBJS = $(SRCS:.c=.o)

# Variáveis para o nome do vídeo
VIDEO_INPUT = input.mp4
VIDEO_OUTPUT_PREFIX = output_segment_

# Regras de construção
all: install $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos temporários e binários
clean:
	rm -f $(OBJS) $(TARGET)

# Regra para rodar o programa com MPI e OpenMP
run: $(TARGET)
	mpirun -n 4 ./$(TARGET) $(VIDEO_INPUT) $(VIDEO_OUTPUT_PREFIX)

# Regras de instalação de dependências
install:
	sudo apt-get update
	sudo apt-get install -y mpich libavformat-dev libavcodec-dev libavutil-dev

# Regra padrão de limpeza completa
.PHONY: all clean run install

