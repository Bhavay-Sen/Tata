#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;

MCP2515 mcp2515(10);

unsigned int  TEVO = 0,
              AVCH = 0;
byte CHCU = 0;
bool OTF = 0;

void setup() {
  Serial.begin(115200);

  pinMode(7 , OUTPUT);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  // Serial.println("Debug Message");

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    float terminal_Volt = 0.00,
          Available_Charge = 0.00,
          String_Current = 0.00;

    Serial.print("CAN MESSAGE ID (HEX) : ");
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.println(" "); 


    Serial.print("DLC (BYTE) : ");
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.println(" ");

    unsigned int shiftSample = 0x00;
    byte sample = 0x80;
    
    // Voltage Operation
    TEVO = canMsg.data[0];
    shiftSample = canMsg.data[1];
    shiftSample = shiftSample << 8;

    TEVO = shiftSample | TEVO ;

    terminal_Volt = TEVO / 10;
    Serial.print("TERMINAL VOLTAGE : "); Serial.print(terminal_Volt); Serial.println(" V");

    // Charge Operation
    AVCH = canMsg.data[2];
    shiftSample = canMsg.data[3];
    shiftSample = shiftSample << 8;

    AVCH = shiftSample | AVCH;

    Available_Charge = AVCH / 10;
    Serial.print("AVAILABLE CHARHE : "); Serial.print(Available_Charge); Serial.println(" C");

    CHCU = canMsg.data[4];
    sample = canMsg.data[4] & sample;
    OTF = sample >> 7;

    CHCU = CHCU & 0x7F;
    String_Current = CHCU / 10;
    Serial.print("CHARHING  CURRENT : "); Serial.print(String_Current); Serial.println(" A");

    if(OTF == 1){
      Serial.print("Temperature Status : "); Serial.println("OVER-TEMPERATURE");
    }
    else{
      Serial.print("Temperature Status : "); Serial.println("NORMAL");
    }

    Serial.println();
    digitalWrite(7, HIGH);
    delay(100); // Settling time     
    digitalWrite(7, LOW);
  }
}
