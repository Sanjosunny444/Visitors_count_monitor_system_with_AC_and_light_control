#define BLYNK_TEMPLATE_ID "TMPL3AWN4be73"
#define BLYNK_TEMPLATE_NAME "attentees"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Ultrasonic.h>
char auth[] = "auth";
char ssid[] = "Angamaly Broadband 7703";
char pass[] = "pass";
#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64   
#define OLED_RESET -1       
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Ultrasonic ultrasonic1(32, 33);
Ultrasonic ultrasonic2(4, 2);
#define relay 18 
#define relay2 19 
int inStatus;
int outStatus;
int countin = 0;
int countout = 0;
int in;
int out;
int now;
WidgetLED light(V0);
void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  delay(1000); 
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  pinMode(relay2, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  delay(2000);
  Serial.println("Visitor Counter Demo");
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 20);
  display.print("Visitor");
  display.setCursor(20, 40);
  display.print("Counter");
  display.display();
  delay(3000);
}
void loop()
{
  Blynk.run();
  inStatus =  ultrasonic1.read();
  outStatus = ultrasonic2.read();
  if (inStatus < 20)
  {
    in = countin++;
  }
 
  if (outStatus < 20)
  {
    out = countout++;
  }
 
  now = in - out;
  if (now <= 0)
  {
    light.off();
    digitalWrite(relay, HIGH);
    digitalWrite(relay2, HIGH);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 15);
    display.print("No Visitor");
    display.setCursor(5, 40);
    display.print("Light Off");
    display.display();
    Serial.println("No Visitors! Light Off");
    delay(500);
  }
  else
  {
    light.on();
    digitalWrite(relay, LOW);
    digitalWrite(relay2, LOW);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(15, 0);
    display.print("Current Visitor");
    display.setTextSize(2);
    display.setCursor(50, 15);
    display.print(now);
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.print("IN: ");
    display.print(in);
    display.setTextSize(1);
    display.setCursor(70, 40);
    display.print("OUT: ");
    display.print(out);
    display.display();
    Serial.print("Current Visitor: ");
    Serial.println(now);
    Serial.print("IN: ");
    Serial.println(in);
    Serial.print("OUT: ");
    Serial.println(out);
    delay(500);
  }
  Blynk.virtualWrite(V1, in); // Visitors In
  Blynk.virtualWrite(V2, out); // Visitors Out
  Blynk.virtualWrite(V3, now); // Current Visitors
  delay(1000);
}