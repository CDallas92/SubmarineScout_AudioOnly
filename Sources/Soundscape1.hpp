#ifndef __SOUNDSCAPE1_HPP_1_0__
#define __SOUNDSCAPE1_HPP_1_0__

#include <xaudio2.h>
#include <X3DAudio.h>
#include <list>
using std::list;

#include "IGameCore.hpp"
using AllanMilne::IGameCore;
//using AllanMilne::Audio::XACore;
//using AllanMilne::Audio::XASound;

class XACore;
class XASound;
class AudioRenderable;

class Soundscape1 : public IGameCore {
public:

	//=== Implementation of the IGameCore interface.

	//--- create and initialize XACore and the sounds to be played.
	//--- since this game has no graphics the argument is not used.
	//--- this will create all the sound elements.
	bool SetupGame (HWND aWindow);

	//--- process a single game frame.
	//--- this will call the relevant AudioRender() functions.
	void ProcessGameFrame (const float deltaTime);

	//--- Release all XACore, XASound and other audio resources.
	void CleanupGame ();

	//--- helper functions for X3DAudio.
	void InitX3DAudio ();
	void InitListener();
	void InitEmitter();
	void InitDSP();
	void Apply3D();

	void MainLogic();
	//=== Game specific behaviour.

	//--- Default constructor.
	Soundscape1 () 
	: mXACore (NULL), mSound(NULL), mMovementIncrement(5.0f), mOrientation(0)
	{ 
		// Zero all structs used in 3D.
		memset ((void*)&mX3DInstance, 0, sizeof (X3DAUDIO_HANDLE));
		memset ((void*)&mListener, 0, sizeof (X3DAUDIO_LISTENER));
		memset ((void*)&mEmitter, 0, sizeof (X3DAUDIO_EMITTER));
		memset ((void*)&mDSPSettings, 0, sizeof (X3DAUDIO_DSP_SETTINGS));
	} // end constructor.


private:
	
	//--- the XAudio2 engine encapsulation.
	XACore *mXACore;

	//--- the sound to be played.
	XASound *mSound;
	
	//--- X3DAudio elements.
	X3DAUDIO_HANDLE mX3DInstance;
	X3DAUDIO_LISTENER mListener;
	X3DAUDIO_EMITTER mEmitter;
	X3DAUDIO_DSP_SETTINGS mDSPSettings; 

	// value to use for each movement step.
	float mMovementIncrement;

	// Current orientation from 0 (North through 2(East) to 7 (North-west).
	int mOrientation;
	
	//--- Helper function to create the XASound object from MySound.wav with filter enabled.
	XASound* LoadXASound ();
	


	//--- the sounds to be played.
	XASound *mAmbience;
	XASound *mSubMove;
	XASound *mSubHalt;
	XASound *alarmForward;
	XASound *alarmLeft;
	XASound *alarmRight;
	XASound *playerDeath;
	XASound *gameWin;
	list<AudioRenderable*> mRenderedSounds;
	
}; // end Soundscape1 class.

#endif