```markdown
# Comparação de Programação Multithread: Java vs Rust

Este `README.md` fornece uma comparação detalhada entre os exemplos de programação multithread em Java e Rust, destacando as principais diferenças e mostrando como executar cada exemplo.

## Código Java

### MultithreadExample.java

```java
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class MultithreadExample {
    public static void main(String[] args) {
        ExecutorService executor = Executors.newFixedThreadPool(2);

        Runnable task1 = () -> {
            for (int i = 1; i <= 5; i++) {
                System.out.println("Task 1 - Iteration " + i);
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        };

        Runnable task2 = () -> {
            for (int i = 1; i <= 5; i++) {
                System.out.println("Task 2 - Iteration " + i);
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        };

        long startTime = System.nanoTime();

        executor.submit(task1);
        executor.submit(task2);

        executor.shutdown();
        try {
            executor.awaitTermination(1, TimeUnit.MINUTES);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        long endTime = System.nanoTime();
        long duration = (endTime - startTime) / 1_000_000; // convert to milliseconds

        System.out.println("All tasks completed in: " + duration + " ms.");
    }
}
```

## Código Rust

### rust_multithread_example.rs

Este Diretório contém exemplos de programação multithread em Rust, demonstrando como utilizar threads para realizar tarefas concorrentes e gerenciar dados compartilhados entre elas.


- **Cargo.toml**: Arquivo de configuração do projeto Cargo.
- **src/main.rs**: O arquivo principal do projeto (não utilizado neste exemplo específico).
- **examples/**: Diretório contendo os exemplos de código para multithreading.

## Exemplos de Multithreading

### 1. Criação de Threads (`create_threads.rs`)

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
- Thread Principal: A thread que executa o código na função main(). É a thread de início do programa.
- Thread Secundária: A thread criada com thread::spawn para executar uma tarefa paralela.
- Usa `handle.join()` para esperar que a thread secundária termine.

**Fonte:** [doc.rust-lang.org ](https://doc.rust-lang.org/book/ch16-01-threads.html)

### 2. Comunicação entre Threads usando Canais (`channels.rs`)

Este exemplo mostra como usar canais para comunicar dados entre threads.

**Código:**
```rust
use std::thread;
use std::sync::{Arc, Mutex};
use std::time::Instant;

fn main() {
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    let start = Instant::now();

    for i in 1..=2 {
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            for _ in 1..=5 {
                let mut num = counter.lock().unwrap();
                *num += 1;
                println!("Thread {} incremented counter to {}", i, *num);
                thread::sleep(std::time::Duration::from_millis(100));
            }
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    let duration = start.elapsed();

    println!("Final counter value: {}", *counter.lock().unwrap());
    println!("All tasks completed in: {:?}", duration);
}
```

## Comparação Detalhada

### 1. Gerenciamento de Memória

**Java**:

- **Garbage Collection**: Java utiliza um garbage collector que gerencia automaticamente a memória. O desenvolvedor não precisa se preocupar com a liberação de memória dos objetos não mais utilizados.
- **ExecutorService**: Uma abstração de alto nível para gerenciar um pool de threads. O `ExecutorService` cuida do ciclo de vida das threads, facilitando a execução e sincronização de tarefas.

```java
ExecutorService executor = Executors.newFixedThreadPool(2);
executor.submit(task1);
executor.submit(task2);
executor.shutdown();
```

**Rust**:

- **Sistema de Propriedade**: Rust utiliza um sistema de propriedade para gerenciamento de memória. Cada recurso tem um proprietário, e a memória é liberada automaticamente quando o proprietário sai de escopo.
- **Arc e Mutex**: `Arc` (Atomic Reference Counting) é usado para contagem de referência segura entre threads, e `Mutex` é usado para garantir exclusão mútua ao acessar dados compartilhados.

```rust
let counter = Arc::new(Mutex::new(0));
let counter = Arc::clone(&counter);
```

### 2. Criação e Sincronização de Threads

**Java**:

- **Runnable e Thread.sleep**: As tarefas são definidas usando `Runnable` e executadas pelo `ExecutorService`. `Thread.sleep` é usado para simular um atraso.
- **awaitTermination**: Espera que todas as tarefas terminem antes de continuar.

```java
Runnable task1 = () -> {
    for (int i = 1; i <= 5; i++) {
        System.out.println("Task 1 - Iteration " + i);
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
};

executor.awaitTermination(1, TimeUnit.MINUTES);
```

**Rust**:

- **thread::spawn**: Cria novas threads. `Arc` e `Mutex` são usados para compartilhar e sincronizar dados entre threads.
- **join**: Espera que todas as threads terminem antes de continuar.

```rust
let handle = thread::spawn(move || {
    for _ in 1..=5 {
        let mut num = counter.lock().unwrap();
        *num += 1;
        println!("Thread {} incremented counter to {}", i, *num);
        thread::sleep(std::time::Duration::from_millis(100));
    }
});

for handle in handles {
    handle.join().unwrap();
}
```

### Principais Diferenças

1. **Gerenciamento de Memória**:
   - **Java**: Automático com garbage collection.
   - **Rust**: Manual, mas seguro, através do sistema de propriedade e empréstimos.

2. **Controle de Threads**:
   - **Java**: Utiliza abstrações de alto nível (ExecutorService) que escondem detalhes de gerenciamento de threads.
   - **Rust**: Dá controle explícito ao desenvolvedor para criar, gerenciar e sincronizar threads, utilizando `thread::spawn`, `Arc` e `Mutex`.

3. **Segurança de Memória**:
   - **Java**: Depende do garbage collector para evitar vazamentos de memória.
   - **Rust**: Garante segurança de memória em tempo de compilação, evitando data races e deadlocks através de verificações de propriedade e sincronização explícita.

### Conclusão

A programação multithread pode ser complexa e propensa a erros em muitas linguagens, mas Rust se sobressai ao oferecer segurança de memória sem a necessidade de um garbage collector. Isso significa que você tem mais controle sobre o gerenciamento de threads e pode evitar problemas como data races e deadlocks, resultando em código mais seguro e eficiente.

## Resultados de Desempenho

### Java

```java
All tasks completed in: 520 ms.
```

### Rust

```rust
All tasks completed in: 1.001948862s
```

Embora o Rust tenha demorado mais tempo para concluir as tarefas neste exemplo específico, isso pode ser influenciado por vários fatores, incluindo o overhead de gerenciamento de threads e sincronização de dados. Em muitos casos, o Rust pode fornecer um desempenho comparável ou superior devido ao seu controle de baixo nível e otimizações específicas.

## Executando os Exemplos

### Java

```bash
javac MultithreadExample.java
java MultithreadExample
```

### Rust

```bash
cargo run --example rust_multithread_example
```



