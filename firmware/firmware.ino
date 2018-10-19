/**
 * Kode program untuk mengontrol lampu rumah melalui WiFi Android
 * menggunakan antarmuka lab.narin.co.id/saklar
 * Untuk informasi penggunaan silahkan masuk ke: tutorkeren.com/artikel/kode-program-untuk-kontrol-lampu-rumah-android-wifi.htm
 * Perhatian: Ini hanya untuk pembelajaran, tidak ada garansi dan penulis tidak bertanggung jawab atas kerusakaan atau kerugian yang
 * ditimbulkan berkaitan dengan segala informasi ini.
 */
#include <ArduinoOTA.h>  
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
  
//Menggunakan DHCP, tidak perlu setting IP 
//const IPAddress WIFI_IP(192, 168, 0, 2);
//const IPAddress WIFI_GATEWAY(192, 168, 0, 1);
//const IPAddress WIFI_SUBNET(255, 255, 255, 0);
//const IPAddress WIFI_DNS_1(192, 168, 0, 1);
//const IPAddress WIFI_DNS_2(8, 8, 8, 8);
const char* WIFI_SSID     = "AndroidAP";
const char* WIFI_PASSWORD = "ingattuhan";
  
#define PAYLOAD_SIZE 256
unsigned long STARTER_DURATION = 3000;
unsigned long STARTER_TRIGER_TIME = 0;
//Tergantung jenis relai,
//Pada contoh menggunakan trigger LOW
//Jika punya kalian menggunakan HIGH tukar nilai dibawah
#define MATI HIGH
#define HIDUP LOW
  
/***** PENYESUAIAN DIMULAI DARI SINI *****/
//Start Inisialisasi perangkat
//GLOBAL
#define LOCATION "Home Sweet Home"
  
//DEVICE 1
const int DEV_1_ID = 1;
#define DEV_1_NAME "Channel 1"
#define DEV_1_TYPE "ACTUATOR"
#define DEV_1_MODEL "RELAY"
#define DEV_1_PIN D0
#define DEV_1_WATT 80
int dev_1_state = 0;
unsigned long dev_1_uptime = 0;
unsigned long dev_1_totaluptime = 0;
  
//DEVICE 2
const int DEV_2_ID = 2;
#define DEV_2_NAME "Channel 2"
#define DEV_2_TYPE "ACTUATOR"
#define DEV_2_MODEL "RELAY"
#define DEV_2_PIN D1
#define DEV_2_WATT 80
int dev_2_state = 0;
unsigned long dev_2_uptime = 0;
unsigned long dev_2_totaluptime = 0;
 
//DEVICE 3
const int DEV_3_ID = 3;
#define DEV_3_NAME "Channel 3"
#define DEV_3_TYPE "ACTUATOR"
#define DEV_3_MODEL "RELAY"
#define DEV_3_PIN D2
#define DEV_3_WATT 80
int dev_3_state = 0;
unsigned long dev_3_uptime = 0;
unsigned long dev_3_totaluptime = 0;

//DEVICE 4
const int DEV_4_ID = 4;
#define DEV_4_NAME "Channel 4"
#define DEV_4_TYPE "ACTUATOR"
#define DEV_4_MODEL "RELAY"
#define DEV_4_PIN D3
#define DEV_4_WATT 80
int dev_4_state = 0;
unsigned long dev_4_uptime = 0;
unsigned long dev_4_totaluptime = 0;

//DEVICE 5
const int DEV_5_ID = 5;
#define DEV_5_NAME "Channel 5"
#define DEV_5_TYPE "ACTUATOR"
#define DEV_5_MODEL "RELAY"
#define DEV_5_PIN D4
#define DEV_5_WATT 80
int dev_5_state = 0;
unsigned long dev_5_uptime = 0;
unsigned long dev_5_totaluptime = 0;

