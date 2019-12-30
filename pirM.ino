#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
int calibrationTime = 30;          
#define pirPin D6    
#define ledPin D5
 

ESP8266WebServer server(80); // 80 is the port number

const char* ssid="My Wifi";
const char* password="alpha321";

String Website,data,XML,Javascript;
void javascriptContent(){
    Javascript+="<script>";
    Javascript+="var xmlHttp=createXmlHttpObject();\n";
    Javascript+="function createXmlHttpObject(){\n";
    Javascript+="if(window.XMLHttpRequest){\n";
    Javascript+="xmlHttp=new XMLHttpRequest();\n";
    Javascript+="}else{\n";
    Javascript+="xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
    Javascript+="}\n";
    Javascript+="return xmlHttp;\n";
    Javascript+="}\n";
    Javascript+="function m1(data1)\n";
    Javascript+="{\n";
    Javascript+="var image;\n";
    Javascript+="if(data1==\"1\")\n";
    Javascript+="{\n";
    Javascript+="image=\"https://cdn.dribbble.com/users/1163294/screenshots/3296560/im-blue.gif\";\n";
    Javascript+="}\n";
    Javascript+="else\n";
    Javascript+="{\n";
    Javascript+="image= \"https://media.giphy.com/media/l3mZliqXpqkVkkQYE/giphy.gif\";\n";
    Javascript+="}\n";
    Javascript+="return image;\n";
    Javascript+="}\n";
    Javascript+="function response(){\n";
    Javascript+="xmlResponse=xmlHttp.responseXML;\n";
    Javascript+="xmldoc = xmlResponse.getElementsByTagName(\"data\");\n";
    Javascript+="message = xmldoc[0].firstChild.nodeValue;\n";
    Javascript+="document.getElementById(\"myImg\").src = m1(message);\n";
    Javascript+="if(message==\"1\")\n";
    Javascript+="{\n";
    Javascript+="document.getElementById(\"demo\").innerHTML = \"Motion Detected\";\n";
    Javascript+="}\n";
    Javascript+="else\n";
    Javascript+="{\n";
    Javascript+="document.getElementById(\"demo\").innerHTML = \"No Motion\";\n";
    Javascript+="}\n";
    Javascript+="}\n";
    Javascript+="function process(){\n";
    Javascript+="xmlHttp.open('PUT','xml',true);\n";
    Javascript+="xmlHttp.onreadystatechange=response;\n";
    Javascript+="xmlHttp.send(null);\n";
    Javascript+="setTimeout('process()',200);\n";
    Javascript+="}\n";
    Javascript+="</script>";
  }

void WebsiteContent(){
    javascriptContent(); 
Website="<html>\n";
Website+="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
Website+="<title>\n";
Website+="PirSensor\n";
Website+="</title>\n";
Website+="<style>\n";
Website+="body{\n";
Website+="background-image: url(\"https://images.designtrends.com/wp-content/uploads/2015/11/06163757/plain-Backgrounds1.jpg\");\n";
Website+="background-repeat: no-repeat;\n";
Website+="background-size:100%;\n";
Website+="}\n";
Website+="#h10h{\n";
Website+="font-family:Times New Roman;\n";
Website+="font-size:100px;\n";
Website+="color:Darker Indigo;\n";
Website+="margin-top:50px;\n";
Website+="text-align:center;\n";
Website+="}\n";
Website+="#myImg{\n";
Website+="width:450px;\n";
Website+="height:250px;\n";
Website+="margin-left:150px;\n";
Website+="margin-top:50px;\n";
Website+="border: 2px solid white;\n";
Website+="}\n";
Website+="#demo{\n";
Website+="font-family: Consolas;\n";
Website+="color:#3C6478;\n";
Website+="font-size:100px;\n";
Website+="margin-right:300px;\n";
Website+="margin-top:50px;\n";
Website+="text-align:right;\n";
Website+="}\n";
Website+="div.main p{\n";
Website+="float:right;\n";
Website+="}\n";
Website+="</style>\n";
Website+="</head>\n";
Website+="<body onload='process()'>\n";
Website+="<h1 id=\"h10h\">\n";
Website+="<b>BURGLAR DETECTION</b>\n";
Website+="</h1>\n";
Website+="<div class=\"main\">\n";
Website+="<img id=\"myImg\" src=\"https://media.giphy.com/media/l3mZliqXpqkVkkQYE/giphy.gif\">\n";
Website+="<p id=\"demo\">"+data+"</p>\n";
Website+="</div>\n";
Website+=Javascript;
Website+="</body>\n";
Website+="</html>\n"; 
    server.send(200,"text/html",Website);
  }
void XMLcontent(){

  XML ="<?xml version='1.0'?>";
  XML+="<data>";
  XML+=data;
  XML+="</data>";

  server.send(200,"text/xml",XML);
  
  }

void setup() {
    Serial.begin(115200);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  WiFi.mode(WIFI_STA);
  Serial.print(WiFi.localIP());
  server.on("/",WebsiteContent);
  server.on("/xml",XMLcontent);
  server.begin();


}
void loop() {
if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
        data="0";
       }
     if(digitalRead(pirPin) == LOW){      
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
        data="1";
     }
       Serial.println(data);
  server.handleClient();
  }
