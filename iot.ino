#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int adcPin = A0;
int adcValue = 0;

void setup() {

  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {

  adcValue = analogRead(adcPin);

  display.clearDisplay();

  display.setCursor(0,10);
  display.print("ADC:");

  display.setCursor(0,35);
  display.print(adcValue);

  display.display();

  delay(500);
}