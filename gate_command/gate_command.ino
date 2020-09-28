#include <ESP32Servo.h> 
#include <WiFi.h>
#include <WiFiUdp.h>
const char*  ssid = "LuqmanZGeh";
const char*  password = "qwertyuiop";
unsigned int localPort_ = 9999;
WiFiUDP udp;
IPAddress ipServer(192, 168, 43, 169); 
char packetBuffer[2]; 
static const int servoPin1 = 32;
static const int servoPin2 = 33;
static const int servoPin3 = 25;
static const int servoPin4 = 26;

static const int m_1 = 17;
static const int m_2 = 14;
int servos_angle[4];int angle;
bool manual,schedual,intelligence,intellegence_work_done;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
void setup() {
    Serial.begin(115200);
    wifi_def(); 
    pinMode(m_1,OUTPUT);
    pinMode(m_2,OUTPUT);
    servo1.attach(servoPin1);
    servo2.attach(servoPin2);
    servo3.attach(servoPin3);
    servo4.attach(servoPin4);
    all_off();
}

void loop() {
    Udp_Packet_Check();
}


void Udp_Packet_Check(){
  if(udp.parsePacket() ){    
    udp.read(packetBuffer, 2);
    Serial.println(packetBuffer);
    delay(2000);
    update_servo_angles();            
    }
    
}
void update_servo_angles(){
  if(!manual && !schedual && !intelligence){
    // mode selection
      if(packetBuffer[0]     =='m'){manual=true;source_on_other_off();}
      else if(packetBuffer[0]=='s'){schedual=true;source_on_other_off();}
      else if(packetBuffer[0]=='i'){intelligence=true;source_on_other_off();}

  }
  //  deactivate conditions
      if(packetBuffer[0]     =='y'){Serial.println("-> Manual Mode Deactivated");manual=false;all_off();}
      else if(packetBuffer[0]=='u'){Serial.println("-> Schedual Mode Deactivated");schedual=false;all_off();}
      else if(packetBuffer[0]=='o'){Serial.println("-> Intellegent Mode Deactivated");intelligence=false;all_off();}

  if     (manual)      {Serial.println("Mode -> Working Manually");manual_work();}
  else if(schedual)    {Serial.println("Mode -> Working Schedually"); schedual_work();}
  else if(intelligence){Serial.println("Mode -> Working Intelligently");if(!intellegence_work_done){intellengce_work();}}

for( int i=0;i<4;i++){// print angles on serial monitor
    Serial.print(servos_angle[i]);Serial.print(" ");}
    Serial.println();


}

void schedual_work(){ // validate its working-> remove some logic errors
    intellengce_work();
    // after 24 hours call intellegence
    if(intellegence_work_done){
    unsigned long time_stamp=millis();
    while(millis() - time_stamp >=86400000){//hold for 24 hours
    }intellegence_work_done=false;
    intellengce_work();
    }

}
void manual_work(){
  
  // we only open source and "ONE" field gate to supply full natural pressure for ONE field

    if(packetBuffer[0]=='q'){
      // Deactivate 
    servos_angle[0]=100;// Open Source Gate on Manual Control activation and close all other
    servos_angle[1]=100;servos_angle[2]=100;servos_angle[3]=100;  
    
    servo1.write(servos_angle[0]);
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]);
    delay(10000);
    digitalWrite(m_1,LOW); digitalWrite(m_2,LOW);
    servo4.write(servos_angle[3]); 

    }

    if(packetBuffer[0]=='w'){
       // Open Gate One
    servos_angle[1]=0;
    servos_angle[2]=100;servos_angle[3]=100;  
    digitalWrite(m_1,LOW); digitalWrite(m_2,LOW);
    servo1.write(servos_angle[0]);
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]);
    servo4.write(servos_angle[3]); 
    }

    if(packetBuffer[0]=='e'){
       // Open Gate Two
    servos_angle[1]=100;
    servos_angle[2]=0;servos_angle[3]=100;  
    digitalWrite(m_1,LOW); digitalWrite(m_2,LOW);
    servo1.write(servos_angle[0]);
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]);
    servo4.write(servos_angle[3]); 
    }

    if(packetBuffer[0]=='r'){
       // Open Gate Three
    servos_angle[1]=100;
    servos_angle[2]=100;servos_angle[3]=0;  
    digitalWrite(m_1,HIGH); digitalWrite(m_2,LOW);
    servo1.write(servos_angle[0]);
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]);
    servo4.write(servos_angle[3]); 
    }
    if(packetBuffer[0]=='j'){
       // Open Gate Three inverted motor
    servos_angle[1]=100;
    servos_angle[2]=100;servos_angle[3]=0;  
    digitalWrite(m_1,LOW); digitalWrite(m_2,HIGH);
    servo1.write(servos_angle[0]);
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]);
    servo4.write(servos_angle[3]); 
    }
}

void intellengce_work(){

  if(packetBuffer[0]=='a'){
    // Open Gate One
    servos_angle[1]=0;digitalWrite(m_1,LOW);digitalWrite(m_2,LOW);
    servos_angle[2]=100;servos_angle[3]=100;
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]);
    servo4.write(servos_angle[3]); }
  if(packetBuffer[0]=='b'){
      // Open Gate Two
    servos_angle[1]=100;digitalWrite(m_1,LOW);digitalWrite(m_2,LOW);
    servos_angle[2]=0;servos_angle[3]=100;
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]);
    servo4.write(servos_angle[3]);     }

  
  if(packetBuffer[0]=='c'){
      // Open Gate Three + Motor
    servos_angle[1]=100;digitalWrite(m_1,HIGH);digitalWrite(m_2,LOW);
    servos_angle[2]=100;servos_angle[3]=0;
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]);
    servo4.write(servos_angle[3]);    }

  if(packetBuffer[0]=='d'){
      //Close all Gates Irrigation is Completed
    servos_angle[0]=100;
    servos_angle[1]=100;
    servos_angle[2]=100;servos_angle[3]=100;
    servo1.write(servos_angle[0]);
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]); 
    delay(10000); // Keep pump on to extract Water Out of Khalla
    digitalWrite(m_1,LOW);digitalWrite(m_2,LOW); 
    servo4.write(servos_angle[3]);  
    intellegence_work_done=true;
    }
  
}

void source_on_other_off(){
    servos_angle[0]=0;// Open Source Gate on Manual Control activation and close all other
    servos_angle[1]=100;servos_angle[2]=100;servos_angle[3]=100;  
    digitalWrite(m_1,LOW); digitalWrite(m_2,LOW);
    servo1.write(servos_angle[0]);
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]);
    servo4.write(servos_angle[3]); 

}
void all_off(){
    servos_angle[0]=100;// Open Source Gate on Manual Control activation and close all other
    servos_angle[1]=100;servos_angle[2]=100;servos_angle[3]=100;  
    digitalWrite(m_1,LOW); digitalWrite(m_2,LOW);
    servo1.write(servos_angle[0]);
    servo2.write(servos_angle[1]);
    servo3.write(servos_angle[2]);
    servo4.write(servos_angle[3]); 

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
