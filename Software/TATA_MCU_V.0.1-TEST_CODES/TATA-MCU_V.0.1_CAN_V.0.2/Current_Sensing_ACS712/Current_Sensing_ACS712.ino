// *( volatile int (*pi) ) 0x25 ; // Memory address of PORTB Register.

void setup() {
  Serial.begin(9600); //Start Serial Monitor to display current read value on Serial monitor
  pinMode(7, OUTPUT);
}

void loop() {
 
  Serial.println("-------------Time Stamp_initial------------------");

  unsigned int x=0;
  float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;

  float B1_raw  = 0.00 ,
        B2_raw  = 0.00 ,
        B1_Samples = 0.00,
        B2_Samples = 0.00,
        B1_read = 0.00 , 
        B2_read = 0.00 , 
        B1_volt = 0.00 , 
        B2_volt = 0.00 ,
        Available_Charge = 0.00;

  for(int i = 0 ; i < 150 ; i++){
    B1_raw = analogRead(A0);
    B1_Samples = B1_Samples + B1_raw;
  }

  for(int i = 0 ; i < 150 ; i++){
    B2_raw = analogRead(A1);
    B2_Samples = B2_Samples + B2_raw;
  }

  B1_raw = B1_Samples / 150.0 ;
  B2_raw = B2_Samples / 150.0 ;

  B1_read = ( B1_raw * 4.98 ) / 1023.0 ;
  B2_read = ( B2_raw * 4.98 ) / 1023.0 ;

  B1_volt = B1_read * 86.1063;
  B2_volt = B2_read * 86.1063;

  digitalWrite(7, HIGH);
  for (int x = 0; x < 150; x++){ //Get 150 samples
    AcsValue = analogRead(A2);     //Read current sensor values   
    Samples = Samples + AcsValue;  //Add samples together
    delay (3); // let ADC settle before next sample 3ms
  }
  digitalWrite(7, LOW);

  AvgAcs=Samples/150.0;//Taking Average of Samples

  //((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
  //2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
  //out to be 2.5 which is out offset. If your arduino is working on different voltage than 
  //you must change the offset according to the input voltage)
  //0.185v(185mV) is rise in output voltage when 1A current flows at input
  // AcsValueF = (2.5 - (AvgAcs * (5.0 / 1024.0)) )/0.185;

  AcsValueF = ((AvgAcs * (5.0 / 1024.0) - 2.5) )/0.185;
  Available_Charge = 0.714 * ( (B1_volt + B2_volt) / 2.00 );

  Serial.print(AcsValueF - 0.07); Serial.println(" Ampere"); //Print the read current on Serial monitor
  Serial.println("|||||||||||||||||||||||||||||||");
  Serial.print("DIVIDER VOLTAGE Board 1 "); Serial.println(B1_read);
  Serial.print(B1_volt); Serial.println(" Volt 1");
  Serial.print("DIVIDER VOLTAGE Board 2 "); Serial.println(B 2_read);
  Serial.print(B2_volt); Serial.println(" Volt 2");
  Serial.print("Available Charge : "); Serial.print(Available_Charge); Serial.println(" Cou lombs");
  Serial.println("|||||||||||||||||||||||||||||||");
  // delay(10);

  delay(3);
}