
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TelegramBot.h>


#define LED 16 
#define pirPin 5

const char* ssid = "wifi_name";
const char* password = "wifi_password";
const char BotToken[] = "bot:token";
const char CHAT_ID[] = "chat_id(if needed)";

int ENABLE = 0;

WiFiClientSecure client;
TelegramBot bot (BotToken, client);

void setup() {
  
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  

  client.setInsecure();
  bot.begin(); 
 
  pinMode(LED, OUTPUT); 
  pinMode(pirPin, INPUT);

  
}

void loop() {
  
  Serial.print("Working");  //for test on PC
  Serial.println("");       //for test on PC
  
  message m = bot.getUpdates();
  

  int pirVal = digitalRead(pirPin); //Считываем значения с датчика движения. Если обнаружили движение, 
                                    //то транслируем сигнал тревоги в монитор порта и включаем светодиод
  if (m.text.equals("/on"))
  {
    ENABLE = 1;
    Serial.println("I'm ON"); //for test on PC
    bot.sendMessage(CHAT_ID, "I'm ON");
  }
  if (m.text.equals("/off"))
  {
    ENABLE = 0;
    Serial.println("I'm OFF"); //for test on PC
    bot.sendMessage(CHAT_ID, "I'm OFF");
  }                                   
  if (ENABLE == 1)
  {
    if (pirVal == HIGH) 
    {
      Serial.println("Somebody is present in the room now"); //for test on PC
      bot.sendMessage(CHAT_ID, "Somebody is present in the room now");
      digitalWrite(LED,LOW);
    }
    else if (pirVal == LOW)
    {
      Serial.println("Scanning...");  //for test on PC
      digitalWrite(LED,HIGH);
    }
  }
  if (ENABLE == 0)
  {
    digitalWrite(LED,HIGH);
  } 
  
    delay(5000);
}
