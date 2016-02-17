/*
 * MiP_commands.h
 *
 *  Created on: Jul 1, 2014
 *      Author: caseykuhns
 */

#ifndef MIP_COMMANDS_H_
#define MIP_COMMANDS_H_

#include "MiP_sounds.h"
#include "MiP_Parameters.h"
#include <Arduino.h>
struct _LEDColor;
class MiP {
  public:
    MiP(int8_t UART_Select_S, int8_t UART_Select_R);
    ~MiP();  
	typedef struct _LEDColor
	{
		int red;
		int green;
		int blue;
	} LEDColor;

    void init(void);

    void playSingleSound(Sounds MiPSound);
    void setPosition(SetPosition pose);
    void distanceDrive(int16_t distance, int16_t angle);


//    void timeDrive(int8_t direction, int8_t speed, uint8_t time);
    void turnAngle(int8_t direction, int8_t speed, uint8_t angle);
/*
    void continuousDrive(int8_t direction, int8_t speed);
*/
    void stop(void);
/*
    void setGameMode(int8_t mode);

    void getStatus(Parameter parameter); //TODO create struct for status
*/
    void standUp(int8_t state);
	/*
    //uint8_t getWeightStatus(void);
	*/
    void requestChestLED(void); //TODO create struct for chest LEDs
    
    void setChestLED(uint8_t red, uint8_t green, uint8_t blue);
    /*
    void flashChestLED(uint8_t red, uint8_t green, uint8_t blue, uint8_t time_on, uint8_t time_off);
	*/
    void setHeadLEDs(uint8_t light1, uint8_t light2, uint8_t light3, uint8_t light4);
	/*
    void getHeadLEDs(void); //TODO create struct for head LEDs

    uint32_t getOdometer(void);
    void resetOdometer(void);

    //void setGestureDetectMode(int8_t mode);
    //int8_t getGestureDetectMode(void);
    int8_t getGestureDetect(void);
    int8_t getRadarDetect(void);

    //void setDetectionMode(int8_t mode, int8_t power);
    //void getDetectionMode(void); //TODO create structure for detection mode

    int8_t getShakeDetection(void);

    void setIRcontrol(int8_t mode);
    int8_t getIRcontrol(void);

    int8_t ping(void);

    void setEEPROMData(int8_t addr, uint8_t data);
    uint8_t getEEPROMData(int8_t addr);

    //void getSoftwareVersion(int8_t* version);
    //void getHardwareVersion(int8_t* version);
*/
    void setVolume(int8_t volume);
	
    int8_t getVolume();
/*
    void setClapDetection(int8_t mode);
    int8_t getClapDetection(void);
    void getClapsRecieved(int8_t* claps);
*/

  private:
    int8_t _UART_Select_S; //Variable to store UART Select pin. Default is D2
	int8_t _UART_Select_R; //Variable to store UART Select pin. Default is D2
    void sendMessage(unsigned char *message, uint8_t array_length);
	void getMessage(unsigned char *answer, int byteCount);
	uint8_t answer[8]; //Variable to store returned data from MiP
	void getData();
};



#endif /* MIP_COMMANDS_H_ */
