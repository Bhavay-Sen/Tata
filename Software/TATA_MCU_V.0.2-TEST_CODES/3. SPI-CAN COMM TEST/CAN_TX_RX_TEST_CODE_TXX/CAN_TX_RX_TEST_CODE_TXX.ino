#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1; // For Data
struct can_frame canMsg2; // For Request

MCP2515 mcp2515(10);


void setup() {

  canMsg1.can_id  = 0x0F6;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 0x8E;
  canMsg1.data[1] = 0x87;
  canMsg1.data[2] = 0x32;
  canMsg1.data[3] = 0xFA;
  canMsg1.data[4] = 0x26;
  canMsg1.data[5] = 0x8E;
  canMsg1.data[6] = 0xBE;
  canMsg1.data[7] = 0x86;

  pinMode(3,OUTPUT);
  
  while (!Serial);
  Serial.begin(9600);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("Example: Write to CAN");
}

void loop() {
  if(mcp2515.readMessage(&canMsg2) == MCP2515::ERROR_OK && canMsg2.can_id == 0x0f2){
    if(canMsg2.data[0] == 0x3f){
      delay(10);
      mcp2515.sendMessage(&canMsg1);

      digitalWrite(3,HIGH);
      delay(1000);
      digitalWrite(3,LOW);
      delay(1000);  

      Serial.println("Messages sent");
    }
  } 
  delay(100);
}
