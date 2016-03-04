/*
* MiP_commands.h
**  Created on: Jul 1, 2014
*      Author: caseykuhns
*/

#ifndef MIP_COMMANDS_H_
#define MIP_COMMANDS_H_

#include "MiP_sounds.h"
#include "MiP_Parameters.h"
#include <Arduino.h>
using namespace std;

struct SoftwareVersion {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t number;
};

struct ChestLEDs {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t timeOn;
	uint8_t timeOff;
};

struct HeadLEDs {
	uint8_t light1;
	uint8_t light2;
	uint8_t light3;
	uint8_t light4;
};

class MiP {

public:
	
	MiP(int8_t UART_Select_S);
	
	~MiP();

	void init(void);

	void playSound(Sounds MiPSound, uint8_t delayInterval, uint8_t repeatTimes);
	
	void setPosition(Position pose);
	
	void driveForward(int16_t distance, int16_t angle);
	
	void driveForward(int8_t direction, int8_t speed, uint8_t time);
	
	void turnLeft(int8_t direction, int8_t speed, uint8_t angle);
	
	void turnRight(int8_t direction, int8_t speed, uint8_t angle);

	void driveContinuous(int8_t direction, int8_t speed);

	void setGameMode(GameMode mode);
	
	GameMode getGameMode(void);
	
	void stop(void);
	
	void getStatus(void);

	void stand(GetUp state);
	
	uint8_t getWeightStatus(void);
	
	ChestLEDs getChestLEDs(void);
	
	void setChestLED(uint8_t red, uint8_t green, uint8_t blue);
	
	void flashChestLED(uint8_t red, uint8_t green, uint8_t blue, uint8_t timeOn, uint8_t timeOff);
	
	void setHeadLEDs(uint8_t light1, uint8_t light2, uint8_t light3, uint8_t light4);
	
	HeadLEDs getHeadLEDs(void);

	uint32_t getOdometerReading(void);
	
	void resetOdometer(void);
	
	void enableGestureDetect(void);
	
	void disableGestureDetect(void);
	
	bool isGestureDetectEnabled(void);
	
	void enableRadarMode(void);
	
	void disableRadarMode(void);

	void setDetectionMode(uint8_t detectionMode, uint8_t tPower);
	
	void getDetectionMode(void);

	void setDetectionMode(int8_t mode, int8_t power);
	
	int8_t getMiPDetected(void);
	
	bool isShakeDetected(void);

	void setIRControlEnabled(void);

	void setIRControlDisabled(void);
	
	bool isIRControlEnabled(void);

	void sleep(void);

	void disconnectApp();

	void forceBLEDisconnect();
	
	void setUserData(int8_t addr, uint8_t data);
	
	uint8_t getUserData(int8_t addr);

	SoftwareVersion getSoftwareVersion(void);

	int8_t getVoiceHardwareVersion(void);
	
	int8_t getHardwareVersion(void);

	void setVolume(uint8_t volume);
	
	int8_t getVolume(void);

	void sendIRDongleCode(int8_t input);
	
	void recvIRDongleCode(void);

	void setClapDetectionEnabled(void);
	
	void setClapDetectionDisabled(void);

	bool isClapDetectionEnabled(void);
	
	void setClapDelayTime(uint8_t delayTime);
	
	uint8_t getClapDelayTime();
	
	int8_t getClapsDetected();
	
	void enableDebug();
	
	void disableDebug();
	
	void simpleAck(); // Another attempt at debugging.  How about flashing the LED instead
	// of writing to serial?
	
private:
	
	SoftwareVersion softwareVersion;
	
	HeadLEDs headLEDs;
	
	ChestLEDs chestLEDs;
	
	int8_t voiceHardwareVersion;
	
	uint8_t hardwareVersion;

	int8_t volume;
	
	GameMode gameMode;
	
	int8_t _UART_Select_S; //Variable to store UART Select pin. Default is D2
	
	bool debug;
	
	void debugOutput(unsigned char *message);
	
	void debugOutput(const char *message);
	
	void debugOutput(uint8_t message);
	
	void sendMessage(unsigned char *message, uint8_t array_length);
	
	void getMessage(unsigned char *answer, int byteCount);
	
	uint8_t answer[8]; //Variable to store returned data from MiP
	
	bool isASCIIEncodedHex(uint8_t inValue);
	
	bool isSWVersionSet;
	
	bool areHeadLEDsSet;
	
	bool isChestLEDSet;
	
};

#endif /* MIP_COMMANDS_H_ */
