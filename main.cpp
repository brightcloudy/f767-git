#include "mbed.h"

Serial pc(SERIAL_TX, SERIAL_RX);

int main() {
  uint32_t count = 0;
  while (1) {
    pc.printf("im lonely %d\n", count);
    count++;
    wait(1.0);
  }
}
