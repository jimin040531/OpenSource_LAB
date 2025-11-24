#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int p[2];
    char buf[256];
    
    if (pipe(p) == -1) { perror("pipe fail"); exit(1); }

    if (fork() == 0) { // 자식: 쓰기
        close(p[0]);
        char *msg = "Message from Child Process!";
        write(p[1], msg, strlen(msg)+1);
        close(p[1]);
    } else { // 부모: 읽기
        close(p[1]);
        read(p[0], buf, sizeof(buf));
        printf("[Parent] 받은 메시지: %s\n", buf);
        close(p[0]);
    }
    return 0;
}


