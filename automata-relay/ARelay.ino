#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiUdp.h>

// ESPOne Relay Buggy Version
//#define GPIO_RELAY 0
//#define GPIO_LED 2
//#define GPIO_BUTTON 0

// ESPOne Relay Proper Version
//#define GPIO_RELAY 0
//#define GPIO_LED 2
//#define GPIO_BUTTON 0

// Sonoff
#define GPIO_RELAY 12
#define GPIO_LED 13
#define GPIO_BUTTON 0
#define GPIO_BUTTON_PRESSING LOW

const uint32_t configurationHeader = 0x5fe08e26;
struct Configuration
{
    uint32_t header;
    uint32_t UUID;
    char wifiSSID[64];
    char wifiPassword[64];
    char hostname[32];
    bool DHCP;
    uint32_t IP;
    uint32_t subnet;
    uint32_t gateway;
    uint16_t httpPort;
    bool powerOnState;
};

const uint32_t configurationReplyHeader =  0x53f59729;
struct ConfigurationBroadcastReplyPacket
{
    uint32_t header;
};

enum class State
{
    WPSConfiguration,
    WifiConnection,
    Configuration,
    Setup,
    Running,
    Teardown
};

// Configuration
Configuration configuration;
State state = State::WifiConnection;
const long clientTimeout = 2000;
bool verbose = true;
bool debug = true;
WiFiServer* server = NULL;
WiFiUDP udp;
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

bool buttonReleased()
{
    static unsigned long lastStateChange = 0;
    bool currentState = digitalRead(GPIO_BUTTON) == GPIO_BUTTON_PRESSING;

    if (currentState)
    {
        if (lastStateChange == 0)
            lastStateChange = millis();

        return false;
    }
    else
    {
        if (lastStateChange == 0)
        {
            return false;
        }
        else if (millis() - lastStateChange > 50)
        {
            lastStateChange = 0;
            return true;
        }
        else
        {
            lastStateChange = 0;
            return false;
        }
    }
}

const char* getStateName(State state)
{
    switch (state)
    {
        case State::WifiConnection:
        return "WifiConnection";

        case State::WPSConfiguration:
        return "WPAConfiguration";

        case State::Configuration:
        return "Configuration";

        case State::Setup:
        return "Setup";

        case State::Running:
        return "Running";

        case State::Teardown:
        return "Teardown";

        default:
        return "UNKNOWN";
    }
}

void changeState(State newState)
{
    if (state == newState)
        return;

    State oldState = state;
    state = newState;
    debugOut("State changed from %s to %s.", getStateName(oldState), getStateName(newState));
}

void setupHardware()
{
    pinMode(GPIO_RELAY, OUTPUT);
    digitalWrite(GPIO_RELAY, LOW);

    pinMode(GPIO_LED, OUTPUT);
    digitalWrite(GPIO_LED, HIGH);

    pinMode(GPIO_BUTTON, INPUT);

    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(false);

    Serial.begin(115200);
    delay(5000);
    Serial.print("\n\n");
}

void readEEPROM()
{
    verboseOut("Reading configurations from EEPROM...");
    EEPROM.begin(sizeof(Configuration));
        EEPROM.get(0, configuration);
    EEPROM.end();

    if (configuration.header != configurationHeader)
    {
        verboseOut("Uninitialized EEPROM detected. Initializing...");
        memset(&configuration, 0, sizeof(Configuration));
        configuration.header = configurationHeader;
        configuration.UUID = random(0, 999999999);
        sprintf(configuration.hostname, "Relay-%u", configuration.UUID);
        configuration.DHCP = true;

        configuration.httpPort = 80;
        writeEEPROM();
    }
    else
    {
        verboseOut("UUID: 0x%08X", configuration.UUID);
        verboseOut("Wifi SSID: %s", configuration.wifiSSID);
        verboseOut("Wifi Password: %s", configuration.wifiPassword);
        verboseOut("Hostname: %s", configuration.hostname);
        verboseOut("DHCP: %s", configuration.DHCP ? "yes" : "no");
        verboseOut("IP: %u.%u.%u.%u",
            ((char*)&configuration.IP)[0],
            ((char*)&configuration.IP)[1],
            ((char*)&configuration.IP)[2],
            ((char*)&configuration.IP)[3]);
        verboseOut("Subnet: %u.%u.%u.%u",
            ((char*)&configuration.subnet)[0],
            ((char*)&configuration.subnet)[1],
            ((char*)&configuration.subnet)[2],
            ((char*)&configuration.subnet)[3]);
        verboseOut("Gateway: %u.%u.%u.%u",
            ((char*)&configuration.gateway)[0],
            ((char*)&configuration.gateway)[1],
            ((char*)&configuration.gateway)[2],
            ((char*)&configuration.gateway)[3]);
        verboseOut("HTTP Port: %u", configuration.httpPort);
        verboseOut("Power On State: %s", configuration.powerOnState ? "Yes" : "No");
    }
}

