/*
 * MiP_commands.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: caseykuhns
 */

#include "MiP_commands.h"
#include "MiP_Parameters.h"
#include <avr/io.h>

MiP::MiP(int8_t UART_Select) {
  _UART_Select = UART_Select; //bring variable into a private variable.
  pinMode(_UART_Select, OUTPUT);
}

MiP::~MiP() {
	// TODO Auto-generated destructor stub
}

void MiP::init(void){
  Serial.begin(115200);
  delay(1000);
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
   data_array[3] = 0x01; // Clocwise    
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
void MiP::turnAngle(int8_t direction, int8_t speed, uint8_t angle){

}
void MiP::continuousDrive(int8_t direction, int8_t speed){

}
void MiP::stop(void){

}

void MiP::setGameMode(Game mode){
	uint8_t data_array[2];
	data_array[0] = 0x76;
	data_array[1] = mode;

	sendMessage(data_array);
}

void MiP::getStatus(void){

} //TODO create struct for status

void MiP::standUp(int8_t state){

}
uint8_t MiP::getWeightStatus(void){
	uint8_t weight;

	return PLAY_BACK;
}

void MiP::requestChestLED(void){

} //TODO create struct for chest LEDs
*/
void MiP::setChestLED(uint8_t red, uint8_t green, uint8_t blue){
  uint8_t array_length = 5;
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
void MiP::setHeadLEDs(void){

}
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

void MiP::setVolume(int8_t volume){

}
int8_t MiP::getVolume(){
	int8_t volume;

	return volume;
}

void MiP::setClapDetection(int8_t mode){

}
int8_t MiP::getClapDetection(void){

}
void MiP::getClapsRecieved(int8_t* claps){

}
*/

// Private Functions


int8_t MiP::sendMessage(unsigned char *message, uint8_t array_length){
  digitalWrite(_UART_Select, HIGH);
  delay(5);
  uint8_t i = 0;
  for(i; i < array_length; i++){
    Serial.write(message[i]);
  }
  Serial.write(0x00);
  delay(5);
  digitalWrite(_UART_Select, LOW);
  return 0;
}

