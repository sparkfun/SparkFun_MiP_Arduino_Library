/*
 * MiP_commands.h
 *
 *  Created on: Jul 1, 2014
 *      Author: caseykuhns
 */

#ifndef MIP_COMMANDS_H_
#define MIP_COMMANDS_H_

#include "MiP_sounds.h"

class MiP {
public:
	MiP();
	~MiP();
	void reset(void);
	void playSingleSound(Sounds sound, unsigned char delay, unsigned char loop_count);
	//void playMultipleSounds();
	void setMiPPosition(unsigned char position);
	//TODO void distanceDrive();
	void timeDrive(char direction, char speed, unsigned char time);
	void turnAngle(char direction, char speed, unsigned char angle);
	void continuousDrive(char direction, char speed);
	void setGameMode(char mode);
	void stop(void);

	void getStatus(void); //TODO create struct for status

	void standUp(char state);
	unsigned char getWeightStatus(void);
	void requestChestLED(void); //TODO create struct for chest LEDS
	void setChestLED(unsigned char red, unsigned char green, unsigned char blue);
	void flashChestLED(unsigned char red, unsigned char green, unsigned char blue, unsigned char time_on, unsigned char time_off);

};

#endif /* MIP_COMMANDS_H_ */
