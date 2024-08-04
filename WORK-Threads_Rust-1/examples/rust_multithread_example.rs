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
