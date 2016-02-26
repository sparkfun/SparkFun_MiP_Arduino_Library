/*
* MiP_commands.cpp
**  Created on: Jul 1, 2014
*      Author: caseykuhns
*/

#include "MiP_commands.h"
#include "MiP_Parameters.h"
#include <avr/io.h>

MiP::MiP(int8_t UART_Select_S/*, int8_t UART_Select_R*/) {
	_UART_Select_S = UART_Select_S; //bring variable into a private variable.
	//_UART_Select_R = UART_Select_R; //bring variable into a private variable.
	pinMode(_UART_Select_S, OUTPUT);
	//pinMode(_UART_Select_R, INPUT);
	
	volume = -1;
	voiceHardwareVersion = -1;
	hardwareVersion = -1;
	gameMode = INVALID;
	softwareVersion = {-1,-1,-1,-1};
	
	debug = false;
}

MiP::~MiP() {
}

void MiP::init(void){
	Serial.begin(BAUD_RATE);
	uint8_t initString[] = {0xFF};
	//uint8_t array_length = 9;
	//uint8_t data_array[] = "TTM:OK\r\n";
	//sendMessage(data_array, array_length);
	sendMessage(initString, 1);
	setVolume(0);
}

void MiP::playSound(Sounds MiPSound, uint8_t delayInterval, uint8_t repeatTimes){
	uint8_t array_length = 4;
	uint8_t data_array[array_length];
	data_array[0] = PLAY_SOUND; //Play sound command from WowWee documentation
	data_array[1] = MiPSound; //User selected sound
	data_array[2] = delayInterval;
	data_array[3] = repeatTimes;

	sendMessage(data_array, array_length);
}

void MiP::setPosition(Position pose){
	uint8_t data_array[] = {SET_POSITION, pose};

	sendMessage(data_array, 2);
}

void MiP::driveForward(int16_t distance, int16_t angle){
	uint8_t array_length = 7;
	uint8_t data_array[array_length];
	data_array[0] = DRIVE_DISTANCE; //Drive Distance Command

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
}

void MiP::driveForward(int8_t direction, int8_t speed, uint8_t time){

}

void MiP::turnLeft(int8_t direction, int8_t angle, uint8_t speed){
	uint8_t array_length = 4;
	uint8_t data_array[array_length];

	if(direction == 0){
		data_array[0] = TURN_LEFT_BY_ANGLE;
	}
	else if(direction == 1){
		data_array[0] = TURN_RIGHT_BY_ANGLE;
	}
	else{
		data_array[0] = TURN_LEFT_BY_ANGLE; //Default to Left Turn
	};

	if(angle >= 255)angle = 255; //cap angle at max of 255
	data_array[1] = uint8_t(angle); //Angle in intervals of 5 degrees

	if(speed >= 24)speed = 24; //cap speed at max of 24
	data_array[2] = uint8_t(speed);

	sendMessage(data_array, array_length);
}

void MiP::turnRight(int8_t direction, int8_t angle, uint8_t speed){
	uint8_t array_length = 4;
	uint8_t data_array[array_length];

	if(direction == 0){
		data_array[0] = TURN_LEFT_BY_ANGLE;
	}
	else if(direction == 1){
		data_array[0] = TURN_RIGHT_BY_ANGLE;
	}
	else{
		data_array[0] = TURN_LEFT_BY_ANGLE; //Default to Left Turn
	};

	if(angle >= 255)angle = 255; //cap angle at max of 255
	data_array[1] = uint8_t(angle); //Angle in intervals of 5 degrees

	if(speed >= 24)speed = 24; //cap speed at max of 24
	data_array[2] = uint8_t(speed);

	sendMessage(data_array, array_length);
}

void MiP::sleep(){
	uint8_t data_array[] = {SLEEP};

	sendMessage(data_array, 1);
}

void MiP::driveContinuous(int8_t direction, int8_t speed){

}

void MiP::stop(void){
	uint8_t data_array[] = {STOP};
	
	sendMessage(data_array, 1);
}

void MiP::setGameMode(GameMode mode){
	uint8_t data_array[] = {SET_GAME_MODE, (uint8_t)mode};

	sendMessage(data_array, 2);
}