//DEVICE 6
const int DEV_6_ID = 6;
#define DEV_6_NAME "Channel 6"
#define DEV_6_TYPE "ACTUATOR"
#define DEV_6_MODEL "RELAY"
#define DEV_6_PIN D5
#define DEV_6_WATT 80
int dev_6_state = 0;
unsigned long dev_6_uptime = 0;
unsigned long dev_6_totaluptime = 0;

//DEVICE 7
const int DEV_7_ID = 7;
#define DEV_7_NAME "Channel 7"
#define DEV_7_TYPE "ACTUATOR"
#define DEV_7_MODEL "RELAY"
#define DEV_7_PIN D6
#define DEV_7_WATT 80
int dev_7_state = 0;
unsigned long dev_7_uptime = 0;
unsigned long dev_7_totaluptime = 0;

//DEVICE 8
const int DEV_8_ID = 8;
#define DEV_8_NAME "Channel 8"
#define DEV_8_TYPE "ACTUATOR"
#define DEV_8_MODEL "STARTER"
#define DEV_8_PIN D7
#define DEV_8_WATT 80
int dev_8_state = 0;
unsigned long dev_8_uptime = 0;
unsigned long dev_8_totaluptime = 0;

//DEVICE 9
const int DEV_9_ID = 9;
#define DEV_9_NAME "Channel 9"
#define DEV_9_TYPE "ACTUATOR"
#define DEV_9_MODEL "RELAY"
#define DEV_9_PIN D8
#define DEV_9_WATT 80
int dev_9_state = 0;
unsigned long dev_9_uptime = 0;
unsigned long dev_9_totaluptime = 0;
//END Inisialisasi perangkat
 
//PENTING harus diset berapa jumlah perangkat pada Node ini
const String dev_count = "9";
 
ESP8266WebServer server ( 80 );
  
//Fungsi get data Dev 1
void list_dev_1(){
  StaticJsonBuffer<PAYLOAD_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //DEFINISI dev1
  root["id"] = DEV_1_ID;
  root["ip"] = WiFi.localIP().toString();
  root["ssid"] = WiFi.SSID();
  root["rssi"] = WiFi.RSSI();
  root["name"] = DEV_1_NAME;
  root["loc"] = LOCATION;
  root["type"] = DEV_1_TYPE;
  root["model"] = DEV_1_MODEL;
  root["pin"] = DEV_1_PIN;
  root["watt"] = DEV_1_WATT;
  root["state"] = dev_1_state;
  if(dev_1_state){
    root["uptime"] = millis() - dev_1_uptime + dev_1_totaluptime;
  }
  else{
    root["uptime"] = dev_1_totaluptime;
  }
  char payload[PAYLOAD_SIZE];
  root.printTo(payload, sizeof(payload));
  server.send(200, "application/json", payload);
}
  
//Fungsi get data Dev 2
void list_dev_2(){
  StaticJsonBuffer<PAYLOAD_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();  
  //DEFINISI dev2
  root["id"] = DEV_2_ID;
  root["ip"] = WiFi.localIP().toString();
  root["ssid"] = WiFi.SSID();
  root["rssi"] = WiFi.RSSI();
  root["name"] = DEV_2_NAME;
  root["loc"] = LOCATION;
  root["type"] = DEV_2_TYPE;
  root["model"] = DEV_2_MODEL;
  root["pin"] = DEV_2_PIN;
  root["watt"] = DEV_2_WATT;
  root["state"] = dev_2_state;
  if(dev_2_state){
    root["uptime"] = millis() - dev_2_uptime + dev_2_totaluptime;
  }
  else{
    root["uptime"] = dev_2_totaluptime;
  }
  
  char payload[PAYLOAD_SIZE];
  root.printTo(payload, sizeof(payload));
  server.send(200, "application/json", payload);
}
 
