
#ifndef __EXPLOSION_HPP_1_1__
#define __EXPLOSION_HPP_1_1__

#include "AudioRenderable.hpp"

class XACore;
class XASound;

class Explosion : public AudioRenderable
{
public:
	//Call Every Frame To Animate Explosion
	//Time In Seconds Since Last Called
	void RenderAudio (const float deltaTime);

	//Indicate If Created Object Correctly
	bool IsOk () const;

	//Explosion Constructor
	Explosion (XACore *aCore);
	virtual ~Explosion();

	//Play Explosion
	void Play();

private:
	XASound *mExplosion;			// Explosion sound.
	float mElapsedTime;				//Time Last Explosion Movement Applied.
	float mVolumeAdjustment;		//Amount To Adjust Volume Per Movement.
	
};

#endif