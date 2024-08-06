/*

# Author  : Bhavay Sen
# Date    : 15/07/2023
# Project : TATA 340 PP 40

Feature covered : Current sensing 
                : Voltage sensing 
                : Available Charge 
                : CAN communication with single message transmission : Message ID : 0x0F6
*/

#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg; // To check weather the request is made by Rx or not.
struct can_frame canMsg1; // Structure for can msg

MCP2515 mcp2515(10);      // Slave select pin for MCP2515 SPI Comm.

// Global Variable Declaration 
unsigned int  TEVO = 0,
              AVCH = 0;
byte CHCU = 0;

// bool OTF = 0;  // for Over-Temperature Fault, it will be placed at LSB of CHCU byte. 0000 000"0" - OTF

/*

*/



void setup() {

  pinMode(7, OUTPUT); // LED D5 for Indication of message transmission

  canMsg1.can_id  = 0x0F6;  // CAN MSG ID : 11-bit identifier - 0000 1111 0110
  canMsg1.can_dlc = 8;      // Data length parameter for payload

  while (!Serial);
  Serial.begin(115200); // UART initialization at 115200 baud rate

  mcp2515.reset();                      // Resetting MCP2515 before configuration
  mcp2515.setBitrate(CAN_125KBPS);      // Setting CAN Speed
  mcp2515.setNormalMode();              // NORMAL MODE | LOOPBACK MODE | LISTEN-ONLY MODE
  Serial.println( "AARTECH SOLONICS LIMITED - YEAR 2023 / 24 \n"
                  "Author : Bhavay Sen \n"
                  "Code Version : V.0.2 \n"
                  "Date : 13-07-2023 \n"
                  "Ultra-Capacitor Department \n");                  // Code Details
  
}

void loop() {

  if(mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK && canMsg.can_id == 0x0F2){ // ERROR_OK = 0; 
    if(canMsg.data[0] == 0x3F){
        Serial.println("-------------Time Stamp_initial------------------");

        // Local Variable Declaration 
        
        // Current Sensor
        float  AcsValue  =  0.0,
                Samples   =  0.0,
                AvgAcs    =  0.0,
                AcsValueF =  0.0;

        // Voltage sensor                  
        float  B1_raw  = 0.00 ,
                B2_raw  = 0.00 ,
                B1_Samples = 0.00,
                B2_Samples = 0.00,
                B1_read = 0.00 , 
                B2_read = 0.00 , 
                B1_volt = 0.00 , 
                B2_volt = 0.00 ,
                Available_Charge = 0.00;

        // Taking Output Terminl Voltage samples
        // for(unsigned char i = 0 ; i < 150 ; i++){
          B1_raw = analogRead(A0);                    // A0 - Voltage 
          // B1_Samples = B1_Samples + B1_raw;
          // delay(3); // ADC Settling time for maximum optimization 
        // }

        // B1_raw = B1_Samples / 150.0 ;
        B1_read = ( B1_raw * 4.98 ) / 1023.0 ;           // NOTE : Recheck point for refernce voltage 
        Serial.print("ADC READ VOLTAGE : "); Serial.println(B1_read);
        B1_volt = (B1_read) * 86.1063;                   // AVG Error calculated with CRO and ADC read value among 11 samples : 0.02

        // Taking String Current Samples
        for (unsigned char x = 0; x < 150; x++){ 
          AcsValue = analogRead(A2);                 // A1 - Current
          Samples = Samples + AcsValue;  
          delay(3); // ADC Settling time for maximum optimization 
        }
        AvgAcs=Samples/150.0;
        AcsValueF = ((AvgAcs * (5.0 / 1024.0) - 2.5) )/0.185;   // NOTE : Recheck point for refernce voltage 
        Available_Charge = 0.71 * ( B1_volt );  

        Serial.print("B1_VOLT : "); Serial.println(B1_volt);  
        Serial.print("Available Charge : "); Serial.println(Available_Charge);  
        Serial.print("Charging Current : "); Serial.println(AcsValueF);  
        
        // Converting for 16-bit transmission
        TEVO = (unsigned int)( B1_volt * 10 ); 
        AVCH = (unsigned int)( Available_Charge * 10 );
        if(AcsValueF < 0)
          CHCU = 0x00;
        else
          CHCU = (byte)( AcsValueF * 10 );

        // Loading frame for transmission  
        Serial.print("TEVO"); Serial.println(TEVO);
        Serial.print("AVCH"); Serial.println(AVCH);
        Serial.print("CHCU"); Serial.println(CHCU);

        canMsg1.data[0] = TEVO;  // TERMINAL VOLTAGE : LSB[8] - MSB[15]
        Serial.print("DATA BYTE 0 : "); Serial.println(canMsg1.data[0], HEX);
        canMsg1.data[1] = TEVO >> 8;  // TERMINAL VOLTAGE : LSB[0] - MSB[7]
        Serial.print("DATA BYTE 1 : "); Serial.println(canMsg1.data[1], HEX);
        canMsg1.data[2] = AVCH;
        Serial.print("DATA BYTE 2 : "); Serial.println(canMsg1.data[2], HEX);
        canMsg1.data[3] = AVCH >> 8;
        Serial.print("DATA BYTE 3 : "); Serial.println(canMsg1.data[3], HEX);
        canMsg1.data[4] = CHCU;       // Add OTF at MSB : 0 - OverTemp , 1 - Normal
        Serial.print("DATA BYTE 4 : "); Serial.println(canMsg1.data[4], HEX);
        canMsg1.data[5] = 0xFF;
        canMsg1.data[6] = 0xFF;
        canMsg1.data[7] = 0xFF;
      

        /*

        canMsg1.data[0] = 0x25;  // TERMINAL VOLTAGE : LSB[8] - MSB[15]
        Serial.print("DATA BYTE 0 : "); Serial.println(canMsg1.data[0]);
        canMsg1.data[1] = 0x2f;  // TERMINAL VOLTAGE : LSB[0] - MSB[7]
        Serial.print("DATA BYTE 1 : "); Serial.println(canMsg1.data[1]);
        canMsg1.data[2] = 0x19;
        Serial.print("DATA BYTE 2 : "); Serial.println(canMsg1.data[2]);
        canMsg1.data[3] = 0x8c;
        Serial.print("DATA BYTE 3 : "); Serial.println(canMsg1.data[3]);
        canMsg1.data[4] = 0x8f;       // Add OTF at MSB : 0 - OverTemp , 1 - Normal
        Serial.print("DATA BYTE 4 : "); Serial.println(canMsg1.data[4]);
        canMsg1.data[5] = 0xFF;
        canMsg1.data[6] = 0xFF;
        canMsg1.data[7] = 0xFF;

        */

        // LED Indication
        digitalWrite(7, HIGH);  // LED D5 Indication
        mcp2515.sendMessage(&canMsg1);
        Serial.println("Debug Message : Messages sent");
        delay(100); // Wait for successful transmission
        digitalWrite(7, LOW);
    }
  }  
}