void writeEEPROM()
{
    verboseOut("Writing configurations to EEPROM...");
    verboseOut("UUID: 0x%08X", configuration.UUID);
    verboseOut("Wifi SSID: %s", configuration.wifiSSID);
    verboseOut("Wifi Password: %s", configuration.wifiPassword);
    verboseOut("Hostname: %s", configuration.hostname);
    verboseOut("DHCP: %s", configuration.DHCP ? "yes" : "no");
    verboseOut("IP: %u.%u.%u.%u",
        ((char*)&configuration.IP)[0],
        ((char*)&configuration.IP)[1],
        ((char*)&configuration.IP)[2],
        ((char*)&configuration.IP)[3]);
    verboseOut("Subnet: %u.%u.%u.%u",
        ((char*)&configuration.subnet)[0],
        ((char*)&configuration.subnet)[1],
        ((char*)&configuration.subnet)[2],
        ((char*)&configuration.subnet)[3]);
    verboseOut("Gateway: %u.%u.%u.%u",
        ((char*)&configuration.gateway)[0],
        ((char*)&configuration.gateway)[1],
        ((char*)&configuration.gateway)[2],
        ((char*)&configuration.gateway)[3]);
    verboseOut("HTTP Port: %u", configuration.httpPort);
    verboseOut("Power On State: %s", configuration.powerOnState ? "Yes" : "No");

    EEPROM.begin(sizeof(Configuration));
        EEPROM.put(0, configuration);
    EEPROM.end();
}

void WPSConfigurationState()
{
    WiFi.disconnect();

    verboseOut("WPS config waiting for user input...");

    while (true)
    {
        blink(1, 100);

        if (buttonReleased())
            break;
    }

    verboseOut("Starting searching WPS configuration by pressing button...");
    digitalWrite(GPIO_LED, HIGH);

    WiFi.hostname(configuration.hostname);
    bool result = WiFi.beginWPSConfig();
    if (!result)
    {
        digitalWrite(GPIO_LED, LOW);
        verboseOut("WPS configuration failed.");
        return;
    }

    String newSSID = WiFi.SSID();
    if (newSSID.length() == 0)
    {
        digitalWrite(GPIO_LED, LOW);
        verboseOut("WPS configuration failed. Empty SSID.");
        return;
    }

    strncpy(configuration.wifiSSID, newSSID.c_str(), 64);
    strncpy(configuration.wifiPassword, WiFi.psk().c_str(), 64);
    configuration.DHCP = true;
    writeEEPROM();

    verboseOut("WPS finished. Connected to access point.");
    verboseOut("SSID: %s", WiFi.SSID().c_str());
    verboseOut("Passwrod: %s", WiFi.psk().c_str());

    digitalWrite(GPIO_LED, LOW);

    changeState(State::WifiConnection);
}

