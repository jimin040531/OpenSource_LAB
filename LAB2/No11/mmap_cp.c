#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) { printf("사용법: %s <원본> <사본>\n", argv[0]); exit(1); }

    int fdin = open(argv[1], O_RDONLY);
    if (fdin < 0) { perror("open src"); exit(1); }

    // 원본 파일 크기 확인
    struct stat sb;
    if (fstat(fdin, &sb) < 0) { perror("fstat"); exit(1); }

    // 사본 파일 열기 및 크기 지정
    int fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fdout < 0) { perror("open dst"); exit(1); }
    if (ftruncate(fdout, sb.st_size) < 0) { perror("ftruncate"); exit(1); }

    // 메모리 매핑
    char *src = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fdin, 0);
    char *dst = mmap(0, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0);

    if (src == MAP_FAILED || dst == MAP_FAILED) { perror("mmap"); exit(1); }

    // 메모리 복사 (== 파일 복사)
    memcpy(dst, src, sb.st_size);

    // 해제
    munmap(src, sb.st_size);
    munmap(dst, sb.st_size);
    close(fdin); close(fdout);
    
    printf("복사 완료 (%ld bytes)\n", sb.st_size);
    return 0;
}
