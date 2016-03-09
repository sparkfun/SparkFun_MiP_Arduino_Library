/*MiP Turn Angle example
Written by 2stacks
2016
*/

#include "MiP_commands.h"

MiP MyMiP(2);
  
void setup(){

  MyMiP.init(); //Serial port is configured for 115200
  Serial.print("MiP is Alive!!!");
  delay(100); //Need a delay to let the buffer clear

}

void loop() {

  MyMiP.playSound(MIP_SOUND_ACTION_BURPING, 0, 0);
  delay(3000);

  /*
   * MyMiP.turnRight(Angle, Speed);
   * or
   * MyMiP.turnLeft(Angle, Speed);   
   *  Angle in intervals of 5 degrees (0~255)
   *    18 = 90 degrees
   *    36 = 180 degrees
   *    54 = 270 degrees
   *    72 = 360 degrees
   *    255 = 1275 degrees or 3.5 complete rotations
   *  Speed (0~24)
  */
  

  MyMiP.turnRight(18, 24);
  delay(2000);
  MyMiP.turnLeft(18, 12);
  delay(2000);
  MyMiP.turnLeft(72, 24);
  delay(2000);
  MyMiP.turnRight(72, 12);
  delay(2000);
  MyMiP.turnRight(255, 24);
  delay(3000);
  
}
