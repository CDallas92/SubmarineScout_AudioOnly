#include "GamePlay.h"
#include "windows.h"

/**********GAMEPLAY MAP*************/
/*****   O = Ship  X = Mine  *******/
/*****		[] Beacon	      ******/
/***********************************/
/**	____|____|___[]____|____|____|**/
/**	____|__X_|____|____|____|__X_|**/
/**	____|____|____|__X_|____|____|**/
/**	____|____|____|____|____|____|**/
/**	____|__X_|____|____|____|____|**/
/**	____|____|____|____|__X_|____|**/
/**__X__|____|__X_|__X_|____|____|**/
/**	____|____|____|____|____|____|**/
/**	____|____|__O_|____|____|__X_|**/
/***********************************/
/**** Target: Reach The Beacon *****/
/**********Avoid The Mines**********/


GamePlay::GamePlay()
{
	Delay = 0;

	//Initial Movement Completed To 0
	MovedUp = false; MovedLeft = false; MovedRight = false; MovedDown = false;


	//Initially Turn Off Alarm
	alarm_left1 = false; alarm_right1 = false; alarm_forward1 = false;
	alarm_left2 = false; alarm_right2 = false; alarm_forward2 = false;
	alarm_left3 = false; alarm_right3 = false; alarm_forward3 = false;
	alarm_left4 = false; alarm_right4 = false; alarm_forward4 = false;
	alarm_left5 = false; alarm_right5 = false; alarm_forward5 = false;
	alarm_left6 = false; alarm_right6 = false; alarm_forward6 = false;
	alarm_left7 = false; alarm_right7 = false; alarm_forward7 = false;
	alarm_left8 = false; alarm_right8 = false; alarm_forward8 = false;
	alarm_left9 = false; alarm_right9 = false; alarm_forward9 = false;

	//Start Position
	 PlayerXPosition = 3;
	 PlayerYPosition = 1;
	//Mine Position
	 Mine1XPosition = 6;
	 Mine1YPosition = 1;
	 Mine1Danger = false;
	//Mine 2 Position
	 Mine2XPosition = 3;
	 Mine2YPosition = 3;
	 Mine2Danger = false;
	 //Mine 3 Position
	 Mine3XPosition = 1;
	 Mine3YPosition = 4;
	 Mine3Danger = false;
	 //Mine 4 Position
	 Mine4XPosition = 5;
	 Mine4YPosition = 4;
	 Mine4Danger = false;
	//Mine 5 Position
	 Mine5XPosition = 2;
	 Mine5YPosition = 6;
	 Mine5Danger = false;
	//Mine 6 Position
	 Mine6XPosition = 4;
	 Mine6YPosition = 8;
	 Mine6Danger = false;
	//Mine 7 Position
	 Mine7XPosition = 2;
	 Mine7YPosition = 9;
	 Mine7Danger = false;
	//Mine 8 Position
	 Mine8XPosition = 6;
	 Mine8YPosition = 9;
	 Mine8Danger = false;
 	//Mine 9 Position
	 Mine9XPosition = 4;
	 Mine9YPosition = 3;
	 Mine9Danger = false;
}

int GamePlay::PlayLeft()
{
	//If Any Left Sound Playing, Play Sound 
	if(alarm_left1 == true)	{return 1;}else
	if(alarm_left2 == true)	{return 1;}else
	if(alarm_left3 == true)	{return 1;}else
	if(alarm_left4 == true)	{return 1;}else
	if(alarm_left5 == true)	{return 1;}else
	if(alarm_left6 == true)	{return 1;}else
	if(alarm_left7 == true)	{return 1;}else
	if(alarm_left8 == true)	{return 1;}else
	if(alarm_left9 == true)	{return 1;}else
	return 0;
}

int GamePlay::PlayRight()
{
	if(alarm_right1 == true){return 1;}else
	if(alarm_right2 == true){return 1;}else
	if(alarm_right3 == true){return 1;}else
	if(alarm_right4 == true){return 1;}else
	if(alarm_right5 == true){return 1;}else
	if(alarm_right6 == true){return 1;}else
	if(alarm_right7 == true){return 1;}else
	if(alarm_right8 == true){return 1;}else
	if(alarm_right9 == true){return 1;}else
	return 0;
}

int GamePlay::PlayForward()
{
	if(alarm_forward1 == true){return 1;}else
	if(alarm_forward2 == true){return 1;}else
	if(alarm_forward3 == true){return 1;}else
	if(alarm_forward4 == true){return 1;}else
	if(alarm_forward5 == true){return 1;}else
	if(alarm_forward6 == true){return 1;}else
	if(alarm_forward7 == true){return 1;}else
	if(alarm_forward8 == true){return 1;}else
	if(alarm_forward9 == true){return 1;}else
	return 0;
}

