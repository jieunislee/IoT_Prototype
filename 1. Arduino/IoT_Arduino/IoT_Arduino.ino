#include <ArduinoJson.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

char ssid[] = "yeakyung";        
char pass[] = "cidr4558";   
unsigned int localPort = 4388;     

const long interval = 25;
unsigned long previousMillis = 0; 

int status = WL_IDLE_STATUS;
bool targetFound = false;
int targetPort;
IPAddress targetIP;

char packetBuffer[255];

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  delay(250);
  
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  
  while (status != WL_CONNECTED) {    
    status = WiFi.begin(ssid, pass);
    delay(500);
  }
  
  Serial.println("");
  Serial.println("Connected to wifi");

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  Udp.begin(localPort);  

  pinMode(9,OUTPUT);
  pinMode(10,INPUT);
}

void loop() { 
  
  StaticJsonBuffer<500> data_A;
  StaticJsonBuffer<500> data_W;
  
  JsonObject& data_to_web = data_A.createObject();
  data_to_web["button"] = digitalRead(10);  
  data_to_web["potentiometer"] = analogRead(A0);  

  int packetSize = Udp.parsePacket();
  if (packetSize) {    
    
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    if(targetFound){
      Serial.println();
      Serial.println("Recevied data: ");
      Serial.println(packetBuffer);

      JsonObject& data_from_web = data_W.parseObject(packetBuffer);
      char* button = data_from_web["button"];
      int potentiometer = data_from_web["potentiometer"]; 

      if(String(button).equals("ON")){
        analogWrite(9,potentiometer);
      }
      else if(String(button).equals("OFF")){
        analogWrite(9,0);
      }

      

    }

    if(String(packetBuffer).equals("Begin")){      
      targetFound = true;
      targetIP = Udp.remoteIP();
      targetPort = Udp.remotePort();
  
      Serial.print("Communicating with ");
      Serial.print(targetIP);
      Serial.print(", port ");
      Serial.println(targetPort);
    }    
  }

  unsigned long currentMillis = millis();

  if(targetFound){
    if (currentMillis - previousMillis >= interval) {    
      previousMillis = currentMillis;
      
      Udp.beginPacket(targetIP, targetPort);
      data_to_web.printTo(Udp);
      Udp.println();
      Udp.endPacket();
    }
  }
}
