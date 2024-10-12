use std::env;
use std::fs::File;
use std::io::{self, BufRead, BufReader};

fn count_stats<R: BufRead>(mut reader: R) -> io::Result<(usize, usize, usize)> {
    let mut lines = 0;
    let mut words = 0;
    let mut bytes = 0;
    //buffer in non string
    let mut buffer = Vec::new(); // Buffer to hold each line in bytes
    //read until newline and add to buffer if there is an error reading go to error and skip if no lines
    while reader.read_until(b'\n', &mut buffer)? != 0 {
        lines += 1;
        // Split the buffer by whitespace and count the number of words
        words += buffer.split(|&b| b.is_ascii_whitespace()).filter(|slice| !slice.is_empty()).count();
        bytes += buffer.len(); // Count the bytes, including the newline character
        buffer.clear(); // Clear buffer for the next line
    }

    Ok((lines, words, bytes))
}

fn main() -> io::Result<()> {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        // Read from stdin
        let stdin = io::stdin();
        let reader = BufReader::new(stdin.lock());
        let (lines, words, bytes) = count_stats(reader)?;
        //print out the stats
        println!("{} {} {} (stdin)", lines, words, bytes);
    } else {
        //set totals
        let mut total_lines = 0;
        let mut total_words = 0;
        let mut total_bytes = 0;
        // for filename in args from 1- end
        for filename in &args[1..] {
            match File::open(filename) {
                Ok(file) => {
                    let reader = BufReader::new(file);
                    let (lines, words, bytes) = count_stats(reader)?;
                    println!("{} {} {} {}", lines, words, bytes, filename);
                    total_lines += lines;
                    total_words += words;
                    total_bytes += bytes;
                }
                Err(_) => {
                    // this is me trying to get the error right
                    // Store the current directory and display it with error for if curdir is here
                    let current_path = env::current_dir()?;
                    eprintln!("{}/wc: {}: No such file or directory", current_path.display(), filename);
                }
            }
        }            

        // Print totals if more than one file was processed
        if args.len() > 2 {
            println!("{} {} {} total", total_lines, total_words, total_bytes);
        }
    }
    Ok(())
}
 