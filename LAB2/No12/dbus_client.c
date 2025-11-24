#include <stdio.h>
#include <systemd/sd-bus.h>

int main(int argc, char *argv[]) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message *m = NULL;
    sd_bus *bus = NULL;
    const char *reply;

    sd_bus_default_user(&bus);

    // 서버 메소드 호출
    sd_bus_call_method(bus,
        "org.example.Notify",           // 서비스명
        "/org/example/Notify",          // 객체 경로
        "org.example.Notify",           // 인터페이스
        "Notify",                       // 메소드명
        &error, &m,
        "s",                            // 인자 타입 (String)
        "긴급 점검 알림입니다!");        // 보낼 메시지

    // 응답 읽기
    sd_bus_message_read(m, "s", &reply);
    printf("[Client] 서버 응답: %s\n", reply);

    sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    sd_bus_unref(bus);
    return 0;
}
