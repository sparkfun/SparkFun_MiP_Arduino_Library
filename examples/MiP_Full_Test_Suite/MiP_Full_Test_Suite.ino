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

uint8_t localVolume = 0;
uint8_t localVoiceVersion = 0;
uint8_t localHWVersion = 0;
SoftwareVersion localSWVersion = {false, 0, 0, 0, 0};
int soundIndex = 1;

void setup() {
  MyMiP.init(); //Serial port is configured for 115200
  //Serial.println("MiP init.");
  delay(1000); // Need a delay to let the buffer clear

  //MyMiP.enableDebug();
  testSoftwareVersion();
  testVoiceHardwareVersion();
  testHardwareVersion();
  testVolume();
  MyMiP.disableDebug();
}



void loop() {
  loop_LEDs();
  delay(3000);
}

void loop_LEDs() {

  MyMiP.setChestLED(255, 0, 0);
  delay(200);
  MyMiP.setChestLED(0, 255, 0);
  delay(200);
  MyMiP.setChestLED(0, 0, 255);
  delay(200);

}

void testSound() {
  while (soundIndex <= 106) {
    MyMiP.playSound((Sounds)soundIndex, 0, 0);
    delay(3000);
    soundIndex++;
  }
}

void testVolume() {
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
}

void testVoiceHardwareVersion() {
  localVoiceVersion = MyMiP.getVoiceHardwareVersion();
  if (localVoiceVersion == -1) {
    Serial.println("Failed getting voice chip version");
    delay(1000);
  } else {
    Serial.print("Voice Hardware version: ");
    Serial.print(localVoiceVersion, HEX);
    Serial.print('\n');
    delay(1000);
  }
}

void testHardwareVersion() {
  localHWVersion = MyMiP.getHardwareVersion();
  if (localHWVersion == -1) {
    Serial.println("Failed getting HW version");
    delay(1000);
  } else {
    Serial.print("Hardware version: ");
    Serial.print(localHWVersion, HEX);
    Serial.print('\n');
    delay(1000);
  }
}

void testSoftwareVersion() {
  localSWVersion = MyMiP.getSoftwareVersion();
  if (!localSWVersion.isSet) {
    Serial.println("Failed getting SW version");
    delay(1000);
  } else {
    Serial.print("Software version: ");
    Serial.print(localSWVersion.year, HEX);
    Serial.print('.');
    Serial.print(localSWVersion.month, HEX);
    Serial.print('.');
    Serial.print(localSWVersion.day, HEX);
    Serial.print('.');
    Serial.print(localSWVersion.number, HEX);
    Serial.print('\n');
    delay(1000);
  }
}