GameMode MiP::getGameMode(void){
	if(gameMode == INVALID){
		uint8_t question[] = {GET_GAME_MODE};		
		uint8_t answer[2];
		int iteration = 0;

		while ((gameMode == INVALID) && iteration < MAX_RETRIES) {
			sendMessage(question, 1);
			if (Serial.available() == 4) {
				getMessage(answer, 2);
				if (answer[0] == GET_GAME_MODE) {
					gameMode = (GameMode)answer[1];
				}
			}
			else
			{
				while (Serial.available())
				Serial.read();
			}
			iteration++;
		}
	}	
	return gameMode;
}

void MiP::getStatus(void){

}

void MiP::stand(GetUp state){
	uint8_t array_length = 2;
	uint8_t data_array[array_length];
	data_array[0] = GET_UP;
	data_array[1] = state;

	sendMessage(data_array, array_length);
}

uint8_t MiP::getWeightStatus(void){
	uint8_t weight;

	return -1;
}

// Doesn't work yet.
void MiP::getChestLED(unsigned char *answer){
	uint8_t question[] = {GET_CHEST_LED};
	int iteration = 0;
	/*
while ((MIN_VOLUME > answerVal || answerVal > MAX_VOLUME) && iteration < MAX_RETRIES) {
	sendMessage(question, 1);
	if (Serial.available() == 4) {
	getMessage(answer, 2);
	if (answer[0] == GET_CHEST_LED) {
		answerVal = answer[1];
	}
	}
	else
	{
	while (Serial.available())
		Serial.read();
	}
	iteration++;
}// One more value check in case last try produced invalid result.
if (0 > answerVal || answerVal > 7)
	return -1;

return answerVal;
*/
}

void MiP::setChestLED(uint8_t red, uint8_t green, uint8_t blue){
	uint8_t array_length = 4;
	uint8_t data_array[array_length];
	data_array[0] = SET_CHEST_LED;
	data_array[1] = red;
	data_array[2] = green;
	data_array[3] = blue;

	sendMessage(data_array, array_length);
}

void MiP::flashChestLED(uint8_t red, uint8_t green, uint8_t blue, uint8_t time_on, uint8_t time_off){

}

void MiP::setHeadLEDs(uint8_t light1, uint8_t light2, uint8_t light3, uint8_t light4){
	uint8_t array_length = 5;
	uint8_t data_array[array_length];
	data_array[0] = SET_HEAD_LEDS;
	data_array[1] = light1;
	data_array[2] = light2;
	data_array[3] = light3;
	data_array[4] = light4;

	sendMessage(data_array, array_length);
}

void MiP::getHeadLEDs(void){

}

uint32_t MiP::getOdometerReading(void){
	uint32_t odometer;

	return odometer;
}

void MiP::resetOdometer(void){
	uint8_t data_array[] = {RESET_ODOMETER};

	sendMessage(data_array, 1);
}

void MiP::enableGestureDetect(void){
	uint8_t data_array[] = {DETECTION_MODE, ENABLE_GESTURE_DETECT};
	
	sendMessage(data_array, 2);
}

void MiP::disableGestureDetect(void){
	uint8_t data_array[] = {DETECTION_MODE, DISABLE_GESTURE_DETECT};
	
	sendMessage(data_array, 2);
}

boolean MiP::isGestureDetectEnabled(void){
	return false;
}

void MiP::enableRadarMode(void){
	uint8_t data_array[] = {DETECTION_MODE, ENABLE_RADAR_DETECT};
	
	sendMessage(data_array, 2);
}

void MiP::disableRadarMode(void){
	uint8_t data_array[] = {DETECTION_MODE, DISABLE_RADAR_DETECT};
	
	sendMessage(data_array, 2);
}

void MiP::setDetectionMode(int8_t mode, int8_t power){

}

void MiP::getDetectionMode(void){

}

boolean MiP::isShakeDetected(void){
	return false;
}

void MiP::setIRControlEnabled(){
	uint8_t data_array[] = {SET_IR_CONTROL_STATUS, ENABLE_IR_CONTROL};
	
	sendMessage(data_array, 2);	
}

void MiP::setIRControlDisabled(){
	uint8_t data_array[] = {SET_IR_CONTROL_STATUS, DISABLE_IR_CONTROL};
	
	sendMessage(data_array, 2);		
}

