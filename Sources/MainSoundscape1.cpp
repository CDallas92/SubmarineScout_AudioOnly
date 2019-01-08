/*
	File:	MainSoundscape1.cpp
	Version:	1.0
	Date: 7th January 2013.
	Author:	Allan C. Milne

	Uses:	WinCore, Soundscape1.

	Description:
	The main code file containing the Windows application entry point
	based on the Main_WinCore.cpp skeleton provided in the WinCore package.
	*	See WinCore.hpp/.cpp for details of the framework abstraction.

	the code instantiates a Soundscape1 object that implements the IGameCore interface and passes it to WinCore.
	this will then call the IGameCore processing methods.

*/

#include <windows.h>

#include "WinCore.hpp"
using AllanMilne::WinCore;

#include "Soundscape1.hpp"

//=== Application entry point. ===
int WINAPI WinMain (HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	WinCore* windowApp;
	IGameCore* game;

	game = new Soundscape1();
	windowApp = new WinCore (game);

	//--- Create the application window and application-wide resources.
	bool ok = windowApp->Initialize (
				" XAudio2 Soundscape 1. ",		// Windows title bar text.
				800, 600,			// width x height
				false,				// use full screen; change to true if windowed is required.
				hinstance );
	if (!ok) {
		MessageBox (NULL, TEXT ("Error occurred while initializing WinCore; application aborted."), TEXT ("Initialize - FAILED"), MB_OK | MB_ICONERROR );
		return 0;
	}

	//--- start the Windows messsage loop and associated game frame processing.
	windowApp->Start ();

	delete windowApp;
	windowApp = NULL;
	delete game;
	game = NULL;
	return 0;
} // end WinMain function.

// end of code.