#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

struct msgbuf {
    long mtype;
    char mtext[100];
};

int msqid;
long my_type, target_type;

// 수신 전담 쓰레드
void *recv_func(void *arg) {
    struct msgbuf recv_msg;
    while(1) {
        // 내 타입(my_type)으로 온 메시지만 수신
        if (msgrcv(msqid, &recv_msg, sizeof(recv_msg.mtext), my_type, 0) != -1) {
            printf("\r[받음] %s\n입력: ", recv_msg.mtext);
            fflush(stdout);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("사용법: %s <1 또는 2>\n(1: A사용자, 2: B사용자)\n", argv[0]);
        exit(1);
    }

    int id = atoi(argv[1]);
    if (id == 1) { my_type = 1; target_type = 2; }
    else { my_type = 2; target_type = 1; }

    key_t key = 12345; // 고정 키 사용
    // 메시지 큐 생성 또는 가져오기
    if ((msqid = msgget(key, IPC_CREAT | 0666)) == -1) {
        perror("msgget"); exit(1);
    }

    pthread_t tid;
    pthread_create(&tid, NULL, recv_func, NULL); // 수신 쓰레드 시작

    struct msgbuf send_msg;
    send_msg.mtype = target_type; // 상대방 타입으로 설정

    printf("채팅 시작 (내 ID: %d, 상대 ID: %ld)\n입력: ", id, target_type);
    while(fgets(send_msg.mtext, sizeof(send_msg.mtext), stdin) != NULL) {
        // 개행 문자 제거
        send_msg.mtext[strcspn(send_msg.mtext, "\n")] = 0;
        msgsnd(msqid, &send_msg, strlen(send_msg.mtext)+1, 0);
        printf("입력: ");
    }

    // 종료 시 큐 삭제는 별도 관리 필요 (여기선 생략)
    return 0;
}





