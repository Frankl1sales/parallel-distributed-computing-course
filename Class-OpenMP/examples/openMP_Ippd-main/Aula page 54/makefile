# Nome do executável
EXEC = testefirst

# Arquivos fonte
SRC = first.c

# Compilador e flags
CC = gcc
CFLAGS = -fopenmp

# Regras
.PHONY: all clean run help install

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ -lpthread

clean:
	rm -f $(EXEC) *.o

run: $(EXEC)
	./$(EXEC)

help:
	@echo "Makefile para compilar e executar um programa C com OpenMP"
	@echo
	@echo "Comandos disponíveis:"
	@echo "  make         - Compila o programa"
	@echo "  make clean   - Remove o executável e arquivos objeto"
	@echo "  make run     - Executa o programa"
	@echo "  make help    - Mostra esta mensagem de ajuda"
	@echo "  make install - Instala as dependências necessárias (GCC e OpenMP)"

install:
	@echo "Verificando dependências..."
	@if ! command -v gcc >/dev/null 2>&1; then \
		echo "GCC não encontrado. Instalando GCC..."; \
		sudo apt-get update; \
		sudo apt-get install -y gcc; \
	else \
		echo "GCC já está instalado."; \
	fi
	@if ! ldconfig -p | grep -q libomp; then \
		echo "Biblioteca OpenMP não encontrada. Instalando libomp-dev..."; \
		sudo apt-get install -y libomp-dev; \
	else \
		echo "Biblioteca OpenMP já está instalada."; \
	fi
	@echo "Dependências verificadas e instaladas (se necessário)."
