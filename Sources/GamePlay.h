#ifndef __GAMEPLAY_H_1_1__
#define __GAMEPLAY_H_1_1__

#include "AudioRenderable.hpp"

class GamePlay
{
public:

	//Constructor
    GamePlay::GamePlay();

	//Main GamePlay Logic Loop
	void GamePlayLogic();

	//Move The Submarine On The Grid
	void PlayerMovement();

	//Handle Player Hitting A Mine
	int MineHit();

	//Play Game Over / Win Noises
	int GameWin();

	//Reset Player Position If They Die
	void ResetPlayer();

	//Proximity Alert1
	void Prox1Test();
	//Proximity Alert2
	void Prox2Test();
	//Proximity Alert3
	void Prox3Test();
	//Proximity Alert4
	void Prox4Test();
	//Proximity Alert5
	void Prox5Test();
	//Proximity Alert6
	void Prox6Test();
	//Proximity Alert7
	void Prox7Test();
	//Proximity Alert8
	void Prox8Test();
	//Proximity Alert9
	void Prox9Test();
	//Proximity Handler
	void ProxHandler();
	//Pan Handling
	void PanHandling();
	//Stop Alarm If Not Nearby
	int StopAlarms();

	//Signal Main To Realise Movement Has Been Completed
	void ResetMovements();
	bool MovedUp;
	bool MovedLeft;
	bool MovedRight;
	bool MovedDown;

	//Player Positions
	int PlayerXPosition;
	int PlayerYPosition;
	//Mine 1 Positions
	int Mine1XPosition;
	int Mine1YPosition;
	bool Mine1Danger;
	//Mine 2 Positions
	int Mine2XPosition;
	int Mine2YPosition;
	bool Mine2Danger;
	//Mine 3 Positions
	int Mine3XPosition;
	int Mine3YPosition;
	bool Mine3Danger;	
	//Mine 4 Positions
	int Mine4XPosition;
	int Mine4YPosition;
	bool Mine4Danger;	
	//Mine 5 Positions
	int Mine5XPosition;
	int Mine5YPosition;
	bool Mine5Danger;	
	//Mine 6 Positions
	int Mine6XPosition;
	int Mine6YPosition;
	bool Mine6Danger;	
	//Mine 7 Positions
	int Mine7XPosition;
	int Mine7YPosition;
	bool Mine7Danger;	
	//Mine 8 Positions
	int Mine8XPosition;
	int Mine8YPosition;
	bool Mine8Danger;
	//Mine 9 Positions
	int Mine9XPosition;
	int Mine9YPosition;
	bool Mine9Danger;

	//Ensure Button Holding Doesnt Move Loads
	int Delay;

	//Decide Which Side To Pan
	void SelectPanSide();
	bool alarm_left1;	bool alarm_right1;	bool alarm_forward1;
	bool alarm_left2;	bool alarm_right2;	bool alarm_forward2;
	bool alarm_left3;	bool alarm_right3;	bool alarm_forward3;
	bool alarm_left4;	bool alarm_right4;	bool alarm_forward4;
	bool alarm_left5;	bool alarm_right5;	bool alarm_forward5;
	bool alarm_left6;	bool alarm_right6;	bool alarm_forward6;
	bool alarm_left7;	bool alarm_right7;	bool alarm_forward7;
	bool alarm_left8;	bool alarm_right8;	bool alarm_forward8;
	bool alarm_left9;	bool alarm_right9;	bool alarm_forward9;

	int PlayLeft();
	int PlayRight();
	int PlayForward();

};
#endif