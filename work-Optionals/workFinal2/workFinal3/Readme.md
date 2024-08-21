### Motivos do Projeto

Este projeto foi concebido com o objetivo principal de explorar e demonstrar a aplicação de técnicas avançadas de programação paralela e distribuída em uma tarefa prática de compressão de vídeo. A compressão de vídeo é uma operação intensiva em termos de processamento, o que a torna uma excelente candidata para otimização através de paralelismo. A combinação das bibliotecas MPI (Message Passing Interface) e OpenMP (Open Multi-Processing) permite dividir eficientemente a carga de trabalho entre múltiplos processos e threads, maximizando o uso dos recursos de hardware disponíveis.

#### Por que Focar na Compressão de Vídeo?
- **Desempenho Acelerado**: A compressão de vídeo, especialmente em resoluções altas ou vídeos de longa duração, pode ser extremamente demorada. Paralelizar essa tarefa pode reduzir significativamente o tempo de processamento, tornando-a mais prática para aplicações em tempo real ou em lotes.
- **Relevância no Mundo Real**: A compressão de vídeo é crucial em várias indústrias, incluindo streaming de mídia, armazenamento digital, e transmissão ao vivo. O aprendizado obtido a partir deste projeto é diretamente aplicável a cenários industriais e acadêmicos.
- **Foco na Paralelização**: Ao utilizar bibliotecas robustas como FFmpeg para o processamento de vídeo, o projeto permite que o foco esteja na aplicação de técnicas de paralelismo, em vez de na implementação de algoritmos complexos de compressão, simplificando o desenvolvimento e permitindo uma exploração mais profunda das técnicas de paralelismo.

### Integração de C, OpenMP e MPI

Este projeto é uma combinação de três tecnologias poderosas: C, OpenMP e MPI. Cada uma dessas tecnologias desempenha um papel essencial na execução eficiente da tarefa de compressão de vídeo.

#### Linguagem C
- **Eficiência de Baixo Nível**: C é uma linguagem conhecida por sua eficiência e controle direto sobre os recursos de hardware, o que é crucial para tarefas que exigem alto desempenho, como a compressão de vídeo.
- **Ampla Compatibilidade**: As bibliotecas de processamento de vídeo, como FFmpeg, possuem interfaces bem estabelecidas em C, facilitando sua integração com outras bibliotecas e frameworks.

#### OpenMP
- **Paralelismo em Memória Compartilhada**: OpenMP é utilizado para paralelizar tarefas dentro de um único processo, aproveitando os núcleos de CPU em um ambiente de memória compartilhada. Isso é essencial para acelerar a compressão de vídeo dentro de cada processo MPI.
- **Facilidade de Implementação**: Com diretivas pragmáticas, OpenMP permite a introdução de paralelismo em código C de forma simples e eficiente, sem a necessidade de gerenciar threads manualmente.

#### MPI (Message Passing Interface)
- **Paralelismo em Memória Distribuída**: MPI é empregado para dividir a carga de trabalho entre diferentes processos que podem estar rodando em diversas máquinas ou nós. Cada processo MPI trata de uma parte distinta do vídeo, operando de forma independente.
- **Alta Escalabilidade**: A utilização de MPI permite que o projeto escale para um grande número de nós, possibilitando a compressão eficiente de vídeos muito grandes através da distribuição da carga de trabalho.

### Funcionamento da Integração

1. **Divisão de Trabalho com MPI**:
   - O vídeo é dividido em segmentos, e cada segmento é atribuído a um processo MPI específico.
   - Cada processo MPI é responsável pela compressão de seu segmento, utilizando parâmetros específicos de compressão.

2. **Paralelização com OpenMP**:
   - Dentro de cada processo MPI, OpenMP paraleliza ainda mais a tarefa de compressão, dividindo o trabalho entre várias threads para maximizar o uso da CPU.

3. **Execução de Comandos FFmpeg**:
   - As threads de OpenMP dentro de cada processo MPI executam comandos FFmpeg para realizar a compressão, utilizando os parâmetros fornecidos e as bibliotecas associadas.

4. **Comunicação e Sincronização com MPI**:
   - MPI gerencia a comunicação entre os processos, garantindo que todos os segmentos do vídeo sejam processados corretamente e que os dados sejam sincronizados de forma eficaz.

5. **Monitoramento com Registro de Logs**:
   - Todas as operações realizadas, tanto por MPI quanto por OpenMP, são registradas em logs para facilitar o monitoramento e a análise de desempenho.

### Benefícios da Abordagem Híbrida

