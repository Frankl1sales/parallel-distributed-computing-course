# Projeto OpenMP - Cálculo em Matrizes

Este projeto demonstra o uso do OpenMP para paralelizar operações em uma matriz de números aleatórios. Ele calcula a soma, a média, a variância, o desvio padrão e a esparsidade da matriz.

## Índice

- [Objetivo](#objetivo)
- [Instalação do OpenMP](#instalação-do-openmp)
- [Como Usar](#como-usar)
- [Licença](#licença)

## Objetivo

O objetivo deste projeto é mostrar como utilizar OpenMP para paralelizar operações matemáticas em uma matriz de inteiros, aproveitando os recursos de múltiplos núcleos de processamento.

## Instalação do OpenMP

Para compilar e executar programas que utilizam OpenMP, você precisa ter um compilador que suporte OpenMP. Aqui estão as instruções para instalar o GCC, que é um compilador que suporta OpenMP, no Ubuntu:

### Passo a Passo para Instalação no Ubuntu

1. **Atualize a lista de pacotes:**

   ```sh
   sudo apt update
   ```

2. **Instale o GCC:**

   ```sh
   sudo apt install gcc
   ```

3. **Verifique a instalação do GCC:**

   ```sh
   gcc --version
   ```

Se você já possui o GCC instalado, ele já deve suportar OpenMP. A versão do GCC deve ser 4.2 ou superior para suportar OpenMP.

## Como Usar

### Compilar o Código

Para compilar o código, use o seguinte comando:

```sh
gcc -fopenmp trabalho_openMP_Franklin.c -lm -o trabalho
```

#### Explicação sobre a flag `-lm`

A biblioteca `math.h` não faz parte da biblioteca padrão do C, portanto, é necessário linká-la. Veja mais detalhes no [Stack Overflow](https://stackoverflow.com/questions/44175151/what-is-the-meaning-of-lm-in-gcc).

### Executar o Programa

Para executar o programa, use o comando:

```sh
./trabalho
```

### Funcionamento do Código

1. **Inicialização da Matriz**: A matriz `a` é inicializada com valores aleatórios entre 0 e 99.
2. **Cálculo da Soma e Contagem de Zeros**: Utilizando a cláusula `reduction` do OpenMP, o código paraleliza a soma dos elementos e a contagem de zeros na matriz.
3. **Cálculo da Média**: A média dos valores da matriz é calculada.
4. **Cálculo da Variância**: Novamente utilizando a cláusula `reduction`, o código paraleliza o cálculo da variância.
5. **Cálculo do Desvio Padrão**: O desvio padrão é calculado a partir da variância.
6. **Cálculo da Esparsidade**: A esparsidade da matriz é determinada pela razão de elementos zero.
7. **Determinação da Esparsidade**: A matriz é considerada esparsa se mais de 50% dos seus elementos forem zeros.

### Exemplo de Saída

```plaintext
Thread 0 está processando a iteração 0-0 - valor: 73
Thread 1 está processando a iteração 0-1 - valor: 45
...
A Media:: 49.484900
O desvio padrao: 28.648705
A matriz tem 100 zeros de 10000 elementos.
esparsividade: 0.010000
A matriz não é esparsa.
```

## Licença

Este projeto está licenciado sob os termos da licença MIT. Veja o arquivo [LICENSE](LICENSE.md) para mais detalhes.
