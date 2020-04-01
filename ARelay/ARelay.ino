#include <ESP8266WiFi.h>

// Configuration
const char* hostname = "Relay-01";
const char* ssid     = "Access-01";
const char* password = "L1sh4r.2222";
const IPAddress staticIP(192, 168, 44, 51);
const IPAddress gateway(192, 168, 44, 1);
const IPAddress subnet(255,255,255,0);
const IPAddress dns1(192, 168, 44, 1);
const IPAddress dns2(192, 168, 44, 1);
const int serverPort = 80;
const long clientTimeout = 2000;
bool verbose = true;
bool debug = false;

#define GPIO_RELAY 0
#define GPIO_LED 2

WiFiServer server(serverPort);
bool relayState = false;


void verboseOut(const char* format, ...)
{
  if (!verbose)
    return;

  va_list args;
  va_start(args, format);
    char buffer[512];
    vsnprintf(buffer, 512, format, args);
    Serial.printf("Verbose: %s\n", buffer);
  va_end(args);
}

void debugOut(const char* format, ...)
{
  if (!debug)
    return;

  va_list args;
  va_start(args, format);
    char buffer[512];
    vsnprintf(buffer, 512, format, args);
    Serial.printf("Debug: %s\n", buffer);
  va_end(args);
}

void blink(int count, int ms)
{
  for (int I = 0; I < count; I++)
  {
    digitalWrite(GPIO_LED, HIGH);
    delay(ms);
    digitalWrite(GPIO_LED, LOW);
    delay(ms);
  }
}

void setupNetwork()
{
  Serial.println("Setting up network...");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(false);
  WiFi.hostname(hostname);
  WiFi.config(staticIP, gateway, subnet, dns1, dns2);
  WiFi.begin(ssid, password);
  verboseOut("SSID: %s", WiFi.SSID().c_str());
  verboseOut("IP: %s", WiFi.localIP().toString().c_str());
  verboseOut("Subnet: %s", WiFi.subnetMask().toString().c_str());
  verboseOut("Gateway: %s", WiFi.gatewayIP().toString().c_str());
  verboseOut("DNS#1: %s", WiFi.dnsIP().toString().c_str());
  verboseOut("DNS#2: %s", WiFi.dnsIP(1).toString().c_str());

  verboseOut("Connecting to access point...");

  while (WiFi.status() != WL_CONNECTED)
    blink(1, 500);

  verboseOut("Connected.");
  blink(4, 250);
}

void setupHardware()
{
  Serial.begin(115200);
  Serial.print("\n\n");

  digitalWrite(GPIO_RELAY, relayState);
  pinMode(GPIO_RELAY, OUTPUT);
  digitalWrite(GPIO_RELAY, relayState);

  pinMode(GPIO_LED, OUTPUT);
  digitalWrite(GPIO_LED, HIGH);
}

void setupServer()
{
  server.begin(serverPort);
  verboseOut("Listening HTTP connections on port %u...", serverPort);
}

void setup() 
{
  setupHardware();
  setupNetwork();
  setupServer();
}

bool processRequest(WiFiClient* client, const String& requestHeader, String& responseHeader, String& responseBody)
{
  if (requestHeader.indexOf("GET / HTTP/1.1") >= 0)
  {
    verboseOut("Requested /");
    debugOut("Relay status is %s.", relayState ? "true" : "false");
  }
  else if (requestHeader.indexOf("GET /on HTTP/1.1") >= 0)
  {
    verboseOut("Requested /on");

    relayState = true;
    digitalWrite(GPIO_RELAY, relayState);

    debugOut("Relay swiched to ON state.");
  }
  else if (requestHeader.indexOf("GET /off HTTP/1.1") >= 0)
  {
    verboseOut("Requested /off");

    relayState = false;
    digitalWrite(GPIO_RELAY, relayState);
  
    debugOut("Relat swiched to OFF state.");
  }
  else
  {
    verboseOut("Bad request");

    responseHeader = 
      "HTTP/1.1 400 Bad Request\r\n"
      "Content-type: application/json\r\n"
      "Connection: close\r\n";

    responseBody= "{\"error\": \"Invalid URL.\"}";

    return true;
  }

  responseHeader = 
    "HTTP/1.1 200 OK\r\n"
    "Content-type: application/json\r\n"
    "Connection: close\r\n";

  responseBody = "{\"state\": ";
  responseBody += relayState ? "true" : "false";
  responseBody += "}";
  
  return true;    
}

void processClient(WiFiClient* client)
{
  verboseOut("Connection received from %s:%u.", client->remoteIP().toString().c_str(), client->remotePort());

  int currentTime = millis();
  int prevTime = currentTime;

  String requestHeader;
  String requestBody;

  String responseHeader;
  String responseBody;

  bool handled = false;
  while (true)
  {
    if (!client->connected())
      break;

    currentTime = millis();
    if ((currentTime - prevTime) > clientTimeout)
    {
      verboseOut("Client timed out.");
      client->stop();
      return;
    }

    if (!client->available())
      continue;

    String headerLine = client->readStringUntil('\n');
    requestHeader += headerLine;
    if (headerLine == "\r")
    {
      debugOut("Request: %s", requestHeader.c_str());

      String responseHeader;
      String responseBody;

      if (!processRequest(client, requestHeader, responseHeader, responseBody))
        return;

      if (responseBody.length() != 0)
      {
        responseHeader += "content-length: ";
        responseHeader += responseBody.length();
        responseHeader += "\r\n";
      }

      responseHeader += "\r\n";

      client->print(responseHeader);
      debugOut("Response Headers: %s", responseHeader.c_str());

      client->print(responseBody);
      debugOut("Response Body: %s", responseBody.c_str());


      client->stop();
      verboseOut("Client disconnected.");
    }   
  }
}

void loop()
{
  WiFiClient client = server.available();
  if (!client)
    return;

  processClient(&client);
}