void GamePlay::PlayerMovement()
{
	//Basic WASD Player Movement
	//Move Forward If W Pressed -- 75000 Loops Of Logic Until Button Can Be Pressed Again  /// 0x57
	if(GetAsyncKeyState(VK_UP) && Delay > 75000){PlayerYPosition++; Delay = 0; OutputDebugString("Moved Forward");MovedUp = true;}
	//Move Left If A Pressed -- 75000 Loops Of Logic Until Button Can Be Pressed Again  /// 0x41
	if(GetAsyncKeyState(VK_LEFT) && Delay > 75000){PlayerXPosition--; Delay = 0; OutputDebugString("Moved Left");MovedLeft = true;}
	//Move Right If D Pressed -- 75000 Loops Of Logic Until Button Can Be Pressed Again  /// 0x44
	if(GetAsyncKeyState(VK_RIGHT) && Delay > 75000){PlayerXPosition++; Delay = 0; OutputDebugString("Moved Right");MovedRight = true;}
	//Move Back If S Pressed -- 75000 Loops Of Logic Until Button Can Be Pressed Again  /// 0x53
	if(GetAsyncKeyState(VK_DOWN) && Delay > 75000){PlayerYPosition--; Delay = 0; OutputDebugString("Moved Back");MovedDown = true;}

}

void GamePlay::GamePlayLogic()
{
	//Test For Mine 1 Proximity
	Prox1Test();
	//Test For Mine 2 Proximity
	Prox2Test();
	//Test For Mine 3 Proximity
	Prox3Test();	
	//Test For Mine 4 Proximity
	Prox4Test();
	//Test For Mine 5 Proximity
	Prox5Test();
	//Test For Mine 6 Proximity
	Prox6Test();
	//Test For Mine 7 Proximity
	Prox7Test();
	//Test For Mine 8 Proximity
	Prox8Test();
	//Test For Mine 9 Proximity
	Prox9Test();

	//Move Player Based On Keyboard Input
	PlayerMovement();

	//Value To Make Sure Player Doesnt Fly Forward With One Button Press
	Delay++;

}

int GamePlay::GameWin()
{
	//If Player Reaches End Of Level
	if(PlayerYPosition > 9){return 1;}

	return 0;
}

void GamePlay::ResetPlayer()
{
	//Resets Player Position If They Die
	PlayerXPosition = 3;
	PlayerYPosition = 1;
}

int GamePlay::MineHit()
{
	//Check If Player Position Is The Same As Mine Position 
	if(PlayerXPosition == 6 && PlayerYPosition == 1){return 1;}
	if(PlayerXPosition == 3 && PlayerYPosition == 3){return 1;}
	if(PlayerXPosition == 1 && PlayerYPosition == 4){return 1;}
	if(PlayerXPosition == 5 && PlayerYPosition == 4){return 1;}
	if(PlayerXPosition == 2 && PlayerYPosition == 6){return 1;}
	if(PlayerXPosition == 4 && PlayerYPosition == 8){return 1;}
	if(PlayerXPosition == 2 && PlayerYPosition == 9){return 1;}
	if(PlayerXPosition == 6 && PlayerYPosition == 9){return 1;}
	if(PlayerXPosition == 4 && PlayerYPosition == 3){return 1;}

	return 0;
}

int GamePlay::StopAlarms()
{
	if(Mine1Danger == false && Mine2Danger == false && Mine3Danger == false && Mine4Danger == false && Mine5Danger == false && Mine6Danger == false && Mine7Danger == false && Mine8Danger == false)
	{
		return 1;
	}

	//If Alarm Still Running Continue Playing
	return 0;
}

void GamePlay::Prox1Test()
{
	//Is Player To Left Of Mine 1
	if(PlayerXPosition == 5 && PlayerYPosition == 1)
	{alarm_right1 = true;}else{alarm_right1 = false;}

	//Is Player To Right Of Mine 1 
	if(PlayerXPosition == 7 && PlayerYPosition == 1)
	{alarm_left1 = true;}else{alarm_left1 = false;}
}

