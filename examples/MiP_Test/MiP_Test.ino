/*MiP Test example
Written by Casey Kuhns
SparkFun Electronics

2015
*/
#include "MiP_commands.h"

MiP MyMiP(2,3);
  
void setup(){

  MyMiP.init(); //Serial port is configured for 115200
  Serial.print("MiP is Alive!!!");
  delay(100); //Need a delay to let the buffer clear

}



void loop() {

  MyMiP.playSingleSound(BURP);
  delay(100);
  MyMiP.distanceDrive(20, 45);
  loop_LEDs();

  delay(5000);
  

}

void loop_LEDs(){

    MyMiP.setChestLED(255, 0, 0);
    delay(200);
    MyMiP.setChestLED(0, 255, 0);
    delay(200);
    MyMiP.setChestLED(0, 0, 255);
    delay(200);

}