boolean MiP::isIRControlEnabled(void){
	int answerVal = -1;
	uint8_t answer[2];
	uint8_t question[] = {GET_IR_CONTROL_STATUS};
	int iteration = 0;
	while ((answerVal != 0x00 || answerVal != 0x01) && iteration < MAX_RETRIES) {
		sendMessage(question, 1);
		if (Serial.available() == 4) {
			getMessage(answer, 2);
			if (answer[0] == GET_IR_CONTROL_STATUS && answer[1] == 0x00) {
				return false;
			}
			else{
				return true;
			}
		}
		else
		{
			while (Serial.available())
			Serial.read();
		}
		iteration++;
	}
}

void MiP::setUserData(int8_t addr, uint8_t data){

}

uint8_t MiP::getUserData(int8_t addr){
	uint8_t data;
	return data;
}

SoftwareVersion MiP::getSoftwareVersion(){
	debugOutput(softwareVersion.year);
	if(softwareVersion.year == -1){
		uint8_t question[] = {GET_SW_VERSION};		
		uint8_t answer[5];
		int iteration = 0;
		
		while ((softwareVersion.year == -1) && iteration < MAX_RETRIES) {
			debugOutput(iteration);
			sendMessage(question, 1);
			if (Serial.available()) {
				getMessage(answer, 5);
				if (answer[0] == GET_SW_VERSION) {
					debugOutput("Right!");
					softwareVersion.year = answer[1];
				} else {
					debugOutput(":(");
					debugOutput(answer[0]);
				}
			}
			else
			{
				while (Serial.available()){
					Serial.read();
				}
			}
			iteration++;
		}
	}
	return softwareVersion;
}

int8_t MiP::getVoiceHardwareVersion(){
	if(voiceHardwareVersion == -1){
		uint8_t question[] = {GET_VOICE_HW_VERSION};		
		uint8_t answer[2];
		int iteration = 0;
		
		while ((voiceHardwareVersion == -1) && iteration < MAX_RETRIES) {
			debugOutput(iteration);
			sendMessage(question, 1);
			if (Serial.available() == 6) {
				getMessage(answer, 2);
				if (answer[0] == GET_VOICE_HW_VERSION) {
					debugOutput("Right!");
					voiceHardwareVersion = answer[1];
				} else {
					debugOutput(":(");
					debugOutput(answer[0]);
				}
			}
			else
			{
				while (Serial.available()){
					Serial.read();
				}
			}
			iteration++;
		}
	}
	return voiceHardwareVersion;
}

int8_t MiP::getHardwareVersion(){
	if(hardwareVersion == -1){
		uint8_t question[] = {GET_HW_VERSION};		
		uint8_t answer[3];
		int iteration = 0;
		
		while ((hardwareVersion == -1) && iteration < MAX_RETRIES) {
			debugOutput(iteration);
			sendMessage(question, 1);
			if (Serial.available() == 6) {
				getMessage(answer, 3);
				if (answer[0] == GET_HW_VERSION) {
					debugOutput("Right!");
					hardwareVersion = answer[2];
				} else {
					debugOutput(":(");
					debugOutput(answer[0]);
				}
			}
			else
			{
				debugOutput("no data");
				//debugOutput(Serial.available());
				while (Serial.available()) {
					Serial.read();
				}
			}
			iteration++;
		}
	}
	return hardwareVersion;
}

void MiP::setVolume(uint8_t newVolume){
	uint8_t data_array[] = {SET_VOLUME, newVolume};

	sendMessage(data_array, 2);
}

int8_t MiP::getVolume(){
	if(volume == -1){
		uint8_t answer[2];
		uint8_t question[] = {GET_VOLUME};
		int iteration = 0;
		while ((MIN_VOLUME > volume || volume > MAX_VOLUME) && iteration < MAX_RETRIES) {
			debugOutput(iteration);
			sendMessage(question, 1);
			if (Serial.available() == 4) {
				getMessage(answer, 2);
				if (answer[0] == GET_VOLUME) {
					volume = answer[1];
				}
			}
			else
			{
				while (Serial.available()){
					Serial.read();
				}
			}
			iteration++;
		}
	}
	// One more value check in case last try produced invalid result.
	if (0 > volume || volume > 7){
		volume = -1;
	}

	return volume;
}

