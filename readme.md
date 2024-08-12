# TATA340PP40-FUNCTINOAL-SOFTWARE
#========================================================#
## HARDWARE VERSIONS
### 1. [MCU CARD V.0.1](https://github.com/BhavaySen/TATA340PP40_SOFTWARE/tree/master/Hardware)

![MCU_CARD_V.0.1](https://github.com/BhavaySen/TATA340PP40_SOFTWARE/blob/master/Hardware/mcu_board_images/MCU_CARD_V.0.1.jpg)
#### Observations for changelog
1. Found the mistake in the design, Power supply feed back not connected to the right place.
2. No isolations used, causing frequent damage to the ICs and Microcontroller during switchings.

### 2. [MCU CARD V.0.2](https://github.com/BhavaySen/TATA340PP40_SOFTWARE/tree/master/Hardware)
![MCU_CARD_V.0.2](https://github.com/BhavaySen/TATA340PP40_SOFTWARE/blob/master/Hardware/mcu_board_images/MCU_CARD_V.0.2.jpg)
#### Changelog - date (unknown/missed)
"This version is for testing purpose only, will order only 1-card"
1. updated the complete design/architecture
2. added isolations at input supply and at communications interface of CAN
3. Removed I2C headers from board.
4. Found the AARTECH LOGO missing on silkscreen, will update in the next revision

### 3. [MCU CARD V.0.3](https://github.com/BhavaySen/TATA340PP40_SOFTWARE/tree/master/Hardware)
![MCU_CARD_V.0.3](https://github.com/BhavaySen/TATA340PP40_SOFTWARE/blob/master/Hardware/mcu_board_images/MCU_CARD_V.0.3.jpg)
3 Nos of this version supplies
This version is in production for 8 more Nos. 
#### Changelog - date (unknown/missed)
1. Added Two-NTCs at on the MCU card.
2. Routed 28-V DC supply directly from input to the UCAP interfacing connector.
3. updated crystal oscillator of MCP2515 from 8MHz to 16MHz, 16MHz crystal (THT).
4. Increase in pad size of the 28V to +15V DC/DC Converter.
5. Change in the interface connector to the UCAP board. 
6. Potentiometer RV1 marked as DNP. No need for precise 15V in the circuit.

### 4. [MCU CARD V.0.3 (PRODUCTION-8Nos)](https://github.com/BhavaySen/TATA340PP40_SOFTWARE/tree/master/Hardware)
This is the same version as before with changes in source code, In this version current sensor on UCAP PCB is changes from ACS712-20A to HE055T01.

#### Changes suggested for future version 
1. 28V to 15V power supply should be removed with the 28V to 5V supply, because now there is no need for 15V in the UCAP card, it is converting its own isolated voltage.
2. A Voltage reference IC should be added for the ADC Vref.
3. Should remove non-isolated buck module from the circuit.

#========================================================#
## SOFTWARE VERSIONS
### 1. [TATA_MCU_V.0.1-CODES](https://github.com/BhavaySen/TATA340PP40_SOFTWARE/tree/master/Software/TATA_MCU_V.0.1-CODES)
TATA MCU V.0.1 Codes, This version was the first concept level testing version and this is the initial software development collections.
In this part some open source and easy to use community works used.
MCP2515 - CAN drivers : Imported from github, later this driver is adjusted by me to optimise the data read and write sequences. 
Creating the code for ATMega328 and ATmega8 compatible. This same code can also be installed in ATmega8 for cost effictive and optimized product.

### 2. [TATA_MCU_V.0.2-CODES](https://github.com/BhavaySen/TATA340PP40_SOFTWARE/tree/master/Software/TATA_MCU_V.0.2-CODES)
Second Board design, Architecture of the hardware (MCU) changed in this version, this version of code contains only test codes which helps system integrator to rapidly test/debug the hardware. Please note this is concept level design and with the completion of all the tests and observations other driver for OS for TATA340PP40 unit is also captured. below mentioned tests are coverd in this version.
1. IO Test
2. ADC Test
3. SPI-Comm Test with MCP2515
4. ACS712-Current sensor integration check
5. voltage sensor test
6. temperature sensor test
7. dip switch addressing test

### 3. [TATA_MCU_V.0.3-CODES](https://github.com/BhavaySen/TATA340PP40_SOFTWARE/tree/master/Software/TATA_MCU_V.0.3-CODES)
Final software - Release version development of firmware.
1. TATA_MCU_V.0.3_CODE_V.1.1 : 
This is code for MCU V.0.3 - code version V.1.1 includes : 
1.1. Voltage Monitoring
1.2.Current monitoring
1.3.Temperature Monitoring
1.4.Charge Calculation 
1.5.CAN Communication With All the above data in single message + Device Address (0000 - 1111), Code Version (for eg : V.1.1 ~ V101) etc

2. TATA_MCU_V.0.3_CODE_V.1.2 : 
Added V.1.2 code version in TATA MCU V.0.3 CODES. This code is based on the updated flow of process verified by AGG,
  Added :
    2.1. DIP switch based device addressing module.
    2.2. Over-Temperature detect, force send can msg without request.
    2.3. Added range in temperature sening to avoid hystersis
    2.4. Changes as per the discussion with client -> New data-frame defined byte AVCH is now AMTE.

3. TATA_MCU_V.0.3_CODE_V.1.3 :
Version 1.3 added in V.0.3 CODES, NEW VERSION OF CODE WITH PCB DESIGN V.0.3
  3.1. Two Individual NTCs on board.
  3.2 Removed Capacitance Measurement
  3.3 Added Temperature Data Transmission in CAN Frame ( Revision as per the new Software ICD V._._)
// (Rev 1.2 out of 9.9) -> 1.1 - 1.9 - 2.0 - 9.9(Max)

4. TATA_MCU_V.0.3_CODE_V.1.4 : 
"This is the final version of code for MCU V.0.3 and UCAP board with ACS712 Hall effect sensor. below changes are made in the code after V.1.3.
4.1. Changed the over-temperature fault mechanism
4.2. Now the message will be sent without request if OT detected, and the
   OS thread will go in sleep for 3 sec, it will send message
   continously till the OT fault status becomes NULL. i.e. System
   temperature < 85 Degree-Cel."

5. TATA_MCU_V.0.3_CODE_V.1.5 : 
Updated code for MCU V.0.3 Code version 1.5 for new UCAP PCB with HET055T01 UCAP PCB V2.0
New function added for HE055T01 Hall effect current sensor

#========================================================#
## DOCUMENTATION
[DRIVE LINK FOR DOCUMENTS](https://drive.google.com/drive/folders/12Dk6qtcRN3KpQthL_j_CCQviy4w8mpz9?usp=drive_link)