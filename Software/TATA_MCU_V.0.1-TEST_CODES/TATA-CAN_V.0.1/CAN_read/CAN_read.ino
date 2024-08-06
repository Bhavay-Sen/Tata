#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);
int i = 0;

void setup() {
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() { 
  char String[16];
  // int j = 0;
  // Serial.println("------------------------------------- CHECKPOINT ---------------------------");
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.print("                    MESSAGE COUNT : ");
    Serial.println(i);
    i++;

    Serial.println("### ---- NEW MESSAGE ---- ###");
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.println(" ");
    
    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
      // Serial.print("DATA BYTE : ");
      // Serial.print(i);
      // Serial.print(" : ");
      // char t = canMsg.data[0];
      for(int i = 0 ; i < canMsg.can_dlc ; i++){
          String[j] = canMsg.data[i];
          j++;
      }
      for( j =0 ; j < 8 ; j++){
        Serial.print(String[j]);
      }
      Serial.println();
  }
  if(i > 1000)
    i = 0;
}
