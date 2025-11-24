use std::collections::HashMap;
use std::io;

fn main() {
    let mut phonebook: HashMap<String, String> = HashMap::new();

    loop {
        println!("\n--- 전화번호부 ---");
        println!("1. 추가  2. 검색  3. 종료");
        print!("선택: ");
        // stdout flush가 필요할 수 있으나 단순화를 위해 생략
        
        let mut choice = String::new();
        io::stdin().read_line(&mut choice).expect("입력 실패");

        match choice.trim() {
            "1" => {
                println!("이름:");
                let mut name = String::new();
                io::stdin().read_line(&mut name).unwrap();
                
                println!("전화번호:");
                let mut number = String::new();
                io::stdin().read_line(&mut number).unwrap();
                
                phonebook.insert(name.trim().to_string(), number.trim().to_string());
                println!("저장되었습니다.");
            },
            "2" => {
                println!("검색할 이름:");
                let mut search_name = String::new();
                io::stdin().read_line(&mut search_name).unwrap();
                let key = search_name.trim();
                
                match phonebook.get(key) {
                    Some(number) => println!("=> {}의 번호: {}", key, number),
                    None => println!("=> '{}' 님을 찾을 수 없습니다.", key),
                }
            },
            "3" => {
                println!("프로그램을 종료합니다.");
                break;
            },
            _ => println!("잘못된 입력입니다."),
        }
    }
}



