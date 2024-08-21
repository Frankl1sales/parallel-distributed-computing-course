# Define o compilador a ser usado
CC = gcc

# Define as flags de compilação
CFLAGS = -O2 -fopenmp  # -O2 ativa otimizações de compilação e -fopenmp habilita o suporte a OpenMP

# Define as flags de linkedição
LDFLAGS =  # Não são usadas flags adicionais para linkedição neste exemplo

# Lista os arquivos de código fonte
SRC = main.c transcoder.c

# Define os arquivos objeto correspondentes aos arquivos de código fonte
OBJ = $(SRC:.c=.o)

# Nome do executável final
EXEC = video_transcoder

# Nome do arquivo de log onde as saídas serão registradas
LOGFILE = build_log.txt

# Regra padrão para construir o executável
all: $(EXEC)

# Regra para linkar o executável
$(EXEC): $(OBJ)
	@echo "Linking $(EXEC)..." >> $(LOGFILE)  # Adiciona uma mensagem ao arquivo de log indicando o início da etapa de linkedição
	@date '+%Y-%m-%d %H:%M:%S' >> $(LOGFILE)  # Adiciona a data e hora atual ao arquivo de log
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS) >> $(LOGFILE) 2>&1  # Compila e linka os arquivos objeto para criar o executável, redirecionando a saída e erros para o arquivo de log
	@date '+%Y-%m-%d %H:%M:%S' >> $(LOGFILE)  # Adiciona a data e hora atual ao arquivo de log após a etapa de linkedição

# Regra para compilar arquivos .c em arquivos .o
%.o: %.c
	@echo "Compiling $<..." >> $(LOGFILE)  # Adiciona uma mensagem ao arquivo de log indicando o início da compilação do arquivo fonte
	@date '+%Y-%m-%d %H:%M:%S' >> $(LOGFILE)  # Adiciona a data e hora atual ao arquivo de log
	$(CC) $(CFLAGS) -c $< -o $@ >> $(LOGFILE) 2>&1  # Compila o arquivo fonte em um arquivo objeto, redirecionando a saída e erros para o arquivo de log
	@date '+%Y-%m-%d %H:%M:%S' >> $(LOGFILE)  # Adiciona a data e hora atual ao arquivo de log após a compilação do arquivo fonte

# Regra para executar o programa
run: $(EXEC)
	@echo "Running $(EXEC) with test arguments..." >> $(LOGFILE)  # Adiciona uma mensagem ao arquivo de log indicando o início da execução do programa
	@date '+%Y-%m-%d %H:%M:%S' >> $(LOGFILE)  # Adiciona a data e hora atual ao arquivo de log
	./$(EXEC) 1920x1080 1280x720 input_video.mp4 >> $(LOGFILE) 2>&1  # Executa o programa com argumentos de teste, redirecionando a saída e erros para o arquivo de log
	@date '+%Y-%m-%d %H:%M:%S' >> $(LOGFILE)  # Adiciona a data e hora atual ao arquivo de log após a execução do programa

# Regra para limpar arquivos de construção
clean:
	@echo "Cleaning up..." >> $(LOGFILE)  # Adiciona uma mensagem ao arquivo de log indicando o início da limpeza
	rm -f $(OBJ) $(EXEC) $(LOGFILE)  # Remove os arquivos objeto, o executável e o arquivo de log

