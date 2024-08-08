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

#========================================================#
## DOCUMENTATION
[DRIVE LINK FOR DOCUMENTS](https://drive.google.com/drive/folders/12Dk6qtcRN3KpQthL_j_CCQviy4w8mpz9?usp=drive_link)