//Fungsi get data Dev 3
void list_dev_3(){
  StaticJsonBuffer<PAYLOAD_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //DEFINISI dev3
  root["id"] = DEV_3_ID;
  root["ip"] = WiFi.localIP().toString();
  root["ssid"] = WiFi.SSID();
  root["rssi"] = WiFi.RSSI();
  root["name"] = DEV_3_NAME;
  root["loc"] = LOCATION;
  root["type"] = DEV_3_TYPE;
  root["model"] = DEV_3_MODEL;
  root["pin"] = DEV_3_PIN;
  root["watt"] = DEV_3_WATT;
  root["state"] = dev_3_state;
  if(dev_3_state){
    root["uptime"] = millis() - dev_3_uptime + dev_3_totaluptime;
  }
  else{
    root["uptime"] = dev_3_totaluptime;
  }
  char payload[PAYLOAD_SIZE];
  root.printTo(payload, sizeof(payload));
  server.send(200, "application/json", payload);
}

//Fungsi get data Dev 4
void list_dev_4(){
  StaticJsonBuffer<PAYLOAD_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //DEFINISI dev4
  root["id"] = DEV_4_ID;
  root["ip"] = WiFi.localIP().toString();
  root["ssid"] = WiFi.SSID();
  root["rssi"] = WiFi.RSSI();
  root["name"] = DEV_4_NAME;
  root["loc"] = LOCATION;
  root["type"] = DEV_4_TYPE;
  root["model"] = DEV_4_MODEL;
  root["pin"] = DEV_4_PIN;
  root["watt"] = DEV_4_WATT;
  root["state"] = dev_4_state;
  if(dev_4_state){
    root["uptime"] = millis() - dev_4_uptime + dev_4_totaluptime;
  }
  else{
    root["uptime"] = dev_4_totaluptime;
  }
  char payload[PAYLOAD_SIZE];
  root.printTo(payload, sizeof(payload));
  server.send(200, "application/json", payload);
  

}

//Fungsi get data Dev 5
void list_dev_5(){
  StaticJsonBuffer<PAYLOAD_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //DEFINISI dev5
  root["id"] = DEV_5_ID;
  root["ip"] = WiFi.localIP().toString();
  root["ssid"] = WiFi.SSID();
  root["rssi"] = WiFi.RSSI();
  root["name"] = DEV_5_NAME;
  root["loc"] = LOCATION;
  root["type"] = DEV_5_TYPE;
  root["model"] = DEV_5_MODEL;
  root["pin"] = DEV_5_PIN;
  root["watt"] = DEV_5_WATT;
  root["state"] = dev_5_state;
  if(dev_5_state){
    root["uptime"] = millis() - dev_5_uptime + dev_5_totaluptime;
  }
  else{
    root["uptime"] = dev_5_totaluptime;
  }
  char payload[PAYLOAD_SIZE];
  root.printTo(payload, sizeof(payload));
  server.send(200, "application/json", payload);
}

//Fungsi get data Dev 6
void list_dev_6(){
  StaticJsonBuffer<PAYLOAD_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //DEFINISI dev6
  root["id"] = DEV_6_ID;
  root["ip"] = WiFi.localIP().toString();
  root["ssid"] = WiFi.SSID();
  root["rssi"] = WiFi.RSSI();
  root["name"] = DEV_6_NAME;
  root["loc"] = LOCATION;
  root["type"] = DEV_6_TYPE;
  root["model"] = DEV_6_MODEL;
  root["pin"] = DEV_6_PIN;
  root["watt"] = DEV_6_WATT;
  root["state"] = dev_6_state;
  if(dev_6_state){
    root["uptime"] = millis() - dev_6_uptime + dev_6_totaluptime;
  }
  else{
    root["uptime"] = dev_6_totaluptime;
  }
  char payload[PAYLOAD_SIZE];
  root.printTo(payload, sizeof(payload));
  server.send(200, "application/json", payload);
}

