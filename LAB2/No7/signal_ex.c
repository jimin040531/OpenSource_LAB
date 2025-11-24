#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

void my_handler(int signo) {
    printf("[Parent] 시그널 %d 수신함! (자식이 보냄)\n", signo);
}

int main() {
    pid_t pid;
    signal(SIGUSR1, my_handler); // 핸들러 등록

    pid = fork();
    if (pid == 0) { // 자식
        printf("[Child] 1초 후 부모에게 시그널 전송 예정...\n");
        sleep(1);
        kill(getppid(), SIGUSR1); // 부모에게 SIGUSR1 전송
        exit(0);
    } else { // 부모
        printf("[Parent] 자식 기다리는 중...\n");
        pause(); // 시그널 대기
        wait(NULL); // 자식 프로세스 종료 회수 (좀비 방지)
        printf("[Parent] 종료\n");
    }
    return 0;
}


