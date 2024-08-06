#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
struct can_frame canMsg1;

MCP2515 mcp2515(10);

unsigned int  TEVO = 0,
              AVCH = 0;
byte CHCU = 0;
bool OTF = 0;


void setup() {
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();

  canMsg1.can_id = 0x0F2;
  canMsg1.can_dlc = 1;
  canMsg1.data[0] = 0x3F;
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  // Serial.println("Debug Message");
  // delay(1000);
  // Serial.print("Print");
/*
  if(Serial.available() > 0){
    int i = Serial.parseInt();
    if(i == 1){
      mcp2515.sendMessage(&canMsg1);
      Serial.println(" Request Sent ");
      Serial.println(canMsg1.data[0]);
    }
  }
*/

  mcp2515.sendMessage(&canMsg1);

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK && canMsg.can_id == 0x0F6 ) {

    // for(int cps = 0 ; cps < 8 ; cps++){
    //   Serial.print("BYTE "); Serial.print(cps); Serial.print(" ");
    //   Serial.println(canMsg.data[cps], HEX);
    // }

    float terminal_Volt = 0.00,
          Available_Charge = 0.00,
          String_Current = 0.00;
Serial.println();
Serial.println("############################################################");
Serial.println();
    Serial.print("CAN MESSAGE ID (HEX) : ");
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.println(" "); 


    // Serial.print("DLC (BYTE) : ");
    // Serial.print(canMsg.can_dlc, HEX); // print DLC
    // Serial.println(" ");

    unsigned int shiftSample = 0x00;
    byte sample = 0x80;
    
    // Voltage Operation
    TEVO = canMsg.data[0];
    shiftSample = canMsg.data[1];
    shiftSample = shiftSample << 8;

    TEVO = shiftSample | TEVO ;
    // Serial.print("TEVO (in HEX0): "); Serial.println(TEVO, HEX);

    terminal_Volt = TEVO / 10.00;
    Serial.print("TERMINAL VOLTAGE : "); Serial.print(terminal_Volt); Serial.println(" V");

    // Charge Operation
    AVCH = canMsg.data[2];
    shiftSample = canMsg.data[3];
    shiftSample = shiftSample << 8;

    AVCH = shiftSample | AVCH;

    Available_Charge = AVCH / 10.00;
    Serial.print("AVAILABLE CHARHE : "); Serial.print(Available_Charge); Serial.println(" C");

    CHCU = canMsg.data[4];
    sample = canMsg.data[4] & sample;
    OTF = sample >> 7;

    CHCU = CHCU & 0x7F;  // 7F = 0111 1111
    String_Current = (CHCU / 10.00);
    Serial.print("CHARHING  CURRENT : "); Serial.print(String_Current); Serial.println(" A");

    if(OTF == 1){
      Serial.print("Temperature Status : "); Serial.println("OVER-TEMPERATURE");
    }
    else{
      Serial.print("Temperature Status : "); Serial.println("NORMAL");
    }
  }

  delay(500);
}
