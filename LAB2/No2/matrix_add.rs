use std::io;

fn main() {
    println!("행렬의 크기(행 열)를 입력하세요:");
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("입력 실패");
    
    let dims: Vec<usize> = input
        .trim()
        .split_whitespace()
        .map(|x| x.parse().expect("숫자가 아닙니다"))
        .collect();

    if dims.len() < 2 {
        println!("행과 열 크기를 모두 입력해야 합니다.");
        return;
    }
    let (rows, cols) = (dims[0], dims[1]);

    println!("======================================");
    println!("첫 번째 행렬 입력:");
    let mat1 = read_matrix(rows, cols);
    
    println!("======================================");
    println!("두 번째 행렬 입력:");
    let mat2 = read_matrix(rows, cols);

    // 결과 행렬 계산
    let mut result = vec![vec![0; cols]; rows];
    for i in 0..rows {
        for j in 0..cols {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }

    println!("======================================");
    println!("결과 행렬:");
    for row in result {
        println!("{:?}", row);
    }
}

fn read_matrix(rows: usize, cols: usize) -> Vec<Vec<i32>> {
    let mut matrix = Vec::new();
    for i in 0..rows {
        println!("{}행의 {}개 숫자를 공백으로 구분하여 입력:", i + 1, cols);
        let mut row_input = String::new();
        io::stdin().read_line(&mut row_input).expect("입력 실패");
        
        let row: Vec<i32> = row_input
            .trim()
            .split_whitespace()
            .map(|x| x.parse().expect("숫자여야 합니다"))
            .collect();
            
        if row.len() != cols {
            panic!("입력된 열의 개수가 설정된 크기와 다릅니다.");
        }
        matrix.push(row);
    }
    matrix
}

