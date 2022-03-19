#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);


const char* ssid = "*название 2-ух Гц wi-fi*";
const char* password = "*пароль*";

const byte port_led_blink = 14;
const byte port_btn_light = 27;
const byte port_ptns_light = 32;
const byte port_btn_ws = 33;
bool state_led_blink = false;
unsigned long timer_led_blink = 0;
unsigned long timer_ptns_blink = 0;
unsigned long timer_btn_ws = 0;
unsigned long timer_btn_looseWater = 0;
bool state_btn_light = false;
int allWateer = 0;
bool state_led_ws = false;
bool old_state_btn_ws = false;

void pageRoot(){
  server.send(200, "text/html", "<a href=\"/on\"><button>Open</button></a><a href=\"/off\"><button>Close</button></a><a href=\"/\"><button>" + String(allWateer) + "</button></a><a href=\"/resetWatr\"><button>reset water</button></a>");
}
void pageOn(){
  state_led_blink = 1;
  digitalWrite(port_led_blink, state_led_blink);
  server.send(200, "text/html", "<a href=\"/on\"><button>Open</button></a><a href=\"/off\"><button>Close</button></a><a href=\"/\"><button>" + String(allWateer) + "</button></a><a href=\"/resetWatr\"><button>reset water</button></a>");
}
void pageOff(){
  state_led_blink = 0;
  digitalWrite(port_led_blink, state_led_blink);
  server.send(200, "text/html", "<a href=\"/on\"><button>Open</button></a><a href=\"/off\"><button>Close</button></a><a href=\"/\"><button>" + String(allWateer) + "</button></a><a href=\"/resetWatr\"><button>reset water</button></a>");
}

void pageBtn(){
  state_led_blink = 0;
  digitalWrite(port_led_blink, state_led_blink);
  server.send(200, "text/html", "<a href=\"/on\"><button>Open</button></a><a href=\"/off\"><button>Close</button></a><button><a href=\"/btn\">The doorbell rang</button></a><a href=\"/off\"><button>Ignore</button></a><a href=\"/\"><button>" + String(allWateer) + "</button></a><a href=\"/resetWatr\"><button>reset water</button></a>");
}

void pageResWatr(){
  allWateer = 0;
  server.send(200, "text/html", "<a href=\"/on\"><button>Open</button></a><a href=\"/off\"><button>Close</button></a><a href=\"/\"><button>" + String(allWateer) + "</button></a><a href=\"/resetWatr\"><button>reset water</button></a>");
}

void setup() {
  
  Serial.begin(9600);
  pinMode(port_led_blink, OUTPUT);
  WiFi.begin(ssid, password);
  pinMode(port_led_blink, OUTPUT);
  pinMode(port_ptns_light, INPUT);
  pinMode(port_ptns_light, INPUT);
  pinMode(port_btn_light, INPUT);
  pinMode(port_btn_ws, INPUT);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);

    Serial.println(WiFi.status());
  }

  Serial.println("Connected to the WiFI network");
  Serial.println(WiFi.localIP());

  server.on("/", pageRoot);
  server.on("/on", pageOn);
  server.on("/off", pageOff);
  server.on("/btn", pageBtn);
  server.on("/resetWatr", pageResWatr);

  server.begin();

}

void loop() {
  if (millis() - timer_btn_looseWater >= 20000 && analogRead(port_ptns_light)/16 >= 1){

  }
  else {
  //Serial.println(allWateer);
    if (millis() - timer_ptns_blink >= 1000) {
      allWateer += analogRead(port_ptns_light)/16;
      timer_ptns_blink = millis();
      //Serial.println(analogRead(port_ptns_light));
    }

    if(analogRead(port_ptns_light)/16 < 1) timer_btn_looseWater = millis();
  }

  bool state_btn_ws = digitalRead(port_btn_ws);
  if ((state_btn_ws) && (!old_state_btn_ws)
  && (millis() - timer_btn_ws >= 250)) {
    allWateer += 5000;
    timer_btn_ws = millis();
  }
  old_state_btn_ws = state_btn_ws;

  

  state_btn_light = digitalRead(port_btn_light);
  if(state_btn_light == 1){
    //Serial.println(state_btn_light);
    pageBtn();
  }

  if (millis() - timer_led_blink >= 10000 && state_led_blink == 1) {
    state_led_blink = 0;
    digitalWrite(port_led_blink, state_led_blink);
    timer_led_blink = millis();
  }
  if (millis() - timer_led_blink >= 10000 && state_led_blink == 0) {
    timer_led_blink = millis();
  }

  if(WiFi.status() == WL_CONNECTED){
    /// главная программа, которая работает с сетью
    server.handleClient();
  }
}