// examples/arc_mutex.rs
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    // Cria um contador compartilhado usando Arc e Mutex
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    // Cria 10 threads
    for _ in 0..10 {
        // Clona o Arc para compartilhar com a nova thread
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            // Bloqueia o mutex para acessar e modificar o contador
            let mut num = counter.lock().unwrap();
            *num += 1; // Incrementa o valor do contador
        });
        handles.push(handle);
    }

    // Espera todas as threads terminarem
    for handle in handles {
        handle.join().unwrap();
    }

    // Imprime o valor final do contador
    println!("Result: {}", *counter.lock().unwrap());
}
