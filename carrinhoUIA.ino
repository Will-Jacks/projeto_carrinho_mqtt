#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define Sentido1MotorA D2
#define Sentido2MotorA D3
#define Sentido1MotorB D5
#define Sentido2MotorB D7
const char* ssid = "mqtt";
const char* passwd = "liviaeuia";
const char* broker_topic = "WILL/teste";
const char* mqtt_server = "broker.emqx.io";
const char* espID = "CARRINHO-UIA";
const int port = 1883;
int velocidade = 150;
String str_msg;

WiFiClient wlanclient;
PubSubClient mqttClient(wlanclient);

void wifiConnect() {
  WiFi.begin(ssid,passwd);
  Serial.println ("Conectando ao WiFi");
  while(WiFi.status()!=WL_CONNECTED) {
    Serial.print (".");
    delay(300);
  }
  Serial.println("");
  Serial.print ("Conectado ao WiFi, no IP: ");
  Serial.println (WiFi.localIP());
}

void mqttConnect(){
  mqttClient.setServer (mqtt_server,port);
  mqttClient.setCallback(mqttCallback);
  
  if(mqttClient.connect (espID,NULL,NULL)) {
    Serial.println ("Conectado ao MQTT Broker.");
  } else {
    Serial.println("Conexão com o broker falhou!");
    delay(200);
  }
  mqttClient.subscribe(broker_topic);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  char message[5]={0x00};
  for (int i = 0; i < length; i++){
    message[i]=(char)payload[i]; 
    }
  message[length]=0x00;
  Serial.println (message);
  str_msg = String(message);
  recebeMensagem();
}

void recebeMensagem() {
 if (str_msg.equals ("Tras")) {
    analogWrite(Sentido1MotorA, velocidade);
    analogWrite(Sentido2MotorA, LOW);
    analogWrite(Sentido1MotorB, velocidade);
    analogWrite(Sentido2MotorB, LOW);
 }
 if (str_msg.equals ("Cima")) {
    analogWrite(Sentido1MotorA, LOW);
    analogWrite(Sentido2MotorA, velocidade);
    analogWrite(Sentido1MotorB, LOW);
    analogWrite(Sentido2MotorB, velocidade);
 }
 if(str_msg.equals ("Esquerda")) {
    analogWrite(Sentido1MotorA, LOW);
    analogWrite(Sentido2MotorA, LOW);
    analogWrite(Sentido1MotorB, LOW);
    analogWrite(Sentido2MotorB, velocidade);
 }
 if(str_msg.equals ("Direita")) {
    analogWrite(Sentido1MotorA, LOW);
    analogWrite(Sentido2MotorA, velocidade);
    analogWrite(Sentido1MotorB, LOW);
    analogWrite(Sentido2MotorB, LOW);
 }
 if(str_msg.equals("Parar")) {
    analogWrite(Sentido1MotorA, LOW);
    analogWrite(Sentido2MotorA, LOW);
    analogWrite(Sentido1MotorB, LOW);
    analogWrite(Sentido2MotorB, LOW);
 }

 if(str_msg.equals("150")) {
  velocidade = 150;
 } 
 if(str_msg.equals("255")) {
  velocidade = 255;
 }
}

void setup() {
  Serial.begin(115200);
  pinMode(Sentido1MotorA, OUTPUT);
  pinMode(Sentido2MotorA, OUTPUT);
  pinMode(Sentido1MotorB, OUTPUT);
  pinMode(Sentido2MotorB, OUTPUT);
  wifiConnect();
  mqttConnect();
}

void loop() {
  if(WiFi.status()!=WL_CONNECTED){
    wifiConnect();
  }else if(!mqttClient.connected()){
    mqttConnect();
  }
  mqttClient.loop();
}
