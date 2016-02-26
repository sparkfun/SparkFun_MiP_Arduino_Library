/*
  MiP_Full_Test_Suite.ino

  Work in progress. This file exercises all of the functions provided
  by SparkFun_MiP_Arduino_Library. 

  Intended for use with Sparkfun's DEV-13058- ProMini Pack that allows 
    hacking of a WowWee MiP.

  Created 26 Feb 2016

*/

#include "MiP_commands.h"

MiP MyMiP(2);

int localVolume = -1;
int localVoiceVersion = -1;
int localVersion = -1;
SoftwareVersion localSWVersion = {-1,-1,-1,-1};

void setup() {
  MyMiP.init(); //Serial port is configured for 115200
  Serial.println("MiP init.");
  delay(3000); // Need a delay to let the buffer clear
  localVoiceVersion = MyMiP.getVoiceHardwareVersion();
  if (localVoiceVersion == -1) {
    Serial.println("Failed getting voice chip version");
    delay(1000);
  } else {
    Serial.print("Voice Hardware version: ");
    Serial.print(localVoiceVersion);
    Serial.print('\n');
    delay(1000);
  }
  localVolume = MyMiP.getVolume();
  if (localVolume == -1) {
    Serial.println("Failed getting volume");
  }
  else {
    Serial.print("Volume: ");
    Serial.print(localVolume);
    Serial.print('\n');
    delay(1000);
  }
  MyMiP.enableDebug();
  localVersion = MyMiP.getHardwareVersion();
  if (localVersion == -1) {
    Serial.println("Failed getting HW version");
    delay(1000);
  } else {
    Serial.print("Voice Hardware version: ");
    Serial.print(localVersion);
    Serial.print('\n');
    delay(1000);
  }
  localSWVersion = MyMiP.getSoftwareVersion();
  if (localSWVersion.year == -1) {
    Serial.println("Failed getting SW version");
    delay(1000);
  } else {
    Serial.print("Software version: ");
    Serial.print(localSWVersion.year);
    Serial.print('\n');
    delay(1000);
  }
  MyMiP.disableDebug();
}



void loop() {
  delay(3000);
  loop_LEDs();
}

void loop_LEDs() {

  MyMiP.setChestLED(255, 0, 0);
  delay(200);
  MyMiP.setChestLED(0, 255, 0);
  delay(200);
  MyMiP.setChestLED(0, 0, 255);
  delay(200);

}
