#include <Arduino.h>
#include <OneWire.h>
#include <Adafruit_NeoPixel.h>
#include <DallasTemperature.h>
#include <GyverOLED.h>
//  #include <sstream>
// #include <BLEDevice.h>//https://github.com/nkolban/ESP32_BLE_Arduino
// #include <BLEUtils.h>
// #include <BLEScan.h>
// #include <BLEAdvertisedDevice.h>


#include "conect.h"
//#include "bk.h"

#define DOWN 23
#define UP 32
#define OK 35

#define first 27
#define seconde 26
#define third 25
#define fourth 33

#define count_led 5
#define PIN 14

#define boil 15



// BLEScan* esp_scan;
// String Bluetooth_device_mac;
// uint8_t Bluetooth_device_rssi;





GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;  

uint32_t buttonTimer; 
uint32_t timerTemp;
uint32_t buttonTimer2;

OneWire oneWire(13);  // порт подключения датчика ТЕМПЕРАТУРЫ ЧАЙНИКА
DallasTemperature ds(&oneWire);


Adafruit_NeoPixel strip = Adafruit_NeoPixel(count_led, PIN, NEO_GRB + NEO_KHZ800);

//GyverOS<5> OS; 
static uint8_t temp = ds.getTempCByIndex(0);

static boolean flag = true; 
//static boolean buttonFlag = true;
static uint8_t boilSet = 38;
static uint8_t emschet = 0;

static uint8_t level = 0;




//  void Bluetooth_scan() {
//       BLEScanResults foundDevices = esp_scan->start(2);//(секунд)
//       uint16_t devices_found_number = foundDevices.getCount();//Количество найденных устройств
//       for (u8_t i = 0; i < devices_found_number; i++){
//            BLEAdvertisedDevice d = foundDevices.getDevice(i);
//            Bluetooth_device_mac = d.getAddress().toString().c_str();
//            Serial.println(Bluetooth_device_mac);//Список обнаруженных устройств
//            if (Bluetooth_device_mac == "d6:39:d8:b5:94:82") {
//                 Serial.println("Device Found #1");//Устройство найдено # 1
//                 Bluetooth_device_rssi = d.getRSSI();
//                 i=devices_found_number;
                 
//                  if (Bluetooth_device_rssi > -50){
//                      Serial.println("signal strength is sufficient #2");//Уровень сигнала достаточен # 2
//                      digitalWrite(2, HIGH);   
//                      delay(1000);                     
//                      digitalWrite(2, LOW);   
//                     }
//                }
//            esp_scan->clearResults();
//            }
// } 



void callback(char* topic, byte* payload, unsigned int length) {
    
  String data_pay;
  for (int i = 0; i < length; i++) {
    data_pay += String((char)payload[i]);
  }
    if( String(topic) == relay_topic ){
        
    if( String(topic) == relay_topic ){
        // if(data_pay == "ON") relay_on = true;
        // if(data_pay == "OFF") relay_on = false;
        int tempIn = atoi(data_pay.c_str());
  if (tempIn >= 39)
  {
    boilSet = tempIn;
  }
  else boilSet = 0;
  }  
     }
}
// void wifiGet(){
// char* topic;


// }
void takct_test(){
  if(digitalRead(UP)) Serial.print("UP");
  if(digitalRead(DOWN)) Serial.print("DOwn");
  if(digitalRead(OK)) Serial.print("OK");
}
void oledStatik(){
  oled.roundRect(100, 15, 122, 20, OLED_STROKE);
  oled.roundRect(100, 25, 122, 30, OLED_STROKE);
  oled.roundRect(100, 35, 122, 40, OLED_STROKE);
  oled.roundRect(100, 45, 122, 50, OLED_STROKE);
  oled.roundRect(100, 55, 122, 60, OLED_STROKE);
}

