use std::env;

fn main() {
    //try for current directory
    match env::current_dir() {
        //is good you can print
        Ok(path) => println!("{}", path.display()),
        //not food print out the error
        Err(_) => eprintln!("{} error: no such file or directory", env::current_dir().unwrap_err()),
    }
}