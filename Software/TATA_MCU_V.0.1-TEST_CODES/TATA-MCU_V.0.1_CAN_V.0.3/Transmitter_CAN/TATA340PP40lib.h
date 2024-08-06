/*This function takes ADC value as an input and return calculated terminal voltage*/
float senseVoltage(unsigned int adcValue, char adcSamples, float refVoltageValue);

/*This function takes ADC value as an input and return calculated charging current*/
float senseChargingCurrent(unsigned int adcValue, char adcSamples, float refVoltageValue);

/*This function takes ADC value as an input and return calculated temperature*/
float senseTemperature(unsigned int adcValue, char adcSamples, float refVoltageValue);

/*This function takes constant capcitance value as an input and return calculated Available charge by multiplying terminal voltage and capacitance*/
float calclateAvailableCharge(float capacitance, float senseVoltage);

/*
txMessageID : Transmitting message ID
rxMessageID : Message ID for the request of the data
requestData : first byte of DATA, decided for request
dlc : Data Length
data[dlc] : Array of data, max size 8 bytes
*/
void canCommunicationSetup(unsigned int txMessageID, unsigned int rxMessageID, char requestData, char dlc, char data[dlc]);