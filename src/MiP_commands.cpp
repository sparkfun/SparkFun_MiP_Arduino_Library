/*
* MiP_commands.cpp
**  Created on: Jul 1, 2014
*      Author: caseykuhns
*/

#include "MiP_commands.h"
#include "MiP_Parameters.h"
#include <avr/io.h>

MiP::MiP(int8_t UART_Select_S) {
	_UART_Select_S = UART_Select_S; //bring variable into a private variable.
	pinMode(_UART_Select_S, OUTPUT);
	pinMode(13, OUTPUT);
	
	volume = -1;
	voiceHardwareVersion = -1;
	hardwareVersion = -1;
	gameMode = INVALID;
	isSWVersionSet = false;
	softwareVersion = {0x00, 0x00, 0x00, 0x00};
	areHeadLEDsSet = false;
	headLEDs = {0x00, 0x00, 0x00, 0x00};
	isChestLEDSet = false;
	chestLEDs = {0x00, 0x00, 0x00, 0x00, 0x00};
	
	debug = false;
}

MiP::~MiP() {
}

void MiP::init(void){
	Serial.begin(BAUD_RATE);
	uint8_t initString[] = {0xFF};
	sendMessage(initString, 1);
	setVolume(0);
	while (Serial.available()){
		Serial.read();
	}
}

void MiP::playSound(Sounds MiPSound, uint8_t delayInterval, uint8_t repeatTimes){
	uint8_t message[] = {PLAY_SOUND, MiPSound, delayInterval, repeatTimes};

	sendMessage(message, 4);
}

void MiP::setPosition(Position pose){
	uint8_t message[] = {SET_POSITION, pose};

	sendMessage(message, 2);
}

void MiP::driveForward(int16_t distance, int16_t angle){
	uint8_t arrayLength = 7;
	uint8_t message[arrayLength];
	message[0] = DRIVE_DISTANCE; //Drive Distance Command

	if(distance >=0){
		message[1] = 0x00; // Forward
	}
	else{
		message[1] = 0x01; // Reverse
	};
	message[2] = uint8_t(distance);

	if(angle >=0){
		message[3] = 0x01; // Clockwise
	}
	else{
		message[3] = 0x00; // Counter Clockwise
	};

	angle = angle & 0x7fff; //remove direction from angle
	if(angle >= 360)angle = 360; //cap rotation at 360 deg

	message[4] = uint8_t(angle >> 8);
	message[5] = uint8_t(angle);

	sendMessage(message, arrayLength);
}

void MiP::driveForward(int8_t direction, int8_t speed, uint8_t time){

}

void MiP::turnLeft(int8_t direction, int8_t angle, uint8_t speed){
	uint8_t arrayLength = 4;
	uint8_t message[arrayLength];

	if(direction == 0){
		message[0] = TURN_LEFT_BY_ANGLE;
	}
	else if(direction == 1){
		message[0] = TURN_RIGHT_BY_ANGLE;
	}
	else{
		message[0] = TURN_LEFT_BY_ANGLE; //Default to Left Turn
	};

	if(angle >= 255)angle = 255; //cap angle at max of 255
	message[1] = uint8_t(angle); //Angle in intervals of 5 degrees

	if(speed >= 24)speed = 24; //cap speed at max of 24
	message[2] = uint8_t(speed);

	sendMessage(message, arrayLength);
}

void MiP::turnRight(int8_t direction, int8_t angle, uint8_t speed){
	uint8_t arrayLength = 4;
	uint8_t message[arrayLength];

	if(direction == 0){
		message[0] = TURN_LEFT_BY_ANGLE;
	}
	else if(direction == 1){
		message[0] = TURN_RIGHT_BY_ANGLE;
	}
	else{
		message[0] = TURN_LEFT_BY_ANGLE; //Default to Left Turn
	};

	if(angle >= 255)angle = 255; //cap angle at max of 255
	message[1] = uint8_t(angle); //Angle in intervals of 5 degrees

	if(speed >= 24)speed = 24; //cap speed at max of 24
	message[2] = uint8_t(speed);

	sendMessage(message, arrayLength);
}

void MiP::sleep(){
	uint8_t message[] = {SLEEP};

	sendMessage(message, 1);
}

void MiP::driveContinuous(int8_t direction, int8_t speed){

}

void MiP::stop(void){
	uint8_t message[] = {STOP};
	
	sendMessage(message, 1);
}

void MiP::setGameMode(GameMode mode){
	uint8_t message[] = {SET_GAME_MODE, (uint8_t)mode};

	sendMessage(message, 2);
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
	uint8_t arrayLength = 2;
	uint8_t message[arrayLength];
	message[0] = GET_UP;
	message[1] = state;

	sendMessage(message, arrayLength);
}

