
/* Pinout connection
 *            RO    -   D10
 *            RE    -   D3
 *            DE    -   D2
 *            DI    -   D9
 *            VCC   -   5V
 *            A     -   A+  (Sensor)
 *            B     -   B-  (Sensor)
 *            GND   -   GND
 */
#include <SoftwareSerial.h>
#include <Wire.h>

#define MAX485_DE        6
#define MAX485_RE_NEG    5
#define RS485Transmit    HIGH
#define RS485Receive     LOW

SoftwareSerial RS485Serial(10,9,false);  //RX, TX, false = TTL

void readTemp(){
  digitalWrite(MAX485_DE, RS485Transmit);
  digitalWrite(MAX485_RE_NEG, RS485Transmit);
  byte Temp_request[] = {0x02, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xF9};
  RS485Serial.write(Temp_request, sizeof(Temp_request));
  RS485Serial.flush();

  digitalWrite(MAX485_DE, RS485Receive);
  digitalWrite(MAX485_RE_NEG, RS485Receive);
  RS485Serial.listen();
  int length = 8;
  byte Temp_buff[length];
  RS485Serial.readBytes(Temp_buff,length);

  Serial.print("Temp: ");
  float Temp = word(Temp_buff[3], Temp_buff[4]);
  Temp = Temp/10;
  Serial.print(Temp);       
}

void readHum(){
  digitalWrite(MAX485_DE, RS485Transmit);
  digitalWrite(MAX485_RE_NEG, RS485Transmit);
  byte Hum_request[] = {0x02, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0xF9};
  RS485Serial.write(Hum_request, sizeof(Hum_request));
  RS485Serial.flush();

  digitalWrite(MAX485_DE, RS485Receive);
  digitalWrite(MAX485_RE_NEG, RS485Receive);
  RS485Serial.listen();
  int length = 8;
  byte Hum_buff[length];
  RS485Serial.readBytes(Hum_buff,length);

  Serial.print(". Hum: ");
  float Hum = word(Hum_buff[3], Hum_buff[4]);
  Hum = Hum/10;
  Serial.print(Hum);             
}

void readCond(){
  digitalWrite(MAX485_DE, RS485Transmit);
  digitalWrite(MAX485_RE_NEG, RS485Transmit);
  byte Cond_request[] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x39};
  RS485Serial.write(Cond_request, sizeof(Cond_request));
  RS485Serial.flush();

  digitalWrite(MAX485_DE, RS485Receive);
  digitalWrite(MAX485_RE_NEG, RS485Receive);
  RS485Serial.listen();
  int length = 8;
  byte Cond_buff[length];
  RS485Serial.readBytes(Cond_buff,length);

  Serial.print(". Cond: ");
  int Cond = word(Cond_buff[3], Cond_buff[4]);
  Serial.println(Cond);
}

void setup(){
  pinMode(MAX485_DE, OUTPUT);
  pinMode(MAX485_RE_NEG, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Starting...");
  RS485Serial.begin(9600);
  Serial.println("Reading ComWinTop Soil Moisture Sensor (Model THC-S)");
  // adquired from https://www.aliexpress.com/store/910369336
  delay(200);
}

void loop(){
  readTemp();
  readHum();
  readCond();
}