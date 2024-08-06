#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
struct can_frame canMsg_Tx; // For Request

MCP2515 mcp2515(10);


void setup() {
  Serial.begin(9600);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();

  canMsg_Tx.can_id = 0x0F2;
  canMsg_Tx.can_dlc = 1;
  canMsg_Tx.data[0] = 0x3F;
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {

  if(Serial.available() > 0){
    int i = Serial.parseInt();
    if(i == 1){
      mcp2515.sendMessage(&canMsg_Tx);
      Serial.println(" Request Sent ");
      Serial.println(canMsg_Tx.data[0]);
    }
  }

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.print(" ");
    
    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
      Serial.print(canMsg.data[i],HEX);
      Serial.print(" ");
    }

    Serial.println();      
  }
}