- **Uso Ótimo dos Recursos**: A combinação de MPI para paralelismo em memória distribuída e OpenMP para paralelismo em memória compartilhada permite o uso eficiente dos recursos disponíveis, seja em um único computador ou em um cluster de máquinas.
- **Flexibilidade e Escalabilidade**: A arquitetura híbrida permite que o projeto seja executado em uma variedade de ambientes, desde computadores pessoais de alto desempenho até clusters de computação em grande escala.
- **Facilidade de Implementação**: Utilizar FFmpeg para o processamento de vídeo libera o desenvolvedor para focar na implementação das técnicas de paralelismo, sem a necessidade de desenvolver algoritmos de compressão do zero.

Este projeto demonstra como técnicas de programação paralela e distribuída podem ser integradas de forma eficaz para resolver problemas computacionalmente intensivos de maneira eficiente e escalável.

### Requisitos para Executar o Projeto

Para compilar e executar o projeto de compressão de vídeo utilizando C, OpenMP, MPI e FFmpeg, é necessário garantir que o ambiente de desenvolvimento atenda a alguns requisitos essenciais:

#### 1. **Compilador com Suporte a MPI (mpicc)**
   - **Descrição**: `mpicc` é o compilador que permite a compilação de programas que utilizam a biblioteca MPI para comunicação entre processos em ambientes de memória distribuída.
   - **Instalação**:
     - **Ubuntu**: 
       ```bash
       sudo apt-get install mpich
       ```
     - **CentOS/RHEL**:
       ```bash
       sudo yum install openmpi openmpi-devel
       ```

#### 2. **Bibliotecas FFmpeg**
   - **Descrição**: FFmpeg é essencial para a manipulação e compressão de arquivos de vídeo. As bibliotecas `libavformat`, `libavcodec`, e `libavutil` são utilizadas neste projeto para realizar as operações de compressão.
   - **Instalação**:
     - **Ubuntu**: 
       ```bash
       sudo apt-get install libavformat-dev libavcodec-dev libavutil-dev
       ```
     - **CentOS/RHEL**:
       ```bash
       sudo yum install ffmpeg-devel
       ```

#### 3. **Compilador C com Suporte a OpenMP**
   - **Descrição**: OpenMP é usado para paralelização em memória compartilhada. O suporte ao OpenMP é geralmente fornecido pelo compilador GCC.
   - **Instalação**:
     - **Ubuntu/CentOS**: 
       ```bash
       sudo apt-get install gcc
       ```
   - **Verificação**:
     - Para verificar se o suporte ao OpenMP está habilitado, você pode compilar um código de teste:
       ```bash
       gcc -fopenmp -o test_openmp test_openmp.c
       ```

#### 4. **Biblioteca Matemática (libm)**
   - **Descrição**: `libm` é a biblioteca padrão de matemática, necessária para operações matemáticas avançadas no código. Geralmente, já está incluída com a maioria dos compiladores e distribuições Linux.

#### 5. **Sistema Operacional**
   - **Descrição**: O código foi desenvolvido e testado em um ambiente Linux. Recomenda-se o uso de distribuições como Ubuntu ou CentOS para garantir a compatibilidade e o funcionamento adequado.

#### 6. **Espaço de Armazenamento**
   - **Descrição**: Certifique-se de ter espaço de armazenamento suficiente para os arquivos de vídeo processados. A compressão de vídeos, especialmente em alta definição, pode exigir uma quantidade significativa de espaço em disco.

### Verificação dos Requisitos

Antes de compilar e executar o projeto, é aconselhável verificar a instalação correta de todos os pacotes e bibliotecas necessárias. Isso evitará problemas durante a compilação ou execução do programa.

### Compilação e Execução

1. **Compilação do Código**:
   - Para compilar o código com suporte a OpenMP, MPI e as bibliotecas FFmpeg, use o comando:
   ```bash
   mpicc -o compress_video_hybrid compress_video_hybrid.c -fopenmp -lavformat -lavcodec -lavutil -lm
   ```
   - **Explicação**:
     - `mpicc`: Compilador que suporta MPI.
     - `-o compress_video_hybrid`: Define o nome do executável gerado.
     - `compress_video_hybrid.c`: Arquivo fonte C.
     - `-fopenmp`: Ativa o suporte ao OpenMP para paralelização.
     - `-lavformat -lavcodec -lavutil`: Linka as bibliotecas FFmpeg necessárias.
     - `-lm`: Linka a biblioteca matemática `libm`.

2. **Execução do Programa**:
   - Para executar o programa utilizando 4 processos MPI, execute:
   ```bash
   mpirun -n 4 compress_video_hybrid
   ```
   - **Explicação**:
     - `mpirun`: Comando utilizado para rodar programas MPI.
     - `-n 4`: Especifica o número de processos MPI (neste caso, 4).
     - `compress_video_hybrid`: Nome do executável gerado na compilação.

Com esses comandos, o projeto estará pronto para executar a compressão de vídeo de forma eficiente, utilizando técnicas de paralelismo distribuído e memória compartilhada.