void GamePlay::Prox2Test()
{
	//Is Player To Left Of Mine 2 
	if(PlayerXPosition == 2 && PlayerYPosition == 3)
	{alarm_right2 = true;}else{alarm_right2 = false;}

	//Is Player To Right Of Mine 2
	if(PlayerXPosition == 4 && PlayerYPosition == 3)
	{alarm_left2 = true;}else{alarm_left2 = false;}

	//Is Player In Front Of Mine 2
	if(PlayerXPosition == 3 && PlayerYPosition == 2)
	{alarm_forward2 = true;}else{alarm_forward2 = false;}
}

void GamePlay::Prox3Test()
{
	//Is Player To Left Of Mine 3 
	if(PlayerXPosition == 0 && PlayerYPosition == 4)
	{alarm_right3 = true;}else{alarm_right3 = false;}

	//Is Player To Right Of Mine 3 
	if(PlayerXPosition == 2 && PlayerYPosition == 4)
	{alarm_left3 = true;}else{alarm_left3 = false;}

	//Is Player In Front Of Mine 3
	if(PlayerXPosition == 1 && PlayerYPosition == 3)
	{alarm_forward3 = true;}else{alarm_forward3 = false;}
}

void GamePlay::Prox4Test()
{
	//Is Player To Left Of Mine 4 
	if(PlayerXPosition == 4 && PlayerYPosition == 4)
	{alarm_right4 = true;}else{alarm_right4 = false;}

	//Is Player To Right Of Mine 4 
	if(PlayerXPosition == 6 && PlayerYPosition == 4)
	{alarm_left4 = true;}else{alarm_left4 = false;}

	//Is Player In Front Of Mine 4
	if(PlayerXPosition == 5 && PlayerYPosition == 3)
	{alarm_forward4 = true;}else{alarm_forward4 = false;}
}

void GamePlay::Prox5Test()
{
	//Is Player To Left Of Mine 5 
	if(PlayerXPosition == 1 && PlayerYPosition == 6)
	{alarm_right5 = true;}else{alarm_right5 = false;}

	//Is Player To Right Of Mine 5 
	if(PlayerXPosition == 3 && PlayerYPosition == 6)
	{alarm_left5 = true;}else{alarm_left5 = false;}

	//Is Player In Front Of Mine 5
	if(PlayerXPosition == 2 && PlayerYPosition == 5)
	{alarm_forward5 = true;}else{alarm_forward5 = false;}
}

void GamePlay::Prox6Test()
{
	//Is Player To Left Of Mine 6 
	if(PlayerXPosition == 3 && PlayerYPosition == 8)
	{alarm_right6 = true;}else{alarm_right6 = false;}

	//Is Player To Right Of Mine 6 
	if(PlayerXPosition == 5 && PlayerYPosition == 8)
	{alarm_left6 = true;}else{alarm_left6 = false;}

	//Is Player In Front Of Mine 6
	if(PlayerXPosition == 4 && PlayerYPosition == 7)
	{alarm_forward6 = true;}else{alarm_forward6 = false;}
}

void GamePlay::Prox7Test()
{
	//Is Player To Left Of Mine 7 
	if(PlayerXPosition == 1 && PlayerYPosition == 9)
	{alarm_right7 = true;}else{alarm_right7 = false;}

	//Is Player To Right Of Mine 7 
	if(PlayerXPosition == 3 && PlayerYPosition == 9)
	{alarm_left7 = true;}else{alarm_left7 = false;}

	//Is Player In Front Of Mine 7
	if(PlayerXPosition == 2 && PlayerYPosition == 8)
	{alarm_forward7 = true;}else{alarm_forward7 = false;}
}

void GamePlay::Prox8Test()
{
	//Is Player To Left Of Mine 8 
	if(PlayerXPosition == 5 && PlayerYPosition == 9)
	{alarm_right8 = true;}else{alarm_right8 = false;}

	//Is Player To Right Of Mine 8 
	if(PlayerXPosition == 7 && PlayerYPosition == 9)
	{alarm_left8 = true;}else{alarm_left8 = false;}

	//Is Player In Front Of Mine 8
	if(PlayerXPosition == 6 && PlayerYPosition == 8)
	{alarm_forward8 = true;}else{alarm_forward8 = false;}
}

void GamePlay::Prox9Test()
{
	//Is Player To Right Of Mine 9 
	if(PlayerXPosition == 5 && PlayerYPosition == 3)
	{alarm_left9 = true;}else{alarm_left9 = false;}

	//Is Player In Front Of Mine 9
	if(PlayerXPosition == 4 && PlayerYPosition == 2)
	{alarm_forward9 = true;}else{alarm_forward9 = false;}
}

void GamePlay::ResetMovements()
{
	MovedUp = false;
	MovedDown = false;
	MovedLeft = false;
	MovedRight = false;
}