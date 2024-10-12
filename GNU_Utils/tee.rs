use std::io::{self, BufRead, Write};
use std::fs::OpenOptions;
use std::env;
// this file has more comments because i was explaining parts of rust to myself as a refresher as I went along
fn main() {
    // Creating a string vector of all the args because env::args().collect() is a hassle
    let args: Vec<String> = env::args().collect();
    
    // Initialize files vector only if arguments are provided
    let mut files = Vec::new();
    
    // Try to open files if any are specified
    if args.len() > 1 {
        for filename in &args[1..] {
            //checks if file exists and error if it doesnt
            match OpenOptions::new()
                .write(true)
                .create(true)
                .truncate(true)
                .open(filename)
            {
                Ok(file) => files.push((filename.clone(), Ok(file))),
                Err(_) => {
                    eprintln!("Error: Could not open file {}.", filename);
                }
            }
        }
    }

    //stdin to be shorthand for io::stdin kinda like std:: in c++
    let stdin = io::stdin();
    //lock makes it so that no one can touch my beautiful baby stdin
    let handle = stdin.lock();
    let mut stdout = io::stdout();

    //for each line from stdin
    for line in handle.lines() {
        //match checks if the line is an error or not
        match line {
            //if line is cooking some heat let it write to the file and stdout if file is given
            Ok(content) => {
                // Write to stdout
                if let Err(e) = writeln!(stdout, "{}", content) {
                    eprintln!("Error writing to stdout: {}", e);
                    continue;
                }
                
                // Write to all successfully opened files
                for (filename, file_result) in &mut files {
                    // if the file is good assign it
                    if let Ok(file) = file_result {
                        //if there is an error writing to the file print it out
                        if let Err(e) = writeln!(file, "{}", content) {
                            eprintln!("Error writing to file '{}': {}", filename, e);
                            *file_result = Err(e);
                        }
                    }
                }
            },
            Err(e) => {
                eprintln!("Error reading from stdin: {}", e);
                continue;
            }
        }
    }
}
