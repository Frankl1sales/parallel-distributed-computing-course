# Processamento de Vídeo com FFmpeg

Este projeto demonstra como usar a biblioteca FFmpeg para ler e processar arquivos de vídeo em C. O código fornece um exemplo básico de como abrir um arquivo de vídeo, encontrar o stream de vídeo, decodificar os frames e processar ou exibir informações sobre esses frames.

## Funcionalidade do Código

O código realiza as seguintes operações:

1. **Abrir o Arquivo de Vídeo**:
   - Usa `avformat_open_input()` para abrir o arquivo de vídeo especificado na linha de comando.

2. **Encontrar o Stream de Vídeo**:
   - Utiliza `avformat_find_stream_info()` para buscar informações sobre o stream de vídeo.
   - Percorre os streams para localizar o stream de vídeo (`AVMEDIA_TYPE_VIDEO`).

3. **Preparar o Codec de Vídeo**:
   - Obtém informações do codec do stream de vídeo e usa `avcodec_find_decoder()` para encontrar o decoder apropriado.
   - Aloca e configura um contexto de codec com `avcodec_alloc_context3()` e `avcodec_parameters_to_context()`.
   - Abre o codec com `avcodec_open2()`.

4. **Processar os Frames de Vídeo**:
   - Lê pacotes de dados do arquivo com `av_read_frame()`.
   - Envia pacotes para o codec com `avcodec_send_packet()` e recupera os frames com `avcodec_receive_frame()`.
   - Imprime informações sobre cada frame, como o número do frame, tipo do frame e tamanho do pacote.

5. **Liberar Recursos**:
   - Libera a memória alocada e fecha o arquivo de vídeo ao final.

## Requisitos

- FFmpeg deve estar instalado no sistema.
- As bibliotecas de desenvolvimento do FFmpeg devem estar instaladas (`libavcodec-dev`, `libavformat-dev`, `libavutil-dev`, `libswscale-dev`, `libswresample-dev`).

## Compilando o Código

Para compilar o código, use o comando `gcc` com as bibliotecas FFmpeg. No terminal, execute:

```bash
gcc -o processamento_imagem processamento_imagem.c -lavformat -lavcodec -lavutil -lswscale -lswresample
```

Este comando compila o código fonte `processamento_imagem.c` e gera um executável chamado `processamento_imagem`.

## Executando o Código

Para executar o programa, forneça o caminho para o arquivo de vídeo como argumento. No terminal, execute:

```bash
./processamento_imagem /caminho/para/seu/video.mp4
```

Substitua `/caminho/para/seu/video.mp4` pelo caminho real para o arquivo de vídeo que você deseja processar.

## Observações

- O código usa funções que estão marcadas como depreciadas, como `av_register_all()`. Isso pode gerar avisos durante a compilação, mas não impede a execução.
- Certifique-se de que todas as bibliotecas necessárias estejam corretamente instaladas e disponíveis no seu sistema.

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).

```

### Como usar

1. **Instale as dependências necessárias**:

   ```bash
   sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libswresample-dev
   ```

2. **Compile o código**:

   ```bash
   gcc -o processamento_imagem processamento_imagem.c -lavformat -lavcodec -lavutil -lswscale -lswresample
   ```

3. **Execute o programa**:

   ```bash
   ./processamento_imagem /caminho/para/seu/video.mp4
   ```

Certifique-se de substituir `/caminho/para/seu/video.mp4` pelo caminho real do arquivo de vídeo que você deseja processar.
