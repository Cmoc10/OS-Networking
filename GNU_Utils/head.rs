use std::env;
use std::fs::File;
use std::io::{self, BufRead, BufReader};

fn print_head<R: BufRead>(reader: R, lines: usize, file_name: Option<&str>, file_num: usize) {
    // If file is the first file print out without new line otherwise go for it
    if let Some(name) = file_name {
        if file_num > 0 {
            println!("\n==> {} <==", name);
        } else {
            println!("==> {} <==", name);
        }
    }
    // get the lines from splitting the reader by newline and get an index from the .enumerate() function
    for (index, line) in reader.split(b'\n').enumerate() {
        // if the index gets to longer than the file dip
        if index >= lines {
            break;
        }
        match line {
            //if the line is good print it out
            Ok(content) => {
                if let Ok(line_str) = std::str::from_utf8(&content) {
                    // If the line is UTF-8, print it as a string
                    println!("{}", line_str);
                } else {
                    // If it's not UTF-8, print it as raw bytes
                    // the :? is a debug print which allows non utf-8 characters to be printed
                    println!("{:?}", content);
                }
            }
            //if the line is bad print out the error
            Err(e) => {
                eprintln!("Error reading line: {}", e);
                std::process::exit(1);
            }
        }
    }
}

fn process_file(file_path: &str, lines: usize, print_headers: bool, file_num: usize) {
    match File::open(file_path) {
        //if the file is good read it and print it out
        Ok(file) => {
            let reader = BufReader::new(file);
            print_head(reader, lines, if print_headers { Some(file_path) } else { None }, file_num);
        }
        //if the file is bad print out the error
        Err(_) => {
            eprintln!("head: cannot open '{}' for reading: No such file or directory", file_path);
        }
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    
    // Parse the number of lines if specified as the first argument after program name
    let (lines, files_start_index) = (10, 1);
    // Get the list of files to process
    let files: Vec<&String> = args[files_start_index..].iter().collect();
    
    // If no files specified read from stdin
    if files.is_empty() {
        let stdin = io::stdin();
        let reader = BufReader::new(stdin.lock());
        print_head(reader, lines, None, 0);
    } else {
        // Process each file
        let print_headers = files.len() > 1;
        //get each file and enumerate for filenum so print can work correctly
        for (file_num, file_path) in files.iter().enumerate() {
            process_file(file_path, lines, print_headers, file_num);
        }
    }
}
