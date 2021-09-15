#include "HX711.h" //hx711 lib by Bogdan Necula

char j_val = 'j';
char r_val = 'r';
float adc_val = 0;
float voltage = 0;
float pwr5v_val = 4.84;
float R_coeff = 4;

#define peep_pin 9 // подключение зуммера
#define crit_voltage 10.9

#define button_pin A3

#define j_pin A6
#define r_pin A7
#define adc_pin A0

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = A1;
const int LOADCELL_SCK_PIN = A2;

HX711 scale;
long readingA;
long readingB;
int hx711_reading_status;

int counter = 0;
int button = 0;
float calcVoltage() {
  adc_val = analogRead(adc_pin);
  return (adc_val * pwr5v_val * R_coeff) / 1024;
}

void commonRoutine() {
  HX711_read();
  Serial.print(j_val);
  Serial.print(r_val);
  Serial.print(calcVoltage());
  Serial.print("V");
  HX711_print();
  button_print();
}

void UART_Control() {
  char Uart_Data = 0;
  if (Serial.available()) {
    Uart_Data = Serial.read();
  }
  switch (Uart_Data) {
    case 'J':
      j_val = 'J';
      digitalWrite(j_pin, HIGH);
      commonRoutine();
      break;
    case 'j':
      j_val = 'j';
      digitalWrite(j_pin, LOW);
      commonRoutine();
      break;
    case 'R':
      r_val = 'R';
      digitalWrite(r_pin, HIGH);
      commonRoutine();
      break;
    case 'r':
      r_val = 'r';
      digitalWrite(r_pin, LOW);
      commonRoutine();
      break;
    case '?':
      commonRoutine();
      break;
  }
}

void IO_init() {
  pinMode(peep_pin, OUTPUT);
  digitalWrite(peep_pin, LOW);

  pinMode(j_pin, OUTPUT);
  digitalWrite(j_pin, LOW);
  pinMode(r_pin, OUTPUT);
  digitalWrite(r_pin, LOW);

  pinMode(button_pin, INPUT_PULLUP); //B~
}

void HX711_read() {
  if (scale.is_ready()) {
    // The function is used to select the channel A or channel B, by passing 128
    // or 64 for channel A, or 32 for channel B as the parameter
    scale.set_gain(128); // A gain 64 or 128
    readingA = scale.read();
    scale.set_gain(32); // B gain 32
    readingB = scale.read();
    hx711_reading_status = 1;
  } else {
    hx711_reading_status = 0;
  }
}

void HX711_print() {
  if (hx711_reading_status) {
    Serial.print(",");
    Serial.print(4 * (readingA + 1010443)); // const for A channel
    scale.set_gain(32);                     // B gain 32
    Serial.print("A,");
    Serial.print(readingB + 1209022); // const for B channel
   //    Serial.print("B\r\n");
    Serial.print("B");
  } else {
    Serial.print(",");
    Serial.print(0);    // const for A channel
    scale.set_gain(32); // B gain 32
    Serial.print("A,");
    Serial.print(0); // const for B channel
    //    Serial.print("B\r\n");
    Serial.print("B");
  }
}

void button_print() {
  if (button) {
    Serial.print(",1\r\n");
    button = 0;
  }
  else   Serial.print(",0\r\n");

}

void peepOrNotToPeep() {
  if (counter > 15000) {
    if (calcVoltage() < crit_voltage)
      analogWrite(peep_pin, 127);
    else
      digitalWrite(peep_pin, LOW);
    counter = 0;
  }
  counter++;
}

void setup() {
  IO_init();

  Serial.begin(115200);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {
  UART_Control();
  peepOrNotToPeep();
  if (!digitalRead(button_pin)) button = 1;
}
