#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
// struct can_frame canMsg2;
MCP2515 mcp2515(10);
int i = 0;
char String[] = "Hello World";

void setup() {
  while (!Serial);
  Serial.begin(115200);


  canMsg1.can_id = 0x026;
  canMsg1.can_dlc = 8;

  /*
  Serial.println("#############################");
  Serial.println(sizeof(String) / sizeof(String[0]));
  Serial.println("#############################");

  canMsg1.data[0] = 'h';
  canMsg1.data[1] = 48;
  canMsg1.data[2] = 39;
  canMsg1.data[3] = 60;
  canMsg1.data[4] = 84;
  canMsg1.data[5] = 96;
  canMsg1.data[6] = 26;
  canMsg1.data[7] = 12;
/*
  canMsg2.can_id  = 0x036;
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x0E;
  canMsg2.data[1] = 0x00;
  canMsg2.data[2] = 0x00;
  canMsg2.data[3] = 0x08;
  canMsg2.data[4] = 0x01;
  canMsg2.data[5] = 0x00;
  canMsg2.data[6] = 0x00;
  canMsg2.data[7] = 0xA0;
  */

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  Serial.println("Example: Write to CAN");
}

void loop() {
  for (int i = 0; i < sizeof(String) / sizeof(String[0]);) {
    for (int j = 0; j < 8; j++) {
      canMsg1.data[j] = String[i];
      i++;
    }
    for (int i = 0; i < 8; i++) {
      Serial.print((char)canMsg1.data[i]);
    }
    Serial.println(" ");
    mcp2515.sendMessage(&canMsg1);
  }
  // mcp2515.sendMessage(&canMsg2);
  i++;
  Serial.println("Messages sent");
  if (i > 1000)
    i = 0;
  Serial.print("LOOP COUNT : ");
  Serial.print(i);
  Serial.println();
  // delay(100);
}
