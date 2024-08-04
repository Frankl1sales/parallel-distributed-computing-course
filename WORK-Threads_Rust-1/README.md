# Proposta 
Estes projeto tem como proposito demonstrar exemplos de recursos multi-threads em Rust para fins de explorar a robustez em relação gerenciamento de memória. Também é objetivo a comparação de algoritmos multi-threads em Java e Rust.

## Estrutura do Projeto

O projeto está estruturado da seguinte forma:

```
multithread_examples/
├── Cargo.toml
├── src/
│   └── main.rs
└── examples/
    ├── create_threads.rs
    ├── channels.rs
    └── arc_mutex.rs
    └── MultithreadExample.java
    └── rust_multithread_example.rs
```

- **Cargo.toml**: Arquivo de configuração do projeto Cargo.
- **src/main.rs**: O arquivo principal do projeto (não utilizado neste exemplo específico).
- **examples/**: Diretório contendo os códigos sobre multithread em Rust, bem como a comparação dos códigos Java e Rust para programação multithread.



# Comparação de Programação Multithread: Java vs Rust

Este repositório contém exemplos práticos de programação multithread em Java e Rust, com foco em comparar o desempenho e as abordagens de gerenciamento de threads e memória em ambas as linguagens. Estes exemplos fazem parte de um vídeo requisitado na cadeira de Introdução ao Processamento Paralelo e Distribuído, ministrada pelo professor Gerson.


## Executando os Exemplos

### Rust

1. Navegue até o diretório `examples/`.
2. Compile e execute o exemplo de multithread em Rust:

```bash
cd examples
cargo run --example rust_multithread_example
```

### Java

1. Navegue até o diretório `examples/`.
2. Compile e execute o exemplo de multithread em Java:

```bash
cd java-rust
javac MultithreadExample.java
java MultithreadExample
```

## Comparação de Desempenho

### Java

```java
All tasks completed in: 520 ms.
```

### Rust

```rust
All tasks completed in: 1.001948862s
```


### Fatores que Influenciam o Tempo de Execução
Embora o Rust tenha demorado mais tempo para concluir as tarefas neste exemplo específico, é importante considerar os fatores que influenciam o desempenho, como o overhead de gerenciamento de threads e sincronização de dados. Rust oferece um controle de baixo nível e otimizações que podem resultar em desempenho superior em outros cenários.

Gerenciamento de Threads e Sistema Operacional:
- Java: Utiliza abstrações de alto nível como ExecutorService, que pode aproveitar otimizações específicas do JVM (Java Virtual Machine) para gerenciar threads de maneira eficiente. A JVM também possui um sofisticado gerenciador de threads que pode melhorar a performance em certos cenários.

- Rust: Utiliza a biblioteca padrão para criar e gerenciar threads, o que pode não ter as mesmas otimizações de alto nível que a JVM fornece. O tempo de criação e gerenciamento de threads em Rust pode ser mais dependente do sistema operacional.

Garbage Collection vs. Sistema de Propriedade:
- Java: O garbage collector pode ter um impacto no tempo de execução, mas no exemplo dado, o tempo de execução foi relativamente curto e a carga de memória não era alta o suficiente para desencadear uma coleta de lixo significativa. Isso significa que o overhead do garbage collector foi mínimo.

- Rust: Utiliza um sistema de propriedade que evita a necessidade de garbage collection, mas isso também significa que a sincronização e o bloqueio de dados compartilhados (como o uso de Mutex) pode introduzir algum overhead adicional.

Custo de Sincronização e Bloqueio:
- Java: O ExecutorService gerencia a sincronização internamente, e as operações simples dentro do Runnable não envolvem bloqueios complexos, resultando em menor overhead.
- Rust: O uso de Mutex para proteger a variável compartilhada pode introduzir latência adicional devido ao custo de bloqueio e desbloqueio, especialmente com muitas iterações.

Ambiente de Execução e Compiladores:
- Java: O JIT (Just-In-Time) compiler da JVM pode otimizar o código em tempo de execução com base no perfil de execução, potencialmente resultando em um código mais eficiente após algumas iterações.
- Rust: O código é compilado antecipadamente (AOT - Ahead-Of-Time), e enquanto o compilador LLVM de Rust realiza muitas otimizações, ele pode não ter o mesmo nível de adaptabilidade em tempo de execução que o JIT da JVM.

Custo de I/O (Entrada/Saída):
- Ambas as versões do código fazem muitas operações de I/O (impressão no console). Dependendo do ambiente e da implementação de I/O, isso pode afetar o desempenho. Operações de impressão no console podem ser mais lentas em alguns ambientes.

## Conclusão

A programação multithread pode ser complexa, mas Rust se destaca ao fornecer segurança de memória sem a necessidade de um garbage collector, resultando em código mais seguro e eficiente. Java, por outro lado, facilita a programação multithread com abstrações de alto nível como `ExecutorService`, mas depende do garbage collector para gerenciamento de memória.

**Fonte:** [medium](https://medium.com/deno-the-complete-reference/url-shortener-service-benchmarking-spring-boot-virtual-threads-vs-rust-actix-d97ecf21c5df])




#  Recursos Multithreads em Rust


## 1. Criação de Threads (`create_threads.rs`)

Este exemplo demonstra como criar e executar múltiplas threads em Rust.

**Código:**
```rust
use std::thread;

fn main() {
    let handle = thread::spawn(|| {
        for i in 1..10 {
            println!("Hello from the spawned thread! {}", i);
        }
    });

    for i in 1..5 {
        println!("Hello from the main thread! {}", i);
    }

    handle.join().unwrap();
}
```

**Descrição:**
- Cria uma thread que imprime mensagens de "Hello" com números.
- A thread principal também imprime mensagens.
- Usa `handle.join()` para esperar que a thread secundária termine.
  
**Fonte:** [doc.rust-lang.org ](https://doc.rust-lang.org/book/ch16-01-threads.html)
  
### 2. Comunicação entre Threads usando Canais (`channels.rs`)

Este exemplo mostra como usar canais para comunicar dados entre threads.

**Código:**
```rust
use std::sync::mpsc;
use std::thread;

fn main() {
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let val = String::from("hi");
        tx.send(val).unwrap();
    });

    let received = rx.recv().unwrap();
    println!("Got: {}", received);
}
```

**Descrição:**
- Cria um canal para comunicação entre threads.
- A thread secundária envia a string "hi" através do canal.
- A thread principal recebe e imprime a mensagem.

**Fonte:** [doc.rust-lang.org ](https://doc.rust-lang.org/book/ch16-02-message-passing.html)

### 3. Compartilhamento de Dados com Arc e Mutex (`arc_mutex.rs`)

Este exemplo demonstra como usar `Arc` e `Mutex` para compartilhar dados entre múltiplas threads de forma segura.

**Código:**
```rust
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    for _ in 0..10 {
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            let mut num = counter.lock().unwrap();
            *num += 1;
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    println!("Result: {}", *counter.lock().unwrap());
}
```

**Descrição:**
- Cria um contador compartilhado usando `Arc` e `Mutex`.
- 10 threads incrementam o contador.
- Usa `Arc` para compartilhar o `Mutex` entre as threads.
- Usa `Mutex` para garantir acesso exclusivo ao contador.

**Fonte:** [doc.rust-lang.org ](https://doc.rust-lang.org/book/ch16-03-shared-state.html)
  
## Como Executar os Exemplos

Para compilar e executar cada exemplo, use o comando `cargo run --example` seguido pelo nome do arquivo do exemplo (sem a extensão `.rs`).

**Exemplos:**

- **Criação de Threads:**
  ```sh
  cargo run --example create_threads
  ```

- **Comunicação entre Threads usando Canais:**
  ```sh
  cargo run --example channels
  ```

- **Compartilhamento de Dados com Arc e Mutex:**
  ```sh
  cargo run --example arc_mutex
  ```

## Requisitos

- Rust (instale-o a partir de [rust-lang.org](https://www.rust-lang.org/))
- Cargo (gerenciador de pacotes do Rust)

## Contribuições

Sinta-se à vontade para enviar pull requests ou abrir issues para sugestões e melhorias.

## Licença

Este projeto é licenciado sob a [MIT License](LICENSE.md).
