#include <ESP8266WiFi.h>
#include <espnow.h>

const int button1 = D3;
const int button2 = D4;
const int button3 = D5;
const int button4 = D6;
const int button5 = D7;

typedef struct dataSent
{
    int pressedButton;
};
uint8_t broadcastAddress[] = {0x4C, 0x11, 0xAE, 0xF9, 0x66, 0x4C};
dataSent myData;

unsigned long lastTime = 0;
unsigned long timerDelay = 2000; // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0)
    {
        Serial.println("Delivery success");
    }
    else
    {
        Serial.println("Delivery fail");
    }
}

void buttonSetup()
{
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);
    pinMode(button4, INPUT_PULLUP);
    pinMode(button5, INPUT_PULLUP);
}

void setup()
{
    delay(1000);
    buttonSetup();
    WiFi.mode(WIFI_STA);
    Serial.begin(9600);
    delay(4000);
    Serial.println();
    Serial.println(WiFi.macAddress());
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop()
{
    int button1State = digitalRead(button1);
    int button2State = digitalRead(button2);
    int button3State = digitalRead(button3);
    int button4State = digitalRead(button4);
    int button5State = digitalRead(button5);

    if (button1State == LOW)
    {
        Serial.println("Button 1 pressed");
        myData.pressedButton = 1;
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    }
    else if (button2State == LOW)
    {
        Serial.println("Button 2 pressed");
        myData.pressedButton = 2;
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    }
    else if (button3State == LOW)
    {
        Serial.println("Button 3 pressed");
        myData.pressedButton = 3;
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    }
    else if (button4State == LOW)
    {
        Serial.println("Button 4 pressed");
        myData.pressedButton = 4;
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    }
    else if (button5State == LOW)
    {
        Serial.println("Button 5 pressed");
        myData.pressedButton = 5;
        esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    }
}