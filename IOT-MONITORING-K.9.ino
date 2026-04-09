#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> // Library for the Web Server
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

const char* ssid     = "HOTSPOT@UPNJATIM.AC.ID";
const char* password = "belanegara";

// Hardware Pins
#define DHTPIN 0      // D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Initialize the web server on port 80
ESP8266WebServer server(80);

void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW); // Blink LED when someone visits the page
  
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Create a simple HTML page
  String html = "<!DOCTYPE html><html>";
html += "<head>";
html += "<meta http-equiv='refresh' content='5'>";
html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
html += "<title> Kelompok 9 - Monitoring</title>";

html += "<style>";
html += "body { font-family: Arial, sans-serif; text-align: center; background: linear-gradient(to right, #74ebd5, #ACB6E5); margin: 0; padding: 0; }";
html += ".container { margin-top: 50px; }";
html += ".card { background: white; padding: 25px; margin: auto; width: 320px; border-radius: 15px; box-shadow: 0px 4px 15px rgba(0,0,0,0.2); }";
html += "h1 { color: #333; margin-bottom: 5px; }";
html += "h2 { color: #777; margin-top: 0; }";
html += ".label { font-size: 18px; color: #555; }";
html += ".value { font-size: 30px; font-weight: bold; color: #007BFF; margin-bottom: 15px; }";
html += ".footer { margin-top: 15px; font-size: 12px; color: #999; }";
html += "</style>";

html += "</head>";

html += "<body>";
html += "<div class='container'>";
html += "<div class='card'>";

html += "<h1>ESP8266 Weather Station</h1>";
html += "<h2>Kelompok 9</h2>";

html += "<p class='label'> Temperature</p>";
html += "<p class='value'>" + String(t) + " °C</p>";

html += "<p class='label'> Humidity</p>";
html += "<p class='value'>" + String(h) + " %</p>";

html += "<div class='footer'>Auto refresh setiap 5 detik</div>";

html += "</div>";
html += "</div>";
html += "</body></html>";

  server.send(200, "text/html", html);
  
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}

void handleRootJSON() {
  // Blink LED to indicate data transmission/request
  digitalWrite(LED_BUILTIN, LOW); 
  
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Create a JSON string (Machine-friendly)
  // Format: {"temperature": 25.5, "humidity": 60.0}
  String json = "{";
  json += "\"temperature\": " + String(t) + ",";
  json += "\"humidity\": " + String(h);
  json += "}";

  // Send with "application/json" header instead of "text/html"
  server.send(200, "application/json", json);
  
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  // Define the "Home" route of the web server
  server.on("/", handleRoot);
  server.begin();

  // Show the IP address on the OLED (You need this to access the data!)
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("IP Address:");
  display.setTextSize(2);
  display.setCursor(0,20);
  display.println(WiFi.localIP());
  display.display();
}

void loop() {
  server.handleClient(); // Listen for web browsers
}