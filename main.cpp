#include "mbed.h"
#include "bbcar.h"
#include <cmath>
#include <cstdio>
#include "math.h"

DigitalOut led1(LED1);
Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);

DigitalInOut pin8(D8);

BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

int main() {
    parallax_ping  ping1(pin8);
    float data[128];
    int index=1;
    data[0] = ping1.ping_cm();
    data[1] = ping1.ping_cm();
    float d1;
    float d2;
    float d;
    int a1;
    int a2;
    float a;
    ThisThread::sleep_for(1s);
    car.turn(60, 1);
    ThisThread::sleep_for(50ms);
    while (data[(index - 1)%128] - data[index%128] < 5) {
        index++;
        data[index%128] = ping1.ping_cm();
        printf("1 %d: %f\n", index%128 , data[index%128]);
        ThisThread::sleep_for(1ms);
    }
    car.stop();
    ThisThread::sleep_for(500ms);
    d1 = ping1.ping_cm() - 9;
    a1 = car.servo0.angle;

    car.turn(50, -1);
    ThisThread::sleep_for(500ms);
    index = 1;
    data[0] = ping1.ping_cm();
    data[1] = ping1.ping_cm();
    while (data[(index - 1)%128] - data[index%128] < 5) {
        index++;
        data[index%128] = ping1.ping_cm();
        printf("2 %d: %f\n", index%128 , data[index%128]);
        ThisThread::sleep_for(1ms);
    }
    car.stop();
    ThisThread::sleep_for(500ms);
    d2 = ping1.ping_cm() - 9;
    a2 = car.servo0.angle;
    a = abs(a1 - a2);
    d = sqrtf(d1 * d1 + d2 * d2 - 2 * d1 * d2 * cos((a*0.5)/180*3.14));
    printf("d1: %f, d2: %f\n", d1, d2);
    printf("a: %d\n", a);
    printf("d: %f\n", d);
}

