/*
@Author Bhavay Sen
@Date : 04/10/2023
@REV : V.1.4
@Hardware : MCU V.0.3
@Project : TATA340PP40
@Description : 
  Changed the Over-Temperature fault mechanisim
    ALGO : if(OT detected){
            send Message without request
            then, Turn on timer 1 in CTC mode for 3 sec 
            then, at ISR for comp A, Turn off timer 1 , reset the check condition flag and again repeat the process
            }
  .
  .
*/
// (Rev 1.2 out of 9.9) -> 1.1 - 1.9 - 2.0 - 9.9(Max)
#define Firmware_Version 1.4 
// volatile uint8_t Device_ID = 0x00;
uint8_t Device_ID = 0x00;

//CAN-Communication
#include <mcp2515.h> 
#include <SPI.h>

// Measurements
#include <VH1K0T01.h>
#include <ACS712_20.h>
#include <USP10976.h>

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
#define voltagePin A3
#define currentPin A2
#define ntcPin2 A1
#define ntcPin1 A0

//Define for Voltage Calculations, Parameters required
#define Rin 42.4000 // Values for Voltage measurement in kohms 
#define Rb  178.0000 // Value of burden resistance in ohms
#define R2_th 1.0000  // Value of Series Resistor in NTC network, in Kohms

#define Threshold_temp 85 // In Degree Centigrade
uint8_t temp_flag = 0; // Temperature flag check

float BANK_VOLTAGE = 0.0;
float CHARGING_CURRENT = 0.0;
float AMBIENT_TEMPERATURE = 0.0;

void Set_Device_Address(){
  Device_ID = (PINB & 0x01) << 3 | ((PIND >> PD7) & 0x01) << 2 | ((PIND >> PD6) & 0x01) << 1 | ((PIND >> PD5) & 0x01) << 0 ;
}

void SEND_CAN_MESSAGE(){
  uint16_t TEVO = BANK_VOLTAGE * 10;
  int AMTE = AMBIENT_TEMPERATURE * 10; // This can be a negative value as well (-30 to +85)
  uint8_t CHCU = 0;
  if(CHARGING_CURRENT < 0){
    CHCU = 0;
  }
  else{
    CHCU = (CHARGING_CURRENT+0.20) * 10.0; // Error Compansatation
  }

  // Serial.println(CHARGING_CURRENT+0.20);
          
  //Prepare CAN MESSAGE
  canTx_Msg.data[0] = TEVO & 0x00FF;
  // canTx_Msg.data[0] = TEVO;
  canTx_Msg.data[1] = (TEVO >> 8) & 0x00FF;
  // canTx_Msg.data[1] = TEVO >> 8;
  canTx_Msg.data[2] = AMTE & 0x00FF;
  // canTx_Msg.data[2] = AMTE;
  canTx_Msg.data[3] = (AMTE >> 8) & 0X00FF;
  // canTx_Msg.data[3] = AMTE >> 8;
  canTx_Msg.data[4] = CHCU; // With Over-Temperature
  // Serial.println(CHCU);

  //Internal Use Data
  canTx_Msg.data[5] = (uint8_t)(Firmware_Version * 10); // Code Version - Will be in char like "V.1.1 ~ V101"
  canTx_Msg.data[6] = Device_ID;
  // Serial.println(Device_ID);
  canTx_Msg.data[7] = 0;  // In future, Product Serial Number and Info.

  mcp2515.sendMessage(&canTx_Msg);

}

// ISR routine - !! VERY IMPORTANT 
ISR(TIMER1_COMPA_vect){
  // Turn OFF timer-1
  TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Turn off timer in ISR
  // Set the flag to 1 after 3 sec count, [ it will go and check the condition again]
  temp_flag = 0;
}

void set_timer_1(){
  // Conterol REG A 
	TCCR1A = 0;

  // Conterol REG B with CTC mode and Timer OFF
	TCCR1B = (1 << WGM12) | (0 << CS12) | (0 << CS10);

  // Conterol REG C
	TCCR1C = 0;

  // Initial value of Timer
	TCNT1 = 0;
  
  // Compare match value for TIMER-1
	OCR1A = 46875;
	
  // Timer-1 Compare match interrupt enable
	TIMSK1 = (1 << OCIE1A);	
}