void wifiConnectionState()
{
    if (configuration.wifiSSID[0] == '\0')
    {
        changeState(State::WPSConfiguration);
        return;
    }

    verboseOut("Setting up network...");
    WiFi.disconnect();

    if (configuration.hostname[0] != '\0')
    {
        WiFi.hostname(configuration.hostname);
        if (!configuration.DHCP)
        {
            WiFi.config(
                IPAddress(configuration.IP),
                IPAddress(configuration.gateway),
                IPAddress(configuration.subnet));
        }
    }
    else
    {
        WiFi.hostname("ARelay");
    }

    verboseOut("Connecting to access point...");
    WiFi.begin(configuration.wifiSSID, configuration.wifiPassword);
    while (WiFi.status() != WL_CONNECTED)
    {
        blink(1, 500);
        if (buttonReleased())
        {
            verboseOut("User rejected wifi configuration by pressing button. Reseting Wifi configuration.");
            WiFi.disconnect();
            changeState(State::WPSConfiguration);
            return;
        }

        if (WiFi.status() == WL_CONNECT_FAILED)
        {
            verboseOut("Connection failed. Retrying.");
            WiFi.disconnect();
            WiFi.begin(configuration.wifiSSID, configuration.wifiPassword);
        }
    }

    verboseOut("Connected to access point.");
    verboseOut("SSID: %s", WiFi.SSID().c_str());
    verboseOut("IP: %s", WiFi.localIP().toString().c_str());
    verboseOut("Subnet: %s", WiFi.subnetMask().toString().c_str());
    verboseOut("Gateway: %s", WiFi.gatewayIP().toString().c_str());
    verboseOut("DNS#1: %s", WiFi.dnsIP().toString().c_str());
    verboseOut("DNS#2: %s", WiFi.dnsIP(1).toString().c_str());

    changeState(State::Setup);
}

void configurationState()
{
    verboseOut("Listening network for configuration packet...");

    Configuration packet;

    digitalWrite(GPIO_LED, LOW);

    udp.begin(1089);
    while (true)
    {
        if (buttonReleased())
        {
            verboseOut("User exited configuration mode by pressing button.");
            changeState(State::Running);
            break;
        }

        yield();

        if (udp.parsePacket() == 0)
            continue;

        if (udp.read((char*)&packet, sizeof(Configuration)) != sizeof(Configuration))
            continue;

        if (packet.header != configurationHeader)
            continue;

        verboseOut("New configuration packet received. Applying configuration...");

        configuration = packet;
        if (packet.wifiSSID[0] == '\0')
        {
            strcpy(configuration.wifiSSID, WiFi.SSID().c_str());
            strcpy(configuration.wifiPassword, WiFi.psk().c_str());
        }
        writeEEPROM();

        verboseOut("Sending acknowlegement to configuration sender...");
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
            ConfigurationBroadcastReplyPacket replayPacket;
            replayPacket.header = configurationReplyHeader;
            udp.write((const char*)&replayPacket, sizeof(ConfigurationBroadcastReplyPacket));
        udp.endPacket();

        udp.stop();

        changeState(State::Teardown);
        break;
    }
}

void setupState()
{
    verboseOut("Initializing HTTP server.");

    if (server == NULL)
        server = new WiFiServer(configuration.httpPort);

    server->begin(configuration.httpPort);
    verboseOut("Listening HTTP connections on port %u...", configuration.httpPort);

    changeState(State::Running);
}

void teardownState()
{
  verboseOut("Tearing down the system...");
    if (server != NULL)
    {
        server->stop();
        delete server;
        server = NULL;
    }

    WiFi.disconnect();

    changeState(State::WifiConnection);
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

void runningState()
{
    digitalWrite(GPIO_LED, HIGH);

    if (WiFi.status() == WL_DISCONNECTED)
    {
        changeState(State::Teardown);
        return;
    }

    WiFiClient client = server->available();
    if (!client)
        return;

    processClient(&client);
}

void setup()
{
    setupHardware();
    readEEPROM();

    relayState = configuration.powerOnState;
    digitalWrite(GPIO_RELAY, relayState);

    changeState(State::WifiConnection);
}

void loop()
{
    if (state >= State::Setup)
    {
        if (configuration.wifiSSID[0] == '\0')
        {
            verboseOut("No Wifi configuration detected.");
            changeState(State::WPSConfiguration);
        }
        else if (WiFi.status() != WL_CONNECTED)
        {
            verboseOut("No Wifi connection detected.");
            changeState(State::WifiConnection);
        }
    }

    switch (state)
    {
        case State::WifiConnection:
            wifiConnectionState();
            break;

        case State::WPSConfiguration:
            WPSConfigurationState();
            break;

        case State::Configuration:
            configurationState();
            break;

        case State::Setup:
            setupState();
            break;

        case State::Running:
            if (buttonReleased())
            {
                verboseOut("User entering configuration mode by pressing button.");
                changeState(State::Configuration);
                break;
            }
            runningState();
            break;

        case State::Teardown:
            teardownState();
            break;
    }
}
