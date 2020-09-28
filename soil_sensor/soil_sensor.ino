
#include <WiFi.h>
#include <WiFiUdp.h>

const char*  ssid = "LuqmanZGeh";
const char*  password = "qwertyuiop";
const char * second_esp_IP = "192.168.43.169";
unsigned int localPort_ = 9999;
char sending_msg;
bool data_send;
WiFiUDP udp;
IPAddress ipServer(192, 168, 43, 229); 


const byte sensors[] = {33,32,35};
int sensors_values[3];

void setup() {
Serial.begin(115200);  
wifi_def(); 
pinMode(sensors[0],INPUT);
pinMode(sensors[1],INPUT);
pinMode(sensors[2],INPUT);

}

void loop() {
  // add code for a-> send data , b-> stop sending data.
   
   if(udp.parsePacket() ){ 
  udp.beginPacket(second_esp_IP, localPort_);
  sending_msg='x';
  udp.write((uint8_t)sending_msg);
  udp.endPacket();
    data_send=true;
    delay(7000);
    
                        }
   if(data_send){// send data after every 4 secs if you recieved a message
   static unsigned long lastTimeItHappened = 0; 
   if (millis() - lastTimeItHappened >= 4000) { 
    lastTimeItHappened = millis();
  send_values();}

   }
   for (int i =0;i<3;i++){
    sensors_values[i] = analogRead(sensors[i]);
      Serial.print(sensors_values[i]);Serial.print(" ");
  }Serial.println();               
   
delay(200);



}
void send_values(){
  
   for(int i =0;i<3;i++){
  sensors_values[i] = analogRead(sensors[i]);
                        }
  udp.beginPacket(second_esp_IP, localPort_);
  if(sensors_values[0] > 3600 && sensors_values[0] > 3600 && sensors_values[2] > 3600){sending_msg='a'; udp.write((uint8_t)sending_msg); }
  if(sensors_values[0] < 3600 && sensors_values[1] > 3600 && sensors_values[2] > 3600){sending_msg='b'; udp.write((uint8_t)sending_msg); }
  if(sensors_values[0] < 3600 && sensors_values[1] < 3600 && sensors_values[2] > 3600){sending_msg='c'; udp.write((uint8_t)sending_msg); }
  if(sensors_values[0] < 3600 && sensors_values[1] < 3600 && sensors_values[2] < 3600){sending_msg='d'; udp.write((uint8_t)sending_msg); }
  udp.endPacket();
  
}
void wifi_def(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {          // Exit only when connected
    delay(500);
    Serial.print(".");}
  
  Serial.print("\nConnected to ");Serial.println(ssid);
  Serial.print("IP address: ");Serial.println(WiFi.localIP());
  delay(3000);
  udp.begin(localPort_);                            // Begin the udp communication
}
