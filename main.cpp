#include "mbed.h"

Serial pc(SERIAL_TX, SERIAL_RX);

BusOut rgbleds(LED1, LED2, LED3);
PwmOut pwmout(D9);

int main() {

  pwmout.period_us(50);
  uint32_t i;
  while(1) {
    //pc.printf("%02d\n", slope);
    rgbleds = rgbleds + 1;
    for (i = 0;i < 100;i++) {
      pwmout = i/100.0f;
      wait_us(10);
    }
    for (i = 100;i > 0;i--) {
      pwmout = i/100.0f;
      wait_us(10);
    }
  }
}
