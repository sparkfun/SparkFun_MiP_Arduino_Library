/*
 * MiP_Parameters.h
 *
 *  Created on: Jul 3, 2014
 *      Author: caseykuhns
 */

#ifndef MIP_PARAMETERS_H_
#define MIP_PARAMETERS_H_

enum SetPosition{
  FACEUP = 0,
  FACEDOWN
};

enum Parameters{
		BATTERY_LEVEL,
		ORIENTATION,

		WEIGHT_UPDATE,

		CHEST_LED_RED,
		CHEST_LED_GREEN,
		CHEST_LED_BLUE,
		CHEST_LED_TIME_ON,
		CHEST_LED_TIME_OFF,

		HEAD_LED_1,
		HEAD_LED_2,
		HEAD_LED_3,
		HEAD_LED_4,

		RADAR_MODE,

		MIP_DETECTION_MODE,
		MIP_DETECCTION_ID,
		MIP_DETECTION_POWER,

		IR_CONTROL,

		VOLUME,

		VERSION_VOICECHIP,
		VERSION_HARDWARE,
};


enum Game{
	APP = 1,
	PLAY_BACK,
	TRACKING,
	DANCE,
	//DEFAULT,
	STACK,
	TRICKS,
	ROAM
};

enum GetUp{
	FRONT,
	BACK,
	BOTH
};

enum LEDState{
	OFF,
	ON,
	BLINK_SLOW,
	BLINK_FAST
};

enum LEDColor{
	RED,
	GREEN,
	BLUE
};

enum Commands{
	GET_VOLUME = 0x16
};

enum Comms{
	MAX_RETRIES = 10
};
#endif /* MIP_PARAMETERS_H_ */
