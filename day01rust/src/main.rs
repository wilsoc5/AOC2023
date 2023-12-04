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

fn convert_line(input: &str) -> Vec<(i32,usize)> {
    let words = vec!["zero","one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "0", "1", "2",
                    "3","4","5","6","7","8","9"];
    let mut loc = Vec::new();
    println!("\t\t{}",input);
    let mut wc = 0;
    for w in words {
        let mut start = 0;
        println!("\tWord: {}", w);
        loop {
            let insub = &input[start..];
            let idx = insub.find(w); 
            println!("\t\tLine: {}, idx: {:?}",insub,idx);
            if idx.is_some(){ 
                if wc < 10{
                    loc.push((wc,start+idx.unwrap()));
                } else {
                    loc.push((wc - 10,start+idx.unwrap()));
                }
                start = start+idx.unwrap() + 1;
                println!("\t\t{}",start);
            } else {
                break;
            }
        }
        wc += 1;
    }
    loc.sort_by(|(_a0,a1), (_b0,b1)| a1.cmp(&b1));
    return loc;
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
        if first_int.is_some() {
            let number = first_int.unwrap() * 10 + last_int.unwrap();  
            println!("\tNumber is {:?}", number);
            sum = sum + number as u64;
            //line_read.unwrap().clear();
        }

        let p2line = convert_line(&line);
        println!("\tp2line: {:?}", p2line);
        if p2line.first().is_some(){
            let (p2first,p2fidx) = p2line.first().unwrap();
            let (p2last, p2lidx) = p2line.last().unwrap();
            println!("\tp2first: {:?}", p2first);
            println!("\tp2last: {:?}", p2last);
            let p2number = p2first * 10 + p2last;  
            sump2 = sump2 + p2number as u64;
        }
    }

    println!("Sum: {}", sum);
    println!("Sum P2: {}", sump2);
    
    Ok(())
}
