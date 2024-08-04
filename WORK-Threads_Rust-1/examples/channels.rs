// examples/channels.rs
use std::sync::mpsc;
use std::thread;

fn main() {
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let val = String::from("hi"); // Criação da string "hi"
        tx.send(val).unwrap(); // Envia a string através do canal
    });

    let received = rx.recv().unwrap(); // Recebe a string do canal
    println!("Got: {}", received); // Imprime a string recebida
}
