/*MiP Turn Angle example
Written by 2stacks
2016
*/

#include <MiP_commands.h>
#include <MiP_Parameters.h>
#include <MiP_sounds.h>


MiP MyMiP(2,3);
  
void setup(){

  MyMiP.init(); //Serial port is configured for 115200
  Serial.print("MiP is Alive!!!");
  delay(100); //Need a delay to let the buffer clear

}

void loop() {

  MyMiP.playSingleSound(BURP);
  delay(3000);

  /*
   * MyMiP.turnAngle(Direction, Angle, Speed);
   *  Direction 
   *    0 = Right
   *    1 = Left
   *  Angle in intervals of 5 degrees (0~255)
   *    18 = 90 degrees
   *    36 = 180 degrees
   *    54 = 270 degrees
   *    72 = 360 degrees
   *    255 = 1275 degrees or 3.5 complete rotations
   *  Speed (0~24)
  */
  

  MyMiP.turnAngle(0, 18, 24);
  delay(3000);
  MyMiP.turnAngle(1, 18, 12);
  delay(3000);
  MyMiP.turnAngle(1, 72, 24);
  delay(3000);
  MyMiP.turnAngle(0, 72, 12);
  delay(3000);
  MyMiP.turnAngle(0, 255, 24);
  delay(3000);
  
}
