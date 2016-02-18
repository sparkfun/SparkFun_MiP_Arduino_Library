/*
 * MiP_commands.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: caseykuhns
 */

#include "MiP_commands.h"
#include "MiP_Parameters.h"
#include <avr/io.h>

MiP::MiP(int8_t UART_Select_S, int8_t UART_Select_R) {
  _UART_Select_S = UART_Select_S; //bring variable into a private variable.
  _UART_Select_R = UART_Select_R; //bring variable into a private variable.
  pinMode(_UART_Select_S, OUTPUT);
  pinMode(_UART_Select_R, INPUT);
}

MiP::~MiP() {
	// TODO Auto-generated destructor stub
}

void MiP::init(void){
  Serial.begin(115200);
  uint8_t array_length = 9;
  uint8_t data_array[] = "TTM:OK\r\n";
  sendMessage(data_array, array_length);
}

void MiP::playSingleSound(Sounds MiPSound){
  uint8_t array_length = 5;
  uint8_t data_array[array_length];
  data_array[0] = 0x06; //Play sound command from WowWee documentation
  data_array[1] = MiPSound; //User selected sound
  data_array[2] = 0;
  data_array[3] = 0;

  sendMessage(data_array, array_length);

}

void MiP::setPosition(SetPosition pose){
  uint8_t array_length = 3;
  uint8_t data_array[array_length];
  data_array[0] = 0x08;
  data_array[1] = pose;

  sendMessage(data_array, array_length);
}

void MiP::distanceDrive(int16_t distance, int16_t angle){
  uint8_t array_length = 7;
  uint8_t data_array[array_length];
  data_array[0] = 0x70; //Drive Distance Command

  if(distance >=0){
   data_array[1] = 0x00; // Forward
  }
  else{
   data_array[1] = 0x01; // Reverse
  };
  data_array[2] = uint8_t(distance);

  if(angle >=0){
   data_array[3] = 0x01; // Clockwise
  }
  else{
   data_array[3] = 0x00; // Counter Clockwise
  };

  angle = angle & 0x7fff; //remove direction from angle
  if(angle >= 360)angle = 360; //cap rotation at 360 deg

  data_array[4] = uint8_t(angle >> 8);
  data_array[5] = uint8_t(angle);

  sendMessage(data_array, array_length);

};

/*
//TODO
void MiP::timeDrive(int8_t direction, int8_t speed, uint8_t time){

}
*/

void MiP::turnAngle(int8_t direction, int8_t angle, uint8_t speed){
  uint8_t array_length = 4;
  uint8_t data_array[array_length];

  if(direction == 0){
    data_array[0] = 0x73; //Turn Left by Angle Command
  }
  else if(direction == 1){
    data_array[0] = 0x74; //Turn Right by Angle Command
  }
  else{
    data_array[0] = 0x73; //Default to Left Turn
  };

  if(angle >= 255)angle = 255; //cap angle at max of 255
  data_array[1] = uint8_t(angle); //Angle in intervals of 5 degrees

  if(speed >= 24)speed = 24; //cap speed at max of 24
  data_array[2] = uint8_t(speed);

  sendMessage(data_array, array_length);

}

/*
void MiP::continuousDrive(int8_t direction, int8_t speed){

}
*/
void MiP::stop(void){
  uint8_t array_length = 1;
  uint8_t data_array[array_length];
  data_array[0] = 0x77;
    
  sendMessage(data_array, array_length);
}
/*
void MiP::setGameMode(Game mode){
	uint8_t data_array[2];
	data_array[0] = 0x76;
	data_array[1] = mode;

	sendMessage(data_array);
}

void MiP::getStatus(void){

} //TODO create struct for status
*/
void MiP::standUp(int8_t state){
  uint8_t array_length = 2;
  uint8_t data_array[array_length];
  data_array[0] = 0x23;
  data_array[1] = state;

  sendMessage(data_array, array_length);
}
/*
uint8_t MiP::getWeightStatus(void){
	uint8_t weight;

	return PLAY_BACK;
}
*/
void MiP::requestChestLED(void){
  uint8_t array_length = 4;
  uint8_t data_array[array_length];
  
  data_array[0] = 0x83;
  data_array[1] = 0x00;
  data_array[2] = 0x00;
  data_array[3] = 0x00;
  //return new _LEDColor();
/*
  uint8_t returned_array[array_length];
  returned_array[array_length] = sendMessage(data_array, array_length);
  if(returned_array[1] == 255)
	  return RED;
  if(returned_array[1] == 255)
	  return GREEN;
  if(returned_array[3] == 255)
	  return BLUE; 
  */
}

