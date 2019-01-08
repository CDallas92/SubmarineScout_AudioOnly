
#ifndef __CONTROLROOM_H_1_1__
#define __CONTROLROOM_H_1_1__

#include<XAudio2.h>

#include "AudioRenderable.hpp"

class XACore;

class ControlRoom : public AudioRenderable
{
public:
	//--- called every game frame to check if random time has passed and frogs should play.
	//--- Parameter is time in seconds since last call.
	void RenderAudio (const float deltaTime);

	//--- Indicate if object has been created correctly.
	bool IsOk() const;

	//--- constructor loads and sets up the sound elements.
	ControlRoom(XACore *aCore);
	virtual ~ControlRoom();

private:
	bool mOk;							// indicates if sound and voices have been set up correctly.
	int mPause;							// number of seconds to pause between playing the frogs.
	float mElapsedTime;					// Time since frogs last played.
	XAUDIO2_BUFFER mCRoomData;			// the sound sample data for the base frogs croak.
	IXAudio2SourceVoice **mCRoomVoices;	// array of pointer to source voices for different harmonies.

};	// end Frogs class.

#endif