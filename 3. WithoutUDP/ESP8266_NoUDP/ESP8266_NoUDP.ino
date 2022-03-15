#include <ESP8266WiFi.h>
#include <SocketIoClient.h> //WebSocket+SocketIoClient Library 둘 다 설치
#include <ArduinoJson.h> //Ver 6.0 이상 사용

const char* ssid     = "TH2"; // WIFI SSID
const char* password = "cidr4558"; // WIFI Password
const char* hostServer = " 143.248.109.186"; // Host Server

SocketIoClient socket;

const long interval = 500; // 데이터를 보낼 주기
unsigned long previousMillis = 0; 

 
void setup() {
  Serial.begin(115200);
  delay(100);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(0, OUTPUT);

  // Socket.IO 정의
  socket.on("toarduino", event); // Event 정의
  socket.begin(hostServer);
}


void loop() {
  unsigned long currentMillis = millis();
  

  // Interval 마다 데이터를 보내는 구문
  if (currentMillis - previousMillis >= interval) {    
    previousMillis = currentMillis;
    
    DynamicJsonDocument data_to_web(1024); 
    data_to_web["button"] = (currentMillis/1000)%2;
    data_to_web["potentiometer"] = 512 + 512*sin(currentMillis/100);
  
    String jsonData;
    serializeJson(data_to_web, jsonData); // JSON to String
    
    socket.emit("fromarduino", jsonData.c_str()); // String을 보내기 위해 .c_str() 붙여줘야 함
  }
  
  socket.loop(); // 항상 필요, 빼먹을 시 작동 안됨.
}

// Event Listening from "toarduino"
void event(const char * data, size_t length) {
  DynamicJsonDocument data_from_web(1024);
  deserializeJson(data_from_web, data);

  int buttonState = data_from_web["button"];
  long potentioValue = data_from_web["potentiometer"];

//  digitalWrite(0, buttonState); // LED 작동
  analogWrite(0, potentioValue); // LED 작동
}
