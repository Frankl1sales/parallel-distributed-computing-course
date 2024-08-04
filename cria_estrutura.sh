#!/bin/bash

# Navegar até a pasta do curso
cd ~/temp/UFPEL/AULAS/parallel-distributed-computing-course

# Criar os diretórios principais
mkdir -p docs/lecture_notes docs/assignments

# Criar diretórios para cada aula
for i in {1..3}; do
    mkdir -p aula$i/examples aula$i/assignment
done

# Adicionar um README.md em cada diretório de aula e assignment
for i in {1..3}; do
    echo "# Aula $i: Descrição da Aula" > aula$i/README.md
    echo "# Assignment da Aula $i" > aula$i/assignment/README.md
done

# Adicionar README.md para examples em cada diretório de aula
for i in {1..3}; do
    echo "# Exemplos da Aula $i" > aula$i/examples/README.md
done

# Mensagem de confirmação
echo "Estrutura de diretórios criada com sucesso!"