void emergeci()
{
   
  if (temp > 110)
  {
    emschet++;
  }
  if (emschet >= 3)
  {
      oled.clear();
    digitalWrite(boil, 0);
    oled.setCursorXY(0, 0);
    oled.setScale(3);
    oled.print("ошибка N1");
      oled.setScale(1);
   // oled.update();
  }
  
}
void boilON()
{
  if (level == 0)
  {
    digitalWrite(boil, 0);
  }
  else
  {
  if (boilSet >= 39)
  {
    digitalWrite(boil, 1);
    //Serial.print("start");
  }
  if (boilSet <= 39)
  {
    digitalWrite(boil, 0);
    //Serial.print("start");
  }
  if (temp > boilSet)
  {
    digitalWrite(boil, 0);
    //Serial.print("finish");
  }
  }
}

void boilFunc(){
  if (millis() - buttonTimer >= 130)
{
  if (digitalRead(OK)) flag = !flag;
  buttonTimer = millis();
}
  
}

void boilSetUp(){
  if(digitalRead(UP)) boilSet++;//, Serial.println(boilSet);
  if(digitalRead(DOWN)) boilSet--;
}
void boilSetHold(){
    if (millis() - buttonTimer2 >= 500)
  {

    // if(digitalRead(UP)){
    // boilSet += 3;
    // }
    // if(digitalRead(DOWN)){
    // boilSet -= 3;
    // }
    // buttonTimer2 = millis();
    if(digitalRead(UP)){
    boilSet += 1;
    }
    if(digitalRead(DOWN)){
    boilSet -= 1;
    }
    buttonTimer2 = millis();
  }
  
}

void kettelSwitch() {
  // if (digitalRead(OK))
  // {
  //   flag = !flag;
  //  // Serial.println(flag);
  // }
  if (flag)
  {
    oled.setCursorXY(30, 30);
    oled.setScale(4);
    oled.print(temp);
    oled.setScale(1);
    oled.setCursorXY(0, 40);
    oled.print("Now");
    oled.setScale(1);
  }
  else
  {
    oled.setCursorXY(30, 30);
    oled.setScale(4);
    if (boilSet >= 39) oled.print(boilSet);
    else oled.print("Off");
    oled.setScale(1);
    oled.setCursorXY(0, 40);
    oled.print("Will");
    //oled.print("Boil");
    oled.setScale(1);

  }
  // if (millis() - timerOled >= 9000)
  // {
  //   flag = !flag;
  //   timerOled = millis();
  //  // Serial.println(flag);
  //}
}

void kettEmpty(){
  if (level == 0)
  {
    oled.clear();
   oled.setCursorXY(0,10);
    oled.setScale(1);
    oled.print("Пополниет");
    oled.setCursorXY(0,30);
    oled.print("запас воды");
    oled.setScale(1);
  }
}

void oledControl(){
  ds.requestTemperatures(); 
  temp = ds.getTempCByIndex(0);
  oled.clear();
  oled.roundRect(22  + 2, 8 - 4,30 + 65 - 2, 8 + 8 + 2, OLED_STROKE);
  oled.setCursorXY(42, 8);
  oled.print("BelTea");

  kettelSwitch();
  kettEmpty();
  oledStatik();
  
  //Serial.print(level);
  
  //if (millis() - timerOled >= 0)
  //{
  switch (level)
  {
  case 0:
    oled.roundRect(100, 55, 122, 60, OLED_FILL);   
    oled.update();
    break;
  case 1:
    oled.roundRect(100, 45, 122, 50, OLED_FILL);
    oled.roundRect(100, 55, 122, 60, OLED_FILL);
    oled.update();
    break;
    case 2:
    oled.roundRect(100, 35, 122, 40, OLED_FILL);
    oled.roundRect(100, 45, 122, 50, OLED_FILL);
    oled.roundRect(100, 55, 122, 60, OLED_FILL);
    oled.update();
    break;
    case 3:
    oled.roundRect(100, 25, 122, 30, OLED_FILL);
    oled.roundRect(100, 35, 122, 40, OLED_FILL);
    oled.roundRect(100, 45, 122, 50, OLED_FILL);
    oled.roundRect(100, 55, 122, 60, OLED_FILL);
    oled.update();
    break;
    case 4:
    oled.roundRect(100, 15, 122, 20, OLED_FILL);
    oled.roundRect(100, 25, 122, 30, OLED_FILL);
    oled.roundRect(100, 35, 122, 40, OLED_FILL);
    oled.roundRect(100, 45, 122, 50, OLED_FILL);
    oled.roundRect(100, 55, 122, 60, OLED_FILL);
    oled.update();
    break;

  default:
  
    break;
 // }
  //timerOled = millis();
  }
}

