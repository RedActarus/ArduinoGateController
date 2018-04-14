#include <SPI.h>
#include <Ethernet.h>
 
boolean statop1 = false;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 1, 177 }; // ip
byte gateway[] = { 192, 168, 1, 1 }; // router access
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //port server
int Rele = 7;
String readString;

void setup() {
  pinMode(7, OUTPUT); //gate rele
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.begin(9600);
  Serial.println("Server cancello 2.0 online, inizio della trasmissione.");
  pinMode(Rele, OUTPUT); //pin rele output
}

void loop() {
  while (Serial.available()) {
    delay(3);  
    char c = Serial.read();
    readString += c; 
  }
  if (readString.length() >0) {
    Serial.println(readString); //RXbluetooth reading
    if (readString == "on")     
    {
      digitalWrite(Rele, HIGH);
    }
    if (readString == "off")
    {
      digitalWrite(Rele, LOW);
    }
    readString=""; //clear for next reading
    } 
   // ethernet cli connection creation
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
        }
 
        //if HTTP request has ended
        if (c == '\n') {
 
          ///////////////
          Serial.println(readString); //print to serial monitor for debuging
 
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();
 
          client.println("<!DOCTYPE html><html><head><meta name='apple-mobile-web-app-capable' content='yes'><meta name='apple-mobile-web-app-status-bar-style' content='black-translucent'><link rel='stylesheet' type='text/css' href='http://homeautocss.net84.net/a.css'><TITLE>CANCELLO</TITLE></HEAD><BODY><H1>CONTROLLER CANCELLO</H1><hr><br><ul><li><p align=\"left\">CANCELLO:&nbsp;&nbsp;&nbsp;&nbsp; <a href=\"/?cancelloon\">AVVIO</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a href=\"/?cancellooff\">STOP</a></li></p></ul></BODY></HTML>");
 
          delay(1);
          //stopping client
          client.stop();
 
          ///////////////////// arduino pin controls
          if(readString.indexOf("?cancelloon") >0)
          {
            statop1 = true;
          }
          if(readString.indexOf("?cancellooff") >0)
          {
            statop1 = false;
          }
          if(statop1 == true)
          {
            digitalWrite(7, LOW);            
          }
          if(statop1 == false)
          {
            digitalWrite(7, HIGH);            
          }
          //clear for next reading
          readString="";
 
        }
      }
    }
  }
} 


