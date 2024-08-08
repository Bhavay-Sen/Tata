/*
@Author Bhavay Sen
@Date : 28/08/2023
@Hardware : MCU V.0.3
@REV : V.1.1
@Project : TATA340PP40
@Description : 

*/
#define Firmware_Version 1.1 // Can be go up to 9.9 Revisions, After that new encoding technique need to be used.

// Device and Firmware information ends here


// Include all custom libraries here
#include <VH1K0T01.h>
#include <ACS712_5.h>
#include <USP10976.h>

// Include all sourced libraries here
#include <mcp2515.h> 
#include <SPI.h>

//LEDs Defination here
#define GreenLED 3 
#define RedLED 4

// Define for ADC Conversions
#define Vref 4.55 // Vref = Vcc or Supply Voltage to the card


//Define for ADC pin measurement
#define voltagePin A2
#define currentPin A1
#define ntcPin A0

float CHARGING_CURRENT = 0.00;

//Define for Voltage Calculations, Parameters required
#define Rin 40.1000 // Values for Voltage measurement in kohms 
#define Rb  179.1000 // Value of burden resistance in ohms
#define R2_th 1.0000  // Value of Series Resistor in NTC network, in Kohms

//Define for Charge Calculation
#define UCAP_BANK_CAPACITANCE 0.714

//Structure for CAN Frame
struct can_frame canREQ_Msg;
struct can_frame canTx_Msg;

//Slave Select pin for SPI Interface b/w uC and MCP2515
MCP2515 mcp2515(10);

//Local defination here
#define uint16_t unsigned int

//Setting up Registers for uC here
void setup() {

  Serial.begin(115200);

  pinMode(GreenLED,OUTPUT);
  pinMode(RedLED,OUTPUT);

  /* Idea : To design a request pattern which can Re-Write the Msg ID, by the Client.
     So they can be able to program/Code device ID by them self.
  */
  
  //Message ID for transmission 
  canTx_Msg.can_id = 0x0f6; // CAN MSG ID : 11-bit identifier - 0000 1111 0110 

  // AT the time of delivery coustomer will provide the ID, We'll hardcode the Message ID
  canTx_Msg.can_dlc = 8;
  
  //Setting up MCP2515
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();

  // Boot-Up Sequance Visual indicator
  for(char i = 0; i < 4 ; i++){
    digitalWrite(GreenLED,HIGH);
    delay(100);
    digitalWrite(GreenLED,LOW);
    delay(100);
    digitalWrite(RedLED,HIGH);
    delay(100);
    digitalWrite(RedLED,LOW);
    delay(100);
  }
}

float currentRead(){
  float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;

    for (int x = 0; x < 150; x++){ //Get 150 samples
    AcsValue = analogRead(currentPin);     //Read current sensor values   
    Samples = Samples + AcsValue;  //Add samples together
    delay (3); // let ADC settle before next sample 3ms
  }
  AvgAcs=Samples/150.0;//Taking Average of Samples

  //((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
  //2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
  //out to be 2.5 which is out offset. If your arduino is working on different voltage than 
  //you must change the offset according to the input voltage)
  //0.185v(185mV) is rise in output voltage when 1A current flows at input
  AcsValueF = ((AvgAcs * (Vref / 1024.0)) - (Vref/2) )/0.185;
  return AcsValueF;
}

void loop() {

  if(mcp2515.readMessage(&canREQ_Msg) == MCP2515::ERROR_OK && canREQ_Msg.can_id == 0x0F2){
    if(canREQ_Msg.data[0] == 0x28){
      
      //Voltage Measurement function
      float BANK_VOLTAGE = terminalVoltage(Vref, 10, voltagePin, Rin, Rb); //Vref(0.0 - 5.0), samples(0-254), adcPin, Rin(Kohms), Rb(ohms)

      //Charge Calculation
      float AVAILABLE_CHARGE = BANK_VOLTAGE * UCAP_BANK_CAPACITANCE;
      
      //Temperature Measurement Function
      float AMBIENT_TEMPERATURE = temperatureSense(Vref, 10, ntcPin, R2_th); // Vref, samples, adcPin, R2(kOhms)

      //Current Measurement function
      // float CHARGING_CURRENT = currentSense(Vref, 10, currentPin); //Vref, samples, adcpin
      // Serial.print("Current : "); Serial.println(CHARGING_CURRENT);
      // CHARGING_CURRENT += 0.11;
      CHARGING_CURRENT = 0.0;
      CHARGING_CURRENT = currentRead();

      Serial.print("Charge : "); Serial.println(AVAILABLE_CHARGE);
      Serial.print("Voltage : "); Serial.println(BANK_VOLTAGE);
      Serial.print("Temperature : "); Serial.println(AMBIENT_TEMPERATURE);

      Serial.println("#############################################################");
      Serial.println();

      uint16_t TEVO = BANK_VOLTAGE * 10;
      uint16_t AVCH = AVAILABLE_CHARGE * 10;
      // if(CHARGING_CURRENT < 0){
      //   CHARGING_CURRENT = (-1) * CHARGING_CURRENT;
      // }
      // // CHARGING_CURRENT = 3.5;
      Serial.print("Charging Current : "); Serial.println(CHARGING_CURRENT + 0.15);
      char CUCH = CHARGING_CURRENT * 10.0;
      Serial.print("TTTT : "); Serial.println(CHARGING_CURRENT * 10.0);
      // // Over-Temperature Fault Check
      // if(AMBIENT_TEMPERATURE > 65){ // 65 Degree C
      //   CUCH |= 0x80; //  CHCU | 1000 0000
      // }
      
      //Client Data
      // canTx_Msg.data[0] = TEVO;
      canTx_Msg.data[0] = TEVO;
      canTx_Msg.data[1] = TEVO >> 8;
      canTx_Msg.data[2] = AVCH;
      canTx_Msg.data[3] = AVCH >> 8;
      canTx_Msg.data[4] = CUCH; // OTF(1-bit) CUCH(7-bit);

      //ASL Data
      canTx_Msg.data[5] = Firmware_Version * 10; // Code Version - Will be in char like "V.1.1 ~ V101"
      canTx_Msg.data[6] = 0;
      canTx_Msg.data[7] = 0;

      for(int i = 0; i < 8 ; i++){
        Serial.println(canTx_Msg.data[i]);
      }

      mcp2515.sendMessage(&canTx_Msg);
    }
  }
}


