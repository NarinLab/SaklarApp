/**
 * Kode program untuk mengontrol lampu rumah melalui WiFi Android
 * menggunakan antarmuka lab.narin.co.id/saklar
 * Untuk informasi penggunaan silahkan masuk ke: tutorkeren.com/artikel/kode-program-untuk-kontrol-lampu-rumah-android-wifi.htm
 * Perhatian: Ini hanya untuk pembelajaran, tidak ada garansi dan penulis tidak bertanggung jawab atas kerusakaan atau kerugian yang
 * ditimbulkan berkaitan dengan segala informasi ini.
 */
  
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
//Tergantung jenis relai,
//Pada contoh menggunakan trigger LOW
//Jika punya kalian menggunakan HIGH tukar nilai dibawah
#define MATI HIGH
#define HIDUP LOW
  
/***** PENYESUAIAN DIMULAI DARI SINI *****/
//Start Inisialisasi perangkat
//GLOBAL
#define LOCATION "Tower Air"
  
//DEVICE 1
const int DEV_1_ID = 1;
#define DEV_1_NAME "Lampu Taman 1"
#define DEV_1_TYPE "ACTUATOR"
#define DEV_1_MODEL "RELAY"
#define DEV_1_PIN D5
#define DEV_1_WATT 80
int dev_1_state = 0;
unsigned long dev_1_uptime = 0;
unsigned long dev_1_totaluptime = 0;
  
//DEVICE 2
const int DEV_2_ID = 2;
#define DEV_2_NAME "Mesin Air"
#define DEV_2_TYPE "ACTUATOR"
#define DEV_2_MODEL "RELAY"
#define DEV_2_PIN D6
#define DEV_2_WATT 750
int dev_2_state = 0;
unsigned long dev_2_uptime = 0;
unsigned long dev_2_totaluptime = 0;
 
//DEVICE 3
const int DEV_3_ID = 3;
#define DEV_3_NAME "Lampu Taman 3"
#define DEV_3_TYPE "ACTUATOR"
#define DEV_3_MODEL "RELAY"
#define DEV_3_PIN D7
#define DEV_3_WATT 80
int dev_3_state = 0;
unsigned long dev_3_uptime = 0;
unsigned long dev_3_totaluptime = 0;
//END Inisialisasi perangkat
 
//PENTING harus diset berapa jumlah perangkat pada Node ini
const String dev_count = "3";
 
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
  root.printTo(Serial);
  Serial.println();
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
  root.printTo(Serial);
  Serial.println();
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
  root.printTo(Serial);
  Serial.println();
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
  devices_list(id);
}
/***** PENYESUAIAN BERAKHIR DARI SINI *****/
  
  
//Fungsi untuk handler root (<a href="<a href="http://alamatip/">http://alamatip/</a>"><a href="http://alamatip/</a>">http://alamatip/</a></a>)
void handle_root(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send ( 200, "text/plain", "done" );
}
  
//Fungsi untuk handler POST /cgi (<a href="<a href="http://alamatip/cgi">http://alamatip/cgi</a>"><a href="http://alamatip/cgi</a>">http://alamatip/cgi</a></a> POST)
void post_handle_cgi(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
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
  //Matikan dulu semua perangkat untuk mencegah hal-hal yg tidak diinginkan
  digitalWrite(DEV_1_PIN, HIGH);
  digitalWrite(DEV_2_PIN, HIGH);
  digitalWrite(DEV_3_PIN, HIGH);
  /***** PENYESUAIAN BERAKHIR DARI SINI *****/
    
  Serial.begin(115200);
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
 server.handleClient();
}