void force_CAN_MSG_SEND_OTF(){

  //Special message for OTF (OVER-TEMPERATURE FAULT)
  uint16_t TEVO = BANK_VOLTAGE * 10;
  int AMTE = AMBIENT_TEMPERATURE * 10; // This can be a negative value as well (-30 to +85)
  uint8_t CHCU = 0;
  if(CHARGING_CURRENT < 0){
    CHCU = 0;
  }
  else{
    CHCU = (CHARGING_CURRENT+0.20) * 10.0; // Error Compansatation
  }
  
  //Prepare CAN MESSAGE
  canTx_Msg.data[0] = TEVO;
  canTx_Msg.data[1] = TEVO >> 8;
  canTx_Msg.data[2] = AMTE;
  canTx_Msg.data[3] = AMTE >> 8;
  canTx_Msg.data[4] = CHCU; // With Over-Temperature

  //Internal Use Data
  canTx_Msg.data[5] = (uint8_t)(Firmware_Version * 10); // Code Version - Will be in char like "V.1.1 ~ V101"
  canTx_Msg.data[6] = Device_ID;
  canTx_Msg.data[7] = 0;  // RSVD for future use

  mcp2515.sendMessage(&canTx_Msg);

  // Visual Indication - OVER-TEMP FAULT MESSAGE VISUALIZATION
  // GREEN LED
  PORTD |= (1<<PD4);
  delay(100);
  PORTD &= ~(1<<PD4);
  delay(100);
}

/*                                                               INFORMATION 
              !DANGER
              1. Do not change or use PD5, PD6, PD7 and PB0 in any operation. Note : Do not write or toggle these bits
              2. Set PD3 and PD4 as an output in set-up or initialization of program for LEDs.

*/

void setup() {

  Serial.begin(9600);

  // For DIP SWITCH ADDRESSING 
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

  // Serial.println(Device_ID, HEX); // Checked and Verified

  // Setting up CAN Credentials
  canTx_Msg.can_id = 0x0f6; // CAN Message ID for transmission
  canTx_Msg.can_dlc = 8;   // Data Length of Message

  //Setting up MCP2515
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ); 
  mcp2515.setNormalMode();

  set_timer_1(); // Setup for timer 1
  sei(); // Enable global interrupts
}

void loop(){

  //Current Measurement function
    CHARGING_CURRENT = currentSense(Vref, 8, currentPin); //Vref, samples, adcpin

  //Terminal Voltage Calculation
    BANK_VOLTAGE = terminalVoltage(Vref, 5, voltagePin, Rin, Rb); //Vref(0.0 - 5.0), samples(0-254), adcPin, Rin(Kohms), Rb(ohms)
  
  //Temperature Measurement Function
    float ntc1_temp = temperatureSense(Vref, 2, ntcPin1, R2_th); // Vref, samples, adcPin, R2(kOhms)
    float ntc2_temp = temperatureSense(Vref, 2, ntcPin2, R2_th); // Vref, samples, adcPin, R2(kOhms)

    AMBIENT_TEMPERATURE = (ntc1_temp + ntc2_temp) / 2.0 ;
  
    if(AMBIENT_TEMPERATURE > Threshold_temp  && temp_flag == 0){
      // SEND FORCE MESSAGE
      force_CAN_MSG_SEND_OTF();
      temp_flag = 1;
      // TIMER-ON FOR 3-SEC
      TCCR1B |= (1 << CS12) | (1 << CS10); // TIMER ON FOR 3-SEC with pre-scaler 1024 at 16MHz
  }

  //COMMUNICATION INTERFACE
  //Checking if the Message is received without any error and the request message id is 0x0f2
  if(mcp2515.readMessage(&canREQ_Msg) == MCP2515::ERROR_OK && canREQ_Msg.can_id == 0x0f2 ){
    if(canREQ_Msg.data[0] == 0x28){

       SEND_CAN_MESSAGE(); // Prepare and Send Message

      //Visual Indication
      // RED-LED
       PORTD |= (1<<PD3);
       delay(100);
       PORTD &= ~(1<<PD3);
       delay(100);
    }
  }
}