//Fungsi get data Dev 7
void list_dev_7(){
  StaticJsonBuffer<PAYLOAD_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //DEFINISI dev7
  root["id"] = DEV_7_ID;
  root["ip"] = WiFi.localIP().toString();
  root["ssid"] = WiFi.SSID();
  root["rssi"] = WiFi.RSSI();
  root["name"] = DEV_7_NAME;
  root["loc"] = LOCATION;
  root["type"] = DEV_7_TYPE;
  root["model"] = DEV_7_MODEL;
  root["pin"] = DEV_7_PIN;
  root["watt"] = DEV_7_WATT;
  root["state"] = dev_7_state;
  if(dev_7_state){
    root["uptime"] = millis() - dev_7_uptime + dev_7_totaluptime;
  }
  else{
    root["uptime"] = dev_7_totaluptime;
  }
  char payload[PAYLOAD_SIZE];
  root.printTo(payload, sizeof(payload));
  server.send(200, "application/json", payload);
}

//Fungsi get data Dev 8
void list_dev_8(){
  StaticJsonBuffer<PAYLOAD_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //DEFINISI dev8
  root["id"] = DEV_8_ID;
  root["ip"] = WiFi.localIP().toString();
  root["ssid"] = WiFi.SSID();
  root["rssi"] = WiFi.RSSI();
  root["name"] = DEV_8_NAME;
  root["loc"] = LOCATION;
  root["type"] = DEV_8_TYPE;
  root["model"] = DEV_8_MODEL;
  root["pin"] = DEV_8_PIN;
  root["watt"] = DEV_8_WATT;
  root["state"] = dev_8_state;
  if(dev_8_state){
    root["uptime"] = millis() - dev_8_uptime + dev_8_totaluptime;
  }
  else{
    root["uptime"] = dev_8_totaluptime;
  }
  char payload[PAYLOAD_SIZE];
  root.printTo(payload, sizeof(payload));
  server.send(200, "application/json", payload);
}

//Fungsi get data Dev 9
void list_dev_9(){
  StaticJsonBuffer<PAYLOAD_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //DEFINISI dev9
  root["id"] = DEV_9_ID;
  root["ip"] = WiFi.localIP().toString();
  root["ssid"] = WiFi.SSID();
  root["rssi"] = WiFi.RSSI();
  root["name"] = DEV_9_NAME;
  root["loc"] = LOCATION;
  root["type"] = DEV_9_TYPE;
  root["model"] = DEV_9_MODEL;
  root["pin"] = DEV_9_PIN;
  root["watt"] = DEV_9_WATT;
  root["state"] = dev_9_state;
  if(dev_9_state){
    root["uptime"] = millis() - dev_9_uptime + dev_9_totaluptime;
  }
  else{
    root["uptime"] = dev_9_totaluptime;
  }
  char payload[PAYLOAD_SIZE];
  root.printTo(payload, sizeof(payload));
  server.send(200, "application/json", payload);
}
  