uint8_t MiP::getWeightStatus(void){
	uint8_t weight;

	return -1;
}

// Doesn't work yet.
ChestLEDs MiP::getChestLEDs(void){
	if(!isChestLEDSet){
		uint8_t question[] = {GET_CHEST_LED};		
		uint8_t answer[6];
		int iteration = 0;
		
		while (!isChestLEDSet && iteration < MAX_RETRIES) {
			debugOutput(iteration);
			sendMessage(question, 1);
			if (Serial.available() == 12) {
				getMessage(answer, 6);
				if (answer[0] == GET_CHEST_LED) {
					debugOutput("Right!");
					isChestLEDSet = true;
					chestLEDs.red = answer[1];
					chestLEDs.green = answer[2];
					chestLEDs.blue = answer[3];
					chestLEDs.timeOn = answer[4];
					chestLEDs.timeOff = answer[5];
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
	return chestLEDs;
}

void MiP::setChestLED(uint8_t red, uint8_t green, uint8_t blue){
	uint8_t message[] = {SET_CHEST_LED, red, green, blue};

	sendMessage(message, 4);
	isChestLEDSet = false;
}

void MiP::flashChestLED(uint8_t red, uint8_t green, uint8_t blue, uint8_t timeOn, uint8_t timeOff){
	uint8_t message[] = {FLASH_CHEST_LED, red, green, blue, timeOn, timeOff};
	
	sendMessage(message, 6);
	isChestLEDSet = false;
}

void MiP::setHeadLEDs(uint8_t light1, uint8_t light2, uint8_t light3, uint8_t light4){
	uint8_t message[] = {SET_HEAD_LEDS, light1, light2, light3, light4};

	sendMessage(message, 5);
	areHeadLEDsSet = false;
}

HeadLEDs MiP::getHeadLEDs(void){
	if(!areHeadLEDsSet){
		uint8_t question[] = {GET_HEAD_LEDS};		
		uint8_t answer[5];
		int iteration = 0;
		
		while (!areHeadLEDsSet && iteration < MAX_RETRIES) {
			debugOutput(iteration);
			sendMessage(question, 1);
			if (Serial.available() == 10) {
				getMessage(answer, 5);
				if (answer[0] == GET_HEAD_LEDS) {
					debugOutput("Right!");
					areHeadLEDsSet = true;
					headLEDs.light1 = answer[1];
					headLEDs.light2 = answer[2];
					headLEDs.light3 = answer[3];
					headLEDs.light4 = answer[4];
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
	return headLEDs;
}

uint32_t MiP::getOdometerReading(void){
	uint32_t odometer;

	return odometer;
}

void MiP::resetOdometer(void){
	uint8_t message[] = {RESET_ODOMETER};

	sendMessage(message, 1);
}

void MiP::enableGestureDetect(void){
	uint8_t message[] = {DETECTION_MODE, ENABLE_GESTURE_DETECT};
	
	sendMessage(message, 2);
}

void MiP::disableGestureDetect(void){
	uint8_t message[] = {DETECTION_MODE, DISABLE_GESTURE_DETECT};
	
	sendMessage(message, 2);
}

bool MiP::isGestureDetectEnabled(void){
	return false;
}

void MiP::enableRadarMode(void){
	uint8_t message[] = {DETECTION_MODE, ENABLE_RADAR_DETECT};
	
	sendMessage(message, 2);
}

void MiP::disableRadarMode(void){
	uint8_t message[] = {DETECTION_MODE, DISABLE_RADAR_DETECT};
	
	sendMessage(message, 2);
}

void MiP::setDetectionMode(int8_t mode, int8_t power){

}

void MiP::getDetectionMode(void){

}

bool MiP::isShakeDetected(void){
	return false;
}

void MiP::setIRControlEnabled(){
	uint8_t message[] = {SET_IR_CONTROL_STATUS, ENABLE_IR_CONTROL};
	
	sendMessage(message, 2);	
}

void MiP::setIRControlDisabled(){
	uint8_t message[] = {SET_IR_CONTROL_STATUS, DISABLE_IR_CONTROL};
	
	sendMessage(message, 2);		
}

bool MiP::isIRControlEnabled(void){
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

SoftwareVersion MiP::getSoftwareVersion(void){
	if(!isSWVersionSet){
		uint8_t question[] = {GET_SW_VERSION};		
		uint8_t answer[5];
		int iteration = 0;
		
		while (!isSWVersionSet && iteration < MAX_RETRIES) {
			debugOutput(iteration);
			sendMessage(question, 1);
			if (Serial.available() == 10) {
				getMessage(answer, 5);
				if (answer[0] == GET_SW_VERSION) {
					debugOutput("Right!");
					isSWVersionSet = true;
					softwareVersion.year = answer[1];
					softwareVersion.month = answer[2];
					softwareVersion.day = answer[3];
					softwareVersion.number = answer[4];
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
				getMessage(answer, 3);
				if (answer[0] == GET_VOICE_HW_VERSION) {
					debugOutput("Right!");
					voiceHardwareVersion = answer[1];
					hardwareVersion = answer[2];
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
		uint8_t answer[2];
		int iteration = 0;
		
		while ((hardwareVersion == -1) && iteration < MAX_RETRIES) {
			debugOutput(iteration);
			sendMessage(question, 1);
			if (Serial.available() == 6) {
				getMessage(answer, 3);
				if (answer[0] == GET_HW_VERSION) {
					debugOutput("Right!");
					voiceHardwareVersion = answer[1];
					hardwareVersion = answer[2];
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
	return hardwareVersion;
}

void MiP::setVolume(uint8_t newVolume){
	uint8_t message[] = {SET_VOLUME, newVolume};

	sendMessage(message, 2);
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
	uint8_t message[] = {SET_CLAP_DETECTION, 0x01};

	sendMessage(message, 2);
}

void MiP::setClapDetectionDisabled(void){
	uint8_t message[] = {SET_CLAP_DETECTION, 0x00};

	sendMessage(message, 2);	
}

bool MiP::isClapDetectionEnabled(void){
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

int8_t MiP::getClapsDetected(){
	uint8_t message[] = {GET_CLAP_DETECTED};
	
	sendMessage(message, 1);
	// Not finished.
}

void MiP::disconnectApp(){
	uint8_t message[] = {SET_DISCONNECT_APP};
	
	sendMessage(message, 1);
}

void MiP::enableDebug(){
	debug = true;
}

void MiP::disableDebug(){
	debug = false;
}

void MiP::simpleAck() {
	digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(1000);              // wait for a second
	digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	delay(1000);              // wait for a second
}

// Private Functions

void MiP::sendMessage(unsigned char *message, uint8_t arrayLength){
	digitalWrite(_UART_Select_S, HIGH);
	delay(5);
	uint8_t i = 0;
	for(i; i < arrayLength; i++){
		Serial.write(message[i]);
	}
	Serial.write(0x00);
	delay(5);
	digitalWrite(_UART_Select_S, LOW);
	delay(5);
}

void MiP::getMessage(unsigned char *answer, int byteCount) {
	debugOutput("start getMessage");
	bool validChar = true;
	uint8_t recvHigh;
	uint8_t recvLow;
	int i = 0;
	// Exit loop if at any time an invalid ASCII character is detected.
	while(i < byteCount && validChar) {
		debugOutput("Avail: ");
		debugOutput(Serial.available());
		debugOutput(" loop");
		recvHigh = Serial.read();		
		if(!isASCIIEncodedHex(recvHigh)){
			debugOutput("  Invalid MSB: ");
			debugOutput(recvHigh);
			validChar = false;
		}
		else {
			// Do some masking to get rid of the ASCII encoding.
			recvHigh = recvHigh & 0x0F;
			// Shift some bits to prepare for concatenation of bytes.
			recvHigh = recvHigh << 4;
			recvLow = Serial.read();
			if(!isASCIIEncodedHex(recvLow)){
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
				debugOutput(recvHigh);
			}
		}
		i++;
	}
	// If there was an invalid char during this getMessage, let caller know with -1.
	if(!validChar){
		for(int j = 0; j < byteCount; j++){
			answer[j] = -1;
		}
		// Wipe out the remaining bytes available for read.
		while(Serial.available()){
			Serial.read();
		}
	}
	debugOutput("end getMessage");
}

void MiP::debugOutput(unsigned char *message){
	if(debug){
		Serial.println((unsigned char&)message);
		Serial.flush();
	}
}

void MiP::debugOutput(const char *message){
	if(debug){
		Serial.println(message);
		Serial.flush();	
	}
}

void MiP::debugOutput(uint8_t message){
	if(debug){
		Serial.println(message, HEX);
		Serial.flush();	
	}
}

bool MiP::isASCIIEncodedHex(uint8_t inValue){
	if((inValue & 0xF0) != 0x30 || (inValue & 0xF0) != 0x40){
		return true;
	}
	return false;
}