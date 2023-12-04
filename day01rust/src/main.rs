use std::io::prelude::*;
use std::io::BufReader;
use std::fs::File;
use std::env;

fn find_first_int(input: &str) -> Option<u32> {
    input.chars().find(|a| a.is_digit(10)).and_then(|a| a.to_digit(10))
}
fn find_last_int(input: &str) -> Option<u32> {
    input.chars().rfind(|a| a.is_digit(10)).and_then(|a| a.to_digit(10))
}

fn main() -> std::io::Result<()> {
    let args: Vec<String> = env::args().collect();
    let filename = &args[1];
    let f = File::open(filename)?;

    let mut reader = BufReader::new(f);
    let mut line = String::new();
    let mut sum = 0u64;
    let len = reader.read_line(&mut line)?;
    println!("First line is {len} bytes: {line}\n", len=len, line=line);
    let first_int = find_first_int(&line);
    let last_int = find_last_int(&line);
    println!("\tFirst int is {:?} \n", first_int);
    println!("\tLast int is {:?} \n", last_int);
    let number = first_int.unwrap() * 10 + last_int.unwrap();  
    println!("\tNumber is {:?} \n", number);
    sum = sum + number as u64;

    println!("\tSum: {}", sum);
    
    Ok(())
}