//Fungsi untuk handler deviceList
void devices_list(int id){
  switch(id){
    case 1:
      list_dev_1();
      break;
    case 2:
      list_dev_2();
      break;
    case 3:
      list_dev_3();
      break;
    case 4:
      list_dev_4();
      break;
    case 5:
      list_dev_5();
      break;
    case 6:
      list_dev_6();
      break;
    case 7:
      list_dev_7();
      break;            
    case 8:
      list_dev_8();
      break;
    case 9:
      list_dev_9();
      break;            
    default:
      list_dev_1();
  }
}
//Fungsi untuk mengendalikan lampu (type: RELAY)
void control_relay(int id, int value){
  if(id == DEV_1_ID){
    switch(value){
      case 1: {
        digitalWrite(DEV_1_PIN, HIDUP);
        if(!dev_1_state){
          dev_1_state = value;
          dev_1_uptime = millis();
        }
        break;
      }
      case 0: {
        digitalWrite(DEV_1_PIN, MATI);
        dev_1_state = value;
        dev_1_totaluptime = dev_1_totaluptime + millis() - dev_1_uptime;
        break;
      }
    }
  }
  else if(id == DEV_2_ID){
    switch(value){
      case 1: {
        digitalWrite(DEV_2_PIN, HIDUP);
        if(!dev_2_state){
          dev_2_state = value;
          dev_2_uptime = millis();
        }
        break;
      }
      case 0: {
        digitalWrite(DEV_2_PIN, MATI);
        dev_2_state = value;
        dev_2_totaluptime = dev_2_totaluptime + millis() - dev_2_uptime;
        break;
      }
    }
  }
  else if(id == DEV_3_ID){
    switch(value){
      case 1: {
        digitalWrite(DEV_3_PIN, HIDUP);
        if(!dev_3_state){
          dev_3_state = value;
          dev_3_uptime = millis();
        }
        break;
      }
      case 0: {
        digitalWrite(DEV_3_PIN, MATI);
        dev_3_state = value;
        dev_3_totaluptime = dev_3_totaluptime + millis() - dev_3_uptime;
        break;
      }
    }
  }
  else if(id == DEV_4_ID){
    switch(value){
      case 1: {
        digitalWrite(DEV_4_PIN, HIDUP);
        if(!dev_4_state){
          dev_4_state = value;
          dev_4_uptime = millis();
        }
        break;
      }
      case 0: {
        digitalWrite(DEV_4_PIN, MATI);
        dev_4_state = value;
        dev_4_totaluptime = dev_4_totaluptime + millis() - dev_4_uptime;
        break;
      }
    }
  }
  else if(id == DEV_5_ID){
    switch(value){
      case 1: {
        digitalWrite(DEV_5_PIN, HIDUP);
        if(!dev_5_state){
          dev_5_state = value;
          dev_5_uptime = millis();
        }
        break;
      }
      case 0: {
        digitalWrite(DEV_5_PIN, MATI);
        dev_5_state = value;
        dev_5_totaluptime = dev_5_totaluptime + millis() - dev_5_uptime;
        break;
      }
    }
  }    
  else if(id == DEV_6_ID){
    switch(value){
      case 1: {
        digitalWrite(DEV_6_PIN, HIDUP);
        if(!dev_6_state){
          dev_6_state = value;
          dev_6_uptime = millis();
        }
        break;
      }
      case 0: {
        digitalWrite(DEV_6_PIN, MATI);
        dev_6_state = value;
        dev_6_totaluptime = dev_6_totaluptime + millis() - dev_6_uptime;
        break;
      }
    }
  }
  else if(id == DEV_7_ID){
    switch(value){
      case 1: {
        digitalWrite(DEV_7_PIN, HIDUP);
        if(!dev_7_state){
          dev_7_state = value;
          dev_7_uptime = millis();
        }
        break;
      }
      case 0: {
        digitalWrite(DEV_7_PIN, MATI);
        dev_7_state = value;
        dev_7_totaluptime = dev_7_totaluptime + millis() - dev_7_uptime;
        break;
      }
    }
  }
  else if(id == DEV_8_ID){
    switch(value){
      case 1: {
        digitalWrite(DEV_8_PIN, HIDUP);
        if(!dev_8_state){
          dev_8_state = value;
          dev_8_uptime = millis();
        }
        break;
      }
      case 0: {
        digitalWrite(DEV_8_PIN, MATI);
        dev_8_state = value;
        dev_8_totaluptime = dev_8_totaluptime + millis() - dev_8_uptime;
        break;
      }
    }
  }      
  else if(id == DEV_9_ID){
    switch(value){
      case 1: {
        digitalWrite(DEV_9_PIN, HIDUP);
        if(!dev_9_state){
          dev_9_state = value;
          dev_9_uptime = millis();
        }
        break;
      }
      case 0: {
        digitalWrite(DEV_9_PIN, MATI);
        dev_9_state = value;
        dev_9_totaluptime = dev_9_totaluptime + millis() - dev_9_uptime;
        break;
      }
    }
  }  
  devices_list(id);
}
/***** PENYESUAIAN BERAKHIR DARI SINI *****/
  
  
//Fungsi untuk handler root (<a href="<a href="http://alamatip/">http://alamatip/</a>"><a href="http://alamatip/</a>">http://alamatip/</a></a>)
void handle_root(){
  //server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Max-Age", "10000");
  server.sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "*");
  server.send ( 200, "text/plain", "done" );
}
  
