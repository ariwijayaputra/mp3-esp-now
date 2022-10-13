/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>
#include "SoftwareSerial.h"
#include <DFMiniMp3.h>
class Mp3Notify;
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3;
DfMp3 dfmp3(Serial2);
class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char *action)
  {
    if (source & DfMp3_PlaySources_Sd)
    {
      Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb)
    {
      Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash)
    {
      Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError(DfMp3 &mp3, uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished(DfMp3 &mp3, DfMp3_PlaySources source, uint16_t track)
  {
    Serial.print("Play finished for #");
    Serial.println(track);
  }
  static void OnPlaySourceOnline(DfMp3 &mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted(DfMp3 &mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved(DfMp3 &mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};

SoftwareSerial mySoftwareSerial(15, 2); // RX, TX

// Structure example to receive data
// Must match the sender structure
typedef struct dataSent
{
  int pressedButton;
};
// Create a struct_message called myData
dataSent myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Button: ");
  Serial.println(myData.pressedButton);
}
void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();

  while ((millis() - start) < msWait)
  {
    // if you have loops with delays, its important to
    // call dfmp3.loop() periodically so it allows for notifications
    // to be handled without interrupts
    dfmp3.loop();
    delay(1);
  }
}
void setupESPNOW()
{
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void setup()
{
  // Initialize Serial Monitor
  Serial.begin(9600);

  Serial2.begin(9600);

  delay(5000);
  Serial.println("initializing...");

  dfmp3.begin();

  uint16_t volume = dfmp3.getVolume();
  Serial.print("volume ");
  Serial.println(volume);
  dfmp3.setVolume(24);

  uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  Serial.print("files ");
  Serial.println(count);

  Serial.println("starting...");
  delay(5000);
  setupESPNOW();
}

void loop()
{
  if (myData.pressedButton == 1)
  {
    dfmp3.playMp3FolderTrack(1);
    myData.pressedButton = 0;
  }else if (myData.pressedButton == 2)
  {
    dfmp3.playMp3FolderTrack(2);
    myData.pressedButton = 0;
  }
  else if (myData.pressedButton == 3)
  {
    dfmp3.playMp3FolderTrack(3);
    myData.pressedButton = 0;
  }
  else if (myData.pressedButton == 4)
  {
    dfmp3.playMp3FolderTrack(4);
    myData.pressedButton = 0;
  }
  else if (myData.pressedButton == 5)
  {
    dfmp3.playMp3FolderTrack(5);
    myData.pressedButton = 0;
  }

}