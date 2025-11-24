#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// 읽기 전담 쓰레드 함수
void *read_thread(void *arg) {
    int pipe_fd = *(int *)arg;
    char buf[256];
    int n;
    while ((n = read(pipe_fd, buf, 255)) > 0) {
        buf[n] = '\0';
        printf("[상대방] %s", buf);
    }
    return NULL;
}

int main() {
    int p1[2], p2[2]; // p1: 부모->자식, p2: 자식->부모
    pthread_t tid;
    char buf[256];

    if (pipe(p1) == -1 || pipe(p2) == -1) exit(1);

    if (fork() == 0) { // 자식 프로세스
        close(p1[1]); close(p2[0]); // 안 쓰는 파이프 끝 닫기
        
        // 수신용 쓰레드 생성 (부모가 보낸 p1[0]을 읽음)
        pthread_create(&tid, NULL, read_thread, &p1[0]);

        // 메인 쓰레드는 송신 담당 (키보드 -> p2[1])
        while (fgets(buf, sizeof(buf), stdin)) {
            write(p2[1], buf, strlen(buf));
        }
    } else { // 부모 프로세스
        close(p1[0]); close(p2[1]);

        // 수신용 쓰레드 생성 (자식이 보낸 p2[0]을 읽음)
        pthread_create(&tid, NULL, read_thread, &p2[0]);

        // 메인 쓰레드는 송신 담당 (키보드 -> p1[1])
        while (fgets(buf, sizeof(buf), stdin)) {
            write(p1[1], buf, strlen(buf));
        }
    }
    return 0;
}


