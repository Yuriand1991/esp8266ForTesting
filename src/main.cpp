#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>


#define ONBOARD_LED_PIN 16
#define ONCHIP_LED_PIN 2

#define SSID "FreeBird"
//#define PASS

String Header();
String Footer();

ESP8266WebServer server(80);

void handleIndex();
void handleOn();
void handleOff();
void handleInfo();

void setup(){
	Serial.begin(9600);
	pinMode(ONBOARD_LED_PIN, OUTPUT);
	pinMode(ONCHIP_LED_PIN, OUTPUT);
	
	digitalWrite(ONBOARD_LED_PIN, HIGH);
	digitalWrite(ONCHIP_LED_PIN, HIGH);
	WiFi.begin(SSID);
	Serial.print("Connecting");
	while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
		delay(100);
    }
	Serial.println(WiFi.localIP().toString());
	ArduinoOTA.begin(); // Для прошивки через Wi-Fi
	//digitalWrite(ONBOARD_LED_PIN, LOW);
	
	server.on("/", handleIndex);
	server.on("/on", handleOn);
	server.on("/off", handleOff);
	server.on("/info", handleInfo);
	server.begin();
}

void loop(){
	//Serial.print(millis());
	//Serial.println(" Hello world");
	//delay(1000);
	server.handleClient();
	ArduinoOTA.handle(); // Для прошивки через Wi-Fi
}

void handleIndex(){
	String html = "<h1>Hello world</h1><h2>My IP: " + WiFi.localIP().toString() + "</h2>"
				  "<button onclick=\"send('/on');\">Включить/Скорость</button><br>"
				  "<button onclick=\"send('/off');\">Выключить</button><br>";
	server.send(200, "text/html", html);
}

void handleOn(){
	digitalWrite(ONBOARD_LED_PIN, LOW);
	server.send(200, "text/html", "OK");
}

void handleOff(){
	digitalWrite(ONBOARD_LED_PIN, HIGH);
	server.send(200, "text/html", "OK");
}

void handleInfo(){
	String html = Header() + "<script>function send(url){ var xhr = new XMLHttpRequest(); xhr.open('GET', url, false); xhr.send(); }</script>"
							 "<h1>Hello</h1>"
							 "<button onclick=\"send('/on');\">Включить</button>"
							 "<button onclick=\"send('/off');\">Выключить</button>"
							 + Footer();
	server.send(200, "text/html", html);	
}

String Header(){
	return String("<DOCTYPE html>"
						"<html>"
							"<head>"
								"<meta charset=\"UTF-8\">"
								"<title>Выключатель</title>"
							"</head>"
							"<body>");
}

String Footer() { 
	return String("</body></html>"); 
}