char j_val = 'j';
char r_val = 'r';
float adc_val = 0;
float voltage = 0;
float pwr5v_val = 4.84;
float R_coeff = 4;
#define j_pin A6 
#define r_pin A7
#define adc_pin A0 

void UART_Control()
{
  char Uart_Data=0;
 if(Serial.available())
  {
   Uart_Data = Serial.read();
  }
  switch(Uart_Data)
  {
     case 'J':  j_val = 'J'; digitalWrite(j_pin, HIGH); adc_val = analogRead(adc_pin); Serial.print(j_val); Serial.print(r_val); voltage = (adc_val*pwr5v_val*R_coeff)/1024; Serial.print(voltage); Serial.print("V\r\n"); break;
     case 'j':  j_val = 'j'; digitalWrite(j_pin, LOW);  adc_val = analogRead(adc_pin); Serial.print(j_val); Serial.print(r_val); voltage = (adc_val*pwr5v_val*R_coeff)/1024; Serial.print(voltage); Serial.print("V\r\n"); break;
     case 'R':  r_val = 'R'; digitalWrite(r_pin, HIGH); adc_val = analogRead(adc_pin); Serial.print(j_val); Serial.print(r_val); voltage = (adc_val*pwr5v_val*R_coeff)/1024; Serial.print(voltage); Serial.print("V\r\n"); break;
     case 'r':  r_val = 'r'; digitalWrite(r_pin, LOW);  adc_val = analogRead(adc_pin); Serial.print(j_val); Serial.print(r_val); voltage = (adc_val*pwr5v_val*R_coeff)/1024; Serial.print(voltage); Serial.print("V\r\n"); break;
     case '?':                                          adc_val = analogRead(adc_pin); Serial.print(j_val); Serial.print(r_val); voltage = (adc_val*pwr5v_val*R_coeff)/1024; Serial.print(voltage); Serial.print("V\r\n"); break;
   }
}


void IO_init()
{
  pinMode(j_pin, OUTPUT); digitalWrite(j_pin, LOW);
  pinMode(r_pin, OUTPUT); digitalWrite(r_pin, LOW);

}


void setup()
{
  IO_init();
  
  Serial.begin(115200);
  
}


void loop()
{
  UART_Control();
 
}
