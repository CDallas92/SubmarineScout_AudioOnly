#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;

#include "Explosion.h"

void Explosion::RenderAudio (const float deltaTime)
{
	//Defines Explosions
	static const float minVolume = 0.1f;		//Level To Adjust Volume By.
	static const float maxVolume = 1.0f;		//When To Lower Volume.
	static const float volumeUp = 1.25f;		//Set Volume 5/4.
	static const float volumeDown = 0.8f;		//Decrease Volume 4/5.
	static const float pauseTime = 1.0f;		//Change Volume Every Second.

	if (!IsOk()) return;

	mElapsedTime += deltaTime;
	if (mElapsedTime > pauseTime)
	{
		mElapsedTime = 0.0f;
		float volume = mExplosion->GetVolume();
		if (volume < minVolume) 
			mVolumeAdjustment = volumeUp;		//Move Explosion Nearer Listener
		else if (volume >maxVolume) 
			mVolumeAdjustment = volumeDown;		//Move Explosion Further Away From Listener
		mExplosion->AdjustVolume(mVolumeAdjustment);
	}
}

//Object Created Properly?
inline bool Explosion::IsOk () const { return (mExplosion != NULL); }

//Explosion Constructor
Explosion::Explosion (XACore *aCore)
	: mExplosion (NULL), mElapsedTime (0.0f), mVolumeAdjustment (1.1f)
{
	mExplosion = aCore->CreateSound("Sounds/underwater_explosion.wav");
}

//Explosion Destructor
Explosion::~Explosion()
{
	if (mExplosion !=NULL)
	{
		delete mExplosion;
		mExplosion = NULL;
	}
}

//Starts Explosions
void Explosion::Play ()
{
	mExplosion->SetLooped(true);
	mExplosion->Play(0);
}