//Fungsi untuk handler POST /cgi (<a href="<a href="http://alamatip/cgi">http://alamatip/cgi</a>"><a href="http://alamatip/cgi</a>">http://alamatip/cgi</a></a> POST)
void post_handle_cgi(){
        //server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Max-Age", "10000");
        server.sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "*");
  if(server.args() > 0){
    /**for(int i = 0; i < server.args(); i++){
      Serial.print("Name: "); Serial.println(server.argName(i));
      Serial.print("Value: "); Serial.println(server.arg(i));
    }**/
    if(server.arg(0) == "get"){
      StaticJsonBuffer<PAYLOAD_SIZE> msg;
      JsonObject& data = msg.parseObject(server.arg(1));
      if(!data.success()){
        return;
      }
      if(data["func"] == "devices_list"){
        devices_list(data["id"]);
      }
      else if(data["func"] == "index"){
        server.send(200, "text/plain", dev_count);
      }
    }
    else if(server.arg(0) == "set"){
      StaticJsonBuffer<PAYLOAD_SIZE> msg;
      JsonObject& data = msg.parseObject(server.arg(1));
      if(!data.success()){
        return;
      }
      if(data["func"] == "control_relay"){
        control_relay(data["id"], data["value"]);
      }
    }
    //Serial.print(server.arg(0));
    //Serial.print(": ");
    //Serial.println(server.arg(1));
  }
}
  
void setup() {
  /***** PENYESUAIAN DIMULAI DARI SINI *****/
  //PIN MODE
  pinMode(DEV_1_PIN, OUTPUT);
  pinMode(DEV_2_PIN, OUTPUT);
  pinMode(DEV_3_PIN, OUTPUT);
  pinMode(DEV_4_PIN, OUTPUT);
  pinMode(DEV_5_PIN, OUTPUT);
  pinMode(DEV_6_PIN, OUTPUT);
  pinMode(DEV_7_PIN, OUTPUT);
  pinMode(DEV_8_PIN, OUTPUT);
  pinMode(DEV_9_PIN, OUTPUT);
  //Matikan dulu semua perangkat untuk mencegah hal-hal yg tidak diinginkan
  digitalWrite(DEV_1_PIN, HIGH);
  digitalWrite(DEV_2_PIN, HIGH);
  digitalWrite(DEV_3_PIN, HIGH);
  digitalWrite(DEV_4_PIN, HIGH);
  digitalWrite(DEV_5_PIN, HIGH);
  digitalWrite(DEV_6_PIN, HIGH);
  digitalWrite(DEV_7_PIN, HIGH);
  digitalWrite(DEV_8_PIN, HIGH);
  digitalWrite(DEV_9_PIN, HIGH);
  /***** PENYESUAIAN BERAKHIR DARI SINI *****/
    
  //Serial.begin(115200);
  delay(10);
    
  WiFi.mode(WIFI_STA);
  //Versi DHCP, tidak menggunakan config().
  //WiFi.config(WIFI_IP, WIFI_GATEWAY, WIFI_SUBNET, WIFI_DNS_1, WIFI_DNS_2);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
  }
  //Serial.println("Connected");
  //Web Server Hanlder
  server.on( "/", handle_root);
  server.on( "/cgi", HTTP_POST, post_handle_cgi);
  server.begin();
}
  
void loop() {
 //Kode lanjutan akan ditulis disini.
 ArduinoOTA.handle();
 server.handleClient();
 unsigned long now = millis();
 //Khusus Channel Starter
 if((now - STARTER_TRIGER_TIME) > STARTER_DURATION){
    digitalWrite(DEV_8_PIN, MATI);
    dev_8_state = 0;
    dev_8_totaluptime = dev_8_totaluptime + millis() - dev_8_uptime;
    STARTER_TRIGER_TIME = now;
 }
}
