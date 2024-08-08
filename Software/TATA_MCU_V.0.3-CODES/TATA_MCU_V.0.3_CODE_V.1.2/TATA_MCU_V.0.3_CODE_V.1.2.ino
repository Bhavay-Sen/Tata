
/*
@Author Bhavay Sen
@Date : 31/08/2023
@Hardware : MCU V.0.3
@REV : V.1.2
@Project : TATA340PP40
@Description : 
  This code is based on the updated flow of process verified by AGG,
  Added : 
    1. DIP switch based device addressing module.
    2. Over-Temperature detect, force send can msg without request.
    3. Added range in temperature sening to avoid hystersis
    4. Changes as per the discussion with client -> New data-frame defined byte AVCH is now AMTE.
*/

#define Firmware_Version 1.2 // (Rev 1.2 out of 9.9) -> 1.1 - 1.9 - 2.0 - 9.9(Max)
uint8_t Device_ID = 0x00;
// Device and Firmware information ends here

// Include all custom libraries here
#include <VH1K0T01.h>
#include <ACS712_5.h>
#include <USP10976.h>

// Include all sourced libraries here
#include <mcp2515.h> 
#include <SPI.h>

//Structure for CAN Frame
struct can_frame canREQ_Msg;
struct can_frame canTx_Msg;

//Slave Select pin for SPI Interface b/w uC and MCP2515
MCP2515 mcp2515(10);

//LEDs Defination here
#define GreenLED 3 
#define RedLED 4

// Define for ADC Conversions
#define Vref 4.5 // Vref = Vcc or Supply Voltage to the card // in Volts

//Define for ADC pin measurement
#define voltagePin A2
#define currentPin A1
#define ntcPin A0

//Define for Voltage Calculations, Parameters required
#define Rin 40.1000 // Values for Voltage measurement in kohms 
#define Rb  179.0000 // Value of burden resistance in ohms
#define R2_th 1.0000  // Value of Series Resistor in NTC network, in Kohms

//Define for Charge Calculation
#define UCAP_BANK_CAPACITANCE 0.71 // Capacitance in Farads

//Local defination here
#define uint16_t unsigned int

// Function prototypes here
void Set_Device_Address();

/*                                                               INFORMATION 
              !DANGER
              1. Do not change or use PD5, PD6, PD7 and PB0 in any operation. Note : Do not write or toggle these bits
              2. Set PD3 and PD4 as an output in set-up or initialization of program for LEDs.

*/

  float BANK_VOLTAGE = 0.0;
  float AVAILABLE_CHARGE = 0.0;
  float CHARGING_CURRENT = 0.0;
  float AMBIENT_TEMPERATURE = 0.0;

void setup() {

  //Setting PD7, PD6, PD5 as an Input Pin
  DDRD &= ~(0xE0);
  //Setting PB0 as an Input Pin
  DDRB &= ~(0x01);
  //Setting up device address ( using 4 pin dip switch)
  Set_Device_Address();

  // Visual indication
  //Setting PD3 and PD4 as Output
  DDRD |= 0x18;
  //Toggling bits using PORT Registors
  
  for(byte i = 0 ; i < 5 ; i++){
    PORTD |= (1<<PD3);
    delay(100); // Delay in ms
    PORTD &= ~(1<<PD3);
    delay(100); 
    PORTD |= (1<<PD4);
    delay(100);
    PORTD &= ~(1<<PD4);
    delay(100);
  }

  // Setting up CAN Credentials
  canTx_Msg.can_id = 0x0f6; // CAN Message ID for transmission
  canTx_Msg.can_dlc = 8;   // Data Length of Message

  //Setting up MCP2515
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();

}

