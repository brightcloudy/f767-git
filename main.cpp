#include "mbed.h"

Serial pc(SERIAL_TX, SERIAL_RX);

DigitalOut fake_switch(D7);
BusOut rgbleds(LED1, LED2, LED3);
Ticker pulseblock;

void five_press() {
  int i = 0;
  for (i = 0; i < 5; i++) {
    fake_switch = 0;
    wait_ms(100);
    fake_switch = 1;
    wait_ms(100);
  }
}

int main() {
  fake_switch = 1; // active low switch
  pulseblock.attach(&five_press, 2.0);

  while(1) {
    int i = 0;
    for (i = 0; i < 8;i++) {
      rgbleds = i;
      wait(5.0);
    }
  }
}