void MiP::setChestLED(uint8_t red, uint8_t green, uint8_t blue){
  uint8_t array_length = 4;
  uint8_t data_array[array_length];
  data_array[0] = 0x84;
  data_array[1] = red;
  data_array[2] = green;
  data_array[3] = blue;

  sendMessage(data_array, array_length);
}

/*
void MiP::flashChestLED(uint8_t red, uint8_t green, uint8_t blue, uint8_t time_on, uint8_t time_off){

}
*/
void MiP::setHeadLEDs(uint8_t light1, uint8_t light2, uint8_t light3, uint8_t light4){
  uint8_t array_length = 6;
  uint8_t data_array[array_length];
  data_array[0] = 0x8A;
  data_array[1] = light1;
  data_array[2] = light2;
  data_array[3] = light3;
  data_array[4] = light4;  

  sendMessage(data_array, array_length);
}
/*
void MiP::getHeadLEDs(void){

} //TODO create struct for head LEDs

uint32_t MiP::getOdometer(void){
	uint32_t odometer;

	return odometer;
}
void MiP::resetOdometer(void){

}

void MiP::setGestureDetectMode(int8_t mode){

}
int8_t MiP::getGestureDetectMode(void){
	int8_t mode;

	return mode;
}
int8_t MiP::getGestureDetect(void){
	int8_t detection;

	return detection;
}
int8_t MiP::getRadarDetect(void){
	int8_t detection;

	return detection;
}

void MiP::setDetectionMode(int8_t mode, int8_t power){

}
void MiP::getDetectionMode(void){

} //TODO create structure for detection mode

int8_t MiP::getShakeDetection(void){
	int8_t shake;

	return shake;
}

void MiP::setIRcontrol(int8_t mode){}
int8_t MiP::getIRcontrol(void){}

int8_t MiP::ping(void){
	int8_t ping;

	return ping;
}

void MiP::setEEPROMData(int8_t addr, uint8_t data){

}
uint8_t MiP::getEEPROMData(int8_t addr){
	uint8_t data;
	return data;
}

void MiP::getSoftwareVersion(int8_t* version){

}
void MiP::getHardwareVersion(int8_t* version){

}
*/
void MiP::setVolume(int8_t volume){
  uint8_t array_length = 2;
  uint8_t data_array[array_length];
  data_array[0] = 0x15;
  data_array[1] = volume;

  sendMessage(data_array, array_length);
}

int8_t MiP::getVolume(){
  int answerVal = -1;
  uint8_t answer[4];
  uint8_t question[] = {GET_VOLUME};
  int iteration = 0;
  while ((0 > answerVal || answerVal > 7) && iteration < MAX_RETRIES) {
    sendMessage(question, 1);
    if (Serial.available() == 4) {
      getMessage(answer, 4);
      if (answer[0] - 48 == 1 && answer[1] - 48 == 6 && answer[2] - 48 == 0)
        answerVal = answer[3] - 48;
    }
    else
    {
      while (Serial.available())
        Serial.read();
    }
    iteration++;
  }
  // One more value check in case last try produced invalid result.
  if (0 > answerVal || answerVal > 7)
    return -1;

  return answerVal;
}
/*
void MiP::setClapDetection(int8_t mode){

}
int8_t MiP::getClapDetection(void){

}
void MiP::getClapsRecieved(int8_t* claps){

}
*/




// Private Functions

void MiP::sendMessage(unsigned char *message, uint8_t array_length){
  digitalWrite(_UART_Select_S, HIGH);
  delay(5);
  uint8_t i = 0;
  for(i; i < array_length; i++){
    Serial.write(message[i]);
  }
  Serial.write(0x00);
  delay(5);
  digitalWrite(_UART_Select_S, LOW);
}

void MiP::getMessage(unsigned char *answer, int byteCount) {
  digitalWrite(_UART_Select_R, HIGH);
  for (int i = 0; i < byteCount; i++) {
    answer[i] = Serial.read();
    //Serial.println(answer[i]);
  }
  digitalWrite(_UART_Select_R, LOW);
}
