# Comparação de Programação Multithread: Java vs Rust

Este repositório contém exemplos de programação multithread em Java e Rust, ilustrando como ambas as linguagens lidam com a sincronização de threads e gerenciamento de memória.

## Descrição

A programação multithread pode ser complexa e propensa a erros em muitas linguagens, mas Rust se sobressai ao oferecer segurança de memória sem a necessidade de um garbage collector. Isso significa que você tem mais controle sobre o gerenciamento de threads e pode evitar problemas como data races e deadlocks.

## Exemplos

### Exemplo em Java

Em Java, utilizamos `ExecutorService` para gerenciar threads e `Runnable` para definir as tarefas. Java conta com um garbage collector para gerenciar automaticamente a memória.

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

        executor.submit(task1);
        executor.submit(task2);

        executor.shutdown();
        try {
            executor.awaitTermination(1, TimeUnit.MINUTES);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        System.out.println("All tasks completed.");
    }
}
```

### Exemplo em Rust

Em Rust, utilizamos `thread::spawn` para criar threads e `Arc` (Atomic Reference Counting) junto com `Mutex` para sincronizar dados compartilhados entre threads. Rust não utiliza garbage collection; em vez disso, gerencia a memória através do sistema de propriedade.

```rust
use std::thread;
use std::sync::{Arc, Mutex};

fn main() {
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

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

    println!("Final counter value: {}", *counter.lock().unwrap());
}
```

## Comparação Detalhada

### Gerenciamento de Memória

**Java:**
- Utiliza garbage collection para gerenciamento automático da memória.
- Abstrações de alto nível (`ExecutorService`) facilitam a execução e sincronização de tarefas.

**Rust:**
- Gerenciamento de memória feito através do sistema de propriedade.
- `Arc` e `Mutex` são usados para compartilhamento e sincronização de dados entre threads.

### Criação e Sincronização de Threads

**Java:**
- Utiliza `Runnable` e `Thread.sleep` para definir e executar tarefas.
- `awaitTermination` para esperar que todas as tarefas terminem.

**Rust:**
- Utiliza `thread::spawn` para criar threads.
- `Arc` e `Mutex` garantem exclusão mútua e sincronização segura.
- `join` para esperar que todas as threads terminem.

### Segurança de Memória

**Java:**
- Depende do garbage collector para evitar vazamentos de memória.

**Rust:**
- Garante segurança de memória em tempo de compilação, evitando data races e deadlocks.

## Conclusão

Rust se destaca na programação multithread por proporcionar segurança de memória e controle explícito sobre a sincronização, sem a necessidade de um garbage collector. Isso permite que os desenvolvedores escrevam código seguro e eficiente, evitando problemas comuns em ambientes multithread, como data races e deadlocks.

## Como Executar

### Java

1. Compile o código Java:
    ```sh
    javac MultithreadExample.java
    ```

2. Execute o código Java:
    ```sh
    java MultithreadExample
    ```

### Rust

1. Compile o código Rust:
    ```sh
    cargo build --example rust_multithread_example
    ```

2. Execute o código Rust:
    ```sh
    cargo run --example rust_multithread_example
    ```

## Contato

Para mais informações, entre em contato através do [seu email] ou visite nosso [GitHub](https://github.com/seu_usuario).

---

Esperamos que esses exemplos ajudem a entender como Rust e Java abordam a programação multithread e gerenciamento de memória de maneiras diferentes.
```

Este `README.md` fornece uma descrição clara e comparativa entre os dois exemplos de código, destacando as diferenças na abordagem de multithreading e gerenciamento de memória em Java e Rust.
