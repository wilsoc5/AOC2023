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

fn convert_line(input: &str) {
    let words = vec!["zero","one", "two", "three", "four", "five", "six", "seven", "eight", "nine"];
    let mut loc = Vec::new();
    for w in words {
        loc.push ( input.find(w) );
    }
}

fn main() -> std::io::Result<()> {
    let args: Vec<String> = env::args().collect();
    let filename = &args[1];
    let f = File::open(filename)?;

    let reader = BufReader::new(f);
    let mut sum = 0u64;
    let mut sump2 = 0u64;
    for line_read in reader.lines() {
        let line = line_read?;
        println!("First line is {}", line=line);
        let first_int = find_first_int(&line);
        let last_int = find_last_int(&line);
        println!("\tFirst int is {:?}", first_int);
        println!("\tLast int is {:?}", last_int);
        let number = first_int.unwrap() * 10 + last_int.unwrap();  
        println!("\tNumber is {:?}", number);
        sum = sum + number as u64;
        //line_read.unwrap().clear();

        let p2line = convert_line(&line);
        println!("\tp2line: {:?}", p2line);
    }

    println!("Sum: {}", sum);
    println!("Sum P2: {}", sump2);
    
    Ok(())
}
