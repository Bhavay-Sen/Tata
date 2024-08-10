#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
struct can_frame canMsg1;

MCP2515 mcp2515(10);

uint16_t  TEVO = 0;
int       AMTE = 0;
uint8_t   CHCU = 0;


void setup() {
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();

  canMsg1.can_id = 0x0F2;
  canMsg1.can_dlc = 1;
  canMsg1.data[0] = 0x28;
  
  Serial.println("                  -------      CAN Read        ----------                   ");
  Serial.println("                     AARTECH SOLONICS LIMITED\n"
                 "                     Version 1.3\n"
                 "                     2023-2024 Bhavay Sen\n"
                 "                     PROJECT : TATA-340-PP-40\n"
                 "                     340VOLTS | PULSE POWER | 40 K-J\n"
                 "                     PROTOCOL USING : CAN 2.0A\n"
                 "                     STANDARD FORMAT\n"
                 "                     REQ CAN MESSAGE ID : 0x0F2\n"
                 "                     REQ DATA[0] : 0x28\n");
}

void loop() {

/*

  if(Serial.available() > 0){
    int i = Serial.parseInt();
    if(i == 1){
      mcp2515.sendMessage(&canMsg1);
      Serial.println("                                  ---- ---- ---- ---- ---- ---- ---- ----   "
                     "                                               Request Sent                 "
                     "                                  ---- ---- ---- ---- ---- ---- ---- ----   ");
      Serial.println();
      Serial.println();
      // Serial.println(canMsg1.data[0]);
    }
  }
*/
      mcp2515.sendMessage(&canMsg1);

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK && canMsg.can_id == 0x0F6 ) {

    float terminal_Volt = 0.00,
          Ambient_temp = 0.00,
          String_Current = 0.00;
    Serial.println();
    Serial.println("                 - - - - - - - - - - - - - - - - - - - - SRT MESSAGE - - - - - - - - - - - - - - - - - - - - - -                 ");
    Serial.println();

    Serial.println();

    Serial.print("      RECEIVED CAN MESSAGE ID (HEX) :       ");
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.println(); 
    Serial.println(); 

    uint16_t shiftSample = 0x00;
    
    // Voltage Operation
    TEVO = canMsg.data[0];
    shiftSample = canMsg.data[1];
    shiftSample = shiftSample << 8;
    TEVO = shiftSample | TEVO ;
    terminal_Volt = TEVO / 10.00;
    Serial.print("|| TERMINAL VOLTAGE    :   "); Serial.print(terminal_Volt);  Serial.println("  VOLT              || ");

    int shift_signed = 0x00;
    // Charge Operation
    AMTE = canMsg.data[2];
    shift_signed = canMsg.data[3];
    shift_signed = shift_signed << 8;
    AMTE = shift_signed | AMTE;
    Ambient_temp = AMTE / 10.00;
    Serial.print("|| AMBIENT TEMPERATURE :   "); Serial.print(Ambient_temp);   Serial.println("  CELs              || ");

    CHCU = canMsg.data[4];
    String_Current = (CHCU / 10.00);
    Serial.print("|| CHARHING  CURRENT   :    "); Serial.print(String_Current); Serial.println("  AMPs              || ");

    Serial.println(" \n");
    Serial.println("                          --------- FOR INTERNAL USE ONLY -----------");

    Serial.print("CODE VERSION  : ");
    float firm_ver = (canMsg.data[5]) / 10.0 ;
    char buffer[8];
    dtostrf(firm_ver,3,1,buffer); // function to change 
    Serial.println(buffer);

    Serial.print("DEVICE_ID     : ");
    Serial.println(canMsg.data[6], DEC);
    Serial.println("\n");
    Serial.println("                 - - - - - - - - - - - - - - - - - - - - END MESSAGE - - - - - - - - - - - - - - - - - - - - - -                 ");

  delay(1000);
  }
}
