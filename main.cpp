#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPSocket.h"
#include "TCPServer.h"

Serial pc(SERIAL_TX, SERIAL_RX);

InterruptIn press(USER_BUTTON);
BusOut rgbleds(LED1, LED2, LED3);

EthernetInterface eth;

void startupinit() {
  rgbleds = 0;
  pc.baud(115200);
}

Thread tcplistener;

void getconns() {
  TCPServer srv;
  TCPSocket sock;
  SocketAddress sockaddr;
  char *buffer = new char[256];
  strcpy(buffer, "test\n");

  srv.open(&eth);
  srv.bind(eth.get_ip_address(), 23);
  srv.listen(1);
  srv.accept(&sock, &sockaddr);
  char *timestr = new char[32];
  uint32_t sizread = sock.recv(timestr, 32);
  timestr[sizread] = '\0';
  set_time(atoi(timestr));
  delete[] timestr;
  sock.close();
  while (1) {
  }
}

/*USBHostMSD msd;
*FATFileSystem fs("usb");
*while (!msd.connect()) {
*  wait_ms(500);
*}
*if (fs.mount(&msd) == 0) 
*  printf("USB FS mounted.\n");
*msd.connect();
*DIR* dir = opendir("/usb/");
*struct dirent* de;
*while ((de = readdir(dir)) != NULL) {
*  printf("%s\n", &(de->d_name)[0]);
*}
*closedir(dir);*/
int main() {
  
  startupinit();
  nsapi_error_t status = eth.connect();
  while (status != NSAPI_ERROR_OK) {
    printf("Error establishing connection: %d\n", (int)status);
    printf("Retrying in 5 seconds...\n");
    wait(5.0);
    status = eth.connect();
  }
  const char *ip = eth.get_ip_address();
  printf("IP address is: %s\n", ip ? ip : "No IP");

  tcplistener.start(getconns);
  while(1) {
    rgbleds[0].write(1);
    wait_ms(500);
    rgbleds[0].write(0);
    wait_ms(500);
  }
}