void loop() {

  //Terminal Voltage Calculation
   BANK_VOLTAGE = terminalVoltage(Vref, 1, voltagePin, Rin, Rb); //Vref(0.0 - 5.0), samples(0-254), adcPin, Rin(Kohms), Rb(ohms)

  //Charge Calculation
   AVAILABLE_CHARGE = BANK_VOLTAGE * UCAP_BANK_CAPACITANCE;
      
  //Current Measurement function
   CHARGING_CURRENT = currentSense(Vref, 10, currentPin); //Vref, samples, adcpin

  //Temperature Measurement Function
  AMBIENT_TEMPERATURE = temperatureSense(Vref, 1, ntcPin, R2_th); // Vref, samples, adcPin, R2(kOhms)

  if(AMBIENT_TEMPERATURE > 65){
    force_CAN_MSG_SEND_OTF();
  }

  //COMMUNICATION INTERFACE
  //Checking if the Message is received without any error and the request message id is 0x0f2
  if(mcp2515.readMessage(&canREQ_Msg) == MCP2515::ERROR_OK && canREQ_Msg.can_id == 0x0f2 ){
    if(canREQ_Msg.data[0] == 0x28){

       SEND_CAN_MESSAGE(); // Prepare and Send Message

      //Visual Indication
       PORTD |= (1<<PD3);
       delay(100);
       PORTD &= ~(1<<PD3);
       delay(100);
    }
  }
}

// Function Device ID initialization
void Set_Device_Address(){
  Device_ID = (PINB & 0x01) << 3 | ((PIND >> PD7) & 0x01) << 2 | ((PIND >> PD6) & 0x01) << 1 | ((PIND >> PD5) & 0x01) << 0 ;
}

//Function for Over-Temperature Detect force message transmission
void force_CAN_MSG_SEND_OTF(){

  //Special message for OTF (OVER-TEMPERATURE FAULT)
  uint16_t TEVO = BANK_VOLTAGE * 10;
  uint16_t AVCH = AVAILABLE_CHARGE * 10;
  uint8_t  CHCU = (CHARGING_CURRENT < 0) ? 0x00 : (CHARGING_CURRENT * 10);
  
  //Prepare CAN MESSAGE
  canTx_Msg.data[0] = TEVO;
  canTx_Msg.data[1] = TEVO >> 8;
  canTx_Msg.data[2] = AVCH;
  canTx_Msg.data[3] = AVCH >> 8;
  canTx_Msg.data[4] = CHCU; // With Over-Temperature
  canTx_Msg.data[5] = 0;
  canTx_Msg.data[6] = 0;
  canTx_Msg.data[7] = 0;

  mcp2515.sendMessage(&canTx_Msg);

  // Visual Indication
  PORTD |= (1<<PD4);
  delay(100);
  PORTD &= ~(1<<PD4);
  delay(100);


  //Delay of 3 sec after detecting Over-Temperature 
  delay(3000);
}

void SEND_CAN_MESSAGE(){

  uint16_t TEVO = BANK_VOLTAGE * 10;
  uint16_t AVCH = AVAILABLE_CHARGE * 10;
  uint8_t  CHCU = (CHARGING_CURRENT < 0) ? 0x00 : (CHARGING_CURRENT * 10);
          
  //Prepare CAN MESSAGE
  canTx_Msg.data[0] = TEVO;
  canTx_Msg.data[1] = TEVO >> 8;
  canTx_Msg.data[2] = AVCH;
  canTx_Msg.data[3] = AVCH >> 8;
  canTx_Msg.data[4] = CHCU; // With Over-Temperature

  //Internal Use Data
  canTx_Msg.data[5] = Firmware_Version * 10; // Code Version - Will be in char like "V.1.1 ~ V101"
  canTx_Msg.data[6] = Device_ID;
  canTx_Msg.data[7] = 0;  // In future, Product Serial Number and Info.

  mcp2515.sendMessage(&canTx_Msg);

  // // Visual Indication
  // PORTD |= (1<<PD4);
  // delay(100);
  // PORTD &= ~(1<<PD4);
  // delay(100);
}

