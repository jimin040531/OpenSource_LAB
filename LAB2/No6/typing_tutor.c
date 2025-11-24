#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main() {
    struct termios init_attr, new_attr;
    char ch;
    char *text = "The magic thing is that you can change it."; 
    int errcnt = 0, correct_cnt = 0;
    time_t start_time, end_time;

    int fd = fileno(stdin);
    tcgetattr(fd, &init_attr); // 기존 속성 백업

    new_attr = init_attr;
    new_attr.c_lflag &= ~ICANON; // 비정규 모드 
    new_attr.c_lflag &= ~ECHO;   // 입력 문자 화면 출력 끄기 
    new_attr.c_cc[VMIN] = 1;     // 1바이트씩 읽기
    new_attr.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
        perror("tcsetattr"); return 1;
    }

    printf("다음 문장을 입력하세요:\n%s\n", text);
    start_time = time(NULL);

    for (int i = 0; i < strlen(text); i++) {
        read(fd, &ch, 1);
        if (ch == text[i]) {
            write(1, &ch, 1); // 정타: 문자 출력
            correct_cnt++;
        } else {
            write(1, "*", 1); // 오타: * 출력
            errcnt++;
        }
    }
    
    end_time = time(NULL);
    tcsetattr(fd, TCSANOW, &init_attr); // 속성 복구
    printf("\n");

    double duration = difftime(end_time, start_time);
    double wpm = (correct_cnt / 5.0) / (duration / 60.0); 

    printf("\n--- 결과 ---\n");
    printf("시간: %.1f초, 오타: %d개\n", duration, errcnt);
    printf("분당 타자수(WPM): %.1f\n", wpm);

    return 0;
}

