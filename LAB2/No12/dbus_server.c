#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>

// 메소드 콜백 함수
static int method_notify(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    const char *msg;
    sd_bus_message_read(m, "s", &msg); // 문자열 인자 읽기
    printf("[Server] 클라이언트 메시지: %s\n", msg);
    return sd_bus_reply_method_return(m, "s", "알림 수신 완료!"); // 응답
}

// VTable 정의
static const sd_bus_vtable notify_vtable[] = {
    SD_BUS_VTABLE_START(0),
    SD_BUS_METHOD("Notify", "s", "s", method_notify, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_VTABLE_END
};

int main(int argc, char *argv[]) {
    sd_bus_slot *slot = NULL;
    sd_bus *bus = NULL;

    // 세션 버스 연결
    sd_bus_default_user(&bus);
    
    // 객체 등록 (/org/example/Notify 경로에 인터페이스 등록)
    sd_bus_add_object_vtable(bus, &slot, 
        "/org/example/Notify", "org.example.Notify", notify_vtable, NULL);

    // 서비스 이름 요청
    sd_bus_request_name(bus, "org.example.Notify", 0);

    printf("[Server] DBus 서비스 실행 중...\n");
    for (;;) {
        sd_bus_process(bus, NULL);
        sd_bus_wait(bus, (uint64_t)-1);
    }
    
    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return 0;
}