void MiP::setClapDetectionEnabled(void){
	uint8_t array_length = 2;
	uint8_t data_array[array_length];
	data_array[0] = SET_CLAP_DETECTION;
	data_array[1] = 0x01;

	sendMessage(data_array, array_length);
}

boolean MiP::isClapDetectionEnabled(void){
	int answerVal = -1;
	uint8_t answer[2];
	uint8_t question[] = {GET_CLAP_DETECTION};
	int iteration = 0;
	while ((answerVal != 0x00 || answerVal != 0x01) && iteration < MAX_RETRIES) {
		debugOutput(iteration);
		sendMessage(question, 1);
		if (Serial.available() == 4) {
			getMessage(answer, 2);
			if (answer[0] == GET_CLAP_DETECTION) {
				answerVal = answer[1];
			}
		}
		else
		{
			while (Serial.available())
			Serial.read();
		}
		iteration++;
	}
	// One more value check in case last try produced invalid result.
	if (answerVal != 0x00 || answerVal != 0x01){
		return -1;		
	}

	if(answerVal == 0x00){
		return false;
	}
	else {
		return true;
	}
}

void MiP::getClapsRecieved(int8_t* claps){

}

void MiP::disconnectApp(){
	uint8_t array_length = 1;
	uint8_t data_array[array_length];
	data_array[0] = SET_DISCONNECT_APP;
	sendMessage(data_array, array_length);
}

void MiP::enableDebug(){
	debug = true;
}

void MiP::disableDebug(){
	debug = false;
}

// Private Functions

void MiP::sendMessage(unsigned char *message, uint8_t array_length){
	digitalWrite(_UART_Select_S, HIGH);
	delay(5);
	uint8_t i = 0;
	for(i; i < array_length; i++){
		Serial.write(message[i]);
	}
	//Serial.write(0x00);
	delay(5);
	digitalWrite(_UART_Select_S, LOW);
	delay(10);
}

void MiP::getMessage(unsigned char *answer, int byteCount) {
	debugOutput("start getMessage");
	boolean validChar = true;
	uint8_t recvHigh;
	uint8_t recvLow;
	int i = 0;
	//digitalWrite(_UART_Select_S, LOW);
	// Exit loop if at any time an invalid ASCII character is detected.
	while(i < byteCount && validChar) {
		debugOutput(" loop");
		delay(100);
		recvHigh = Serial.read();
		
		// A valid ASCII character will have MSB=0011b (0x30)
		if((recvHigh & 0xF0) != 0x30){
			debugOutput("  Invalid MSB: ");
			debugOutput(recvHigh);
			validChar = false;
		}
		else {
			// Do some masking to get rid of the ASCII encoding.
			recvHigh = recvHigh & 0x0F;
			// Shift some bits to prepare for concatenation of bytes.
			recvHigh = recvHigh << 4;
			delay(100);
			recvLow = Serial.read();
			//Serial.flush();
			// Again, check for an invalid character.
			if((recvLow & 0xF0) != 0x30){
				debugOutput("  Invalid LSB: ");
				debugOutput(recvLow);
				validChar = false;
			}
			else {
				// Do some masking to get rid of the ASCII encoding.
				recvLow  = recvLow & 0x0F;
				// Put the two together (concatenation).
				recvHigh = recvHigh + recvLow;
				answer[i] = recvHigh;
				debugOutput("  Complete byte");
			}
		}
		i++;
		delay(500);
	}
	//digitalWrite(_UART_Select_S, HIGH);
	// If there was an invalid char during this getMessage, let caller know with -1.
	if(!validChar){
		for(int j = 0; j < byteCount; j++){
			answer[j] = -1;
		}
		while(Serial.available()){
			Serial.read();
		}
	}
	debugOutput("end getMessage");
}

void MiP::debugOutput(unsigned char *message){
	if(debug){
		Serial.println((unsigned char&)message);
		delay(500);
	}
}

void MiP::debugOutput(const char *message){
	if(debug){
		Serial.println(message);
		delay(500);	
	}
}

void MiP::debugOutput(uint8_t message){
	if(debug){
		Serial.println(message, HEX);
		delay(500);	
	}
}