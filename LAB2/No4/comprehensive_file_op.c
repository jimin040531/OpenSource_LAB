#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

int main() {
    char cwd[1024];
    const char *dir_name = "lab4_practice";
    const char *file_name = "original.txt";
    const char *hard_link_name = "hard_link.txt";
    const char *soft_link_name = "soft_link.txt";
    const char *new_name = "renamed.txt";
    
    struct stat file_info;
    int fd;

    printf("=== 파일 및 디렉토리 함수 종합 실습 ===\n\n");

    // 1. [getcwd] 현재 작업 디렉토리 확인
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("[1] 시작 디렉토리: %s\n", cwd);

    // 2. [mkdir] 디렉토리 생성
    if (mkdir(dir_name, 0755) == -1) {
        if (errno != EEXIST) perror("mkdir"); // 이미 존재하면 넘어감
    } else {
        printf("[2] 디렉토리 '%s' 생성 완료\n", dir_name);
    }

    // 3. [chdir] 디렉토리 이동
    if (chdir(dir_name) == -1) { perror("chdir"); exit(1); }
    printf("[3] '%s' 디렉토리 내부로 이동함\n", dir_name);

    // 4. [open, write] 파일 생성 및 데이터 쓰기
    fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) { perror("open"); exit(1); }
    
    char *content = "Hello, System Programming!\n";
    write(fd, content, strlen(content));
    close(fd); // [close]
    printf("[4] 파일 '%s' 생성 및 데이터 기록 완료\n", file_name);

    // 5. [stat] 파일 상태(크기, inode) 확인
    if (stat(file_name, &file_info) == -1) { perror("stat"); exit(1); }
    printf("[5] '%s' 상태 정보:\n", file_name);
    printf("    - Inode 번호: %ld\n", (long)file_info.st_ino);
    printf("    - 파일 크기: %ld bytes\n", (long)file_info.st_size);

    // 6. [link, symlink] 링크 생성
    // 이미 존재할 수 있으니 unlink 후 생성
    unlink(hard_link_name); 
    unlink(soft_link_name);

    if (link(file_name, hard_link_name) == -1) perror("link");
    if (symlink(file_name, soft_link_name) == -1) perror("symlink");
    printf("[6] 하드 링크('%s')와 심볼릭 링크('%s') 생성\n", hard_link_name, soft_link_name);

    // 7. [opendir, readdir] 디렉토리 내용 읽기
    printf("[7] 현재 디렉토리 파일 목록:\n");
    DIR *dp = opendir(".");
    struct dirent *entry;
    if (dp != NULL) {
        while ((entry = readdir(dp)) != NULL) {
            // . 과 .. 은 출력 생략
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            printf("    Found: %s (inode: %ld)\n", entry->d_name, (long)entry->d_ino);
        }
        closedir(dp);
    }

    // 8. [rename] 파일 이름 변경
    if (rename(file_name, new_name) == -1) perror("rename");
    printf("[8] 파일명 변경: '%s' -> '%s'\n", file_name, new_name);

    // 9. [access] 파일 접근성/존재 확인
    if (access(new_name, F_OK) == 0) {
        printf("[9] access 함수 확인: '%s' 파일이 정상적으로 존재합니다.\n", new_name);
    }

    // 10. [unlink] 파일 삭제
    unlink(hard_link_name);
    unlink(soft_link_name);
    unlink(new_name);
    chdir("..");
    rmdir(dir_name);
    printf("[10] 생성된 파일 및 디렉토리 정리 완료\n");

    return 0;
}