void ledLevel()
{

  if(digitalRead(first) == 0){
    level = 1;
    if (digitalRead(seconde) == 0)
    {
      level = 2;
      if (digitalRead(third) == 0)
           {
             level = 3;
              if (digitalRead(fourth) == 0)
                {
                 level = 4;

                }
          }
           
      }
          
    }

   else level=0;   
   
 }
void kettelAnimation() 
{
  ds.requestTemperatures(); 
  //Serial.println(ds.getTempCByIndex(0));
  // if (flag)
  // {
     temp = ds.getTempCByIndex(0);
  // }
  // else
  // {
  //   temp = boilSet;
  // }
    if(millis() - timerTemp >= 500){
    strip.clear();
     for(int i = 0; i <= level; i++)
      {
   // Serial.println(i); 
      //Serial.println(level);
      if (temp <= 25){
        //Serial.println(i);
        strip.setPixelColor(i, strip.Color(0,0,temp * 50));
        strip.show();
        
      } 
      if (temp >= 26 && temp <= 30){
        strip.setPixelColor(i, strip.Color(temp * 50,temp * 10,0));
        strip.show();
      } 
      if (31 <= temp){
        strip.setPixelColor(i, strip.Color(temp * 100,0,0));
        strip.show();
      } 
       
      timerTemp = millis();
  }
 
  }
}

void setup() {

    //  Serial.begin(9600);
    //  pinMode(2, OUTPUT);
    //  BLEDevice::init("");
    //  esp_scan = BLEDevice::getScan();
    //  esp_scan->setActiveScan(true); //активное сканирование потребляет больше энергии, но быстрее результаты
    //  esp_scan->setInterval(200); // интервал для сканирования (мсек)
    //  esp_scan->setWindow(30);  // меньше или равно значению setInterval

  pinMode(first, INPUT_PULLUP);
  pinMode(seconde, INPUT_PULLUP);
  pinMode(third, INPUT_PULLUP);
  pinMode(fourth, INPUT_PULLUP);

  pinMode(DOWN, INPUT_PULLUP);
  pinMode(OK, INPUT_PULLDOWN);
  pinMode(UP, INPUT_PULLUP);

  pinMode(boil, OUTPUT);
  Serial.begin(9600);

  ds.begin(); 

  strip.begin();
  strip.clear();

  oled.init();              // инициализация
  Wire.setClock(400000L);
  oled.clear();
  oled.update();  

 Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}
 
void loop() {

 if (!client.connected()) {
    reconnect();
  }
  client.loop();

ledLevel();
kettelAnimation();
oledControl();

 String levelS = String(level);
 String tempS = String(temp);
 client.publish("Kout",levelS.c_str());
 client.publish("temp",tempS.c_str());

  if (level > 0)
  {
    attachInterrupt(digitalPinToInterrupt(DOWN), boilSetUp,CHANGE);
    attachInterrupt(digitalPinToInterrupt(OK), boilFunc,CHANGE);
    attachInterrupt(digitalPinToInterrupt(UP), boilSetUp,CHANGE);
    
    boilSetHold();
  }

  if (boilSet >= 100)
  {
    boilSet = 100;
  }
  if (boilSet <= 38)
  {
    boilSet = 38;
  }

boilON();
}