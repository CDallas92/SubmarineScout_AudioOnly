/*
	File:	AudioRenderable.hpp
	Version:	1.0 
	Date:	7th January 2013.
	Author:	Allan C. Milne.

	Exposes:	AudioRenderable.

	Description:
	An intrface that defines a function to allow an object to be rendered in a game frame loop.
	also exposes a funtion to check if the object was created correctly.
*/

#ifndef __AUDIORENDERABLE_1_0__
#define __AUDIORENDERABLE_1_0__

class AudioRenderable
{
public:

	//--- pure virtual function to be called each time round a game frame loop.
	//--- Parameter is time in seconds since last call.
	virtual void RenderAudio (const float deltaTime) = 0;

	//--- Indicates if object was created correctly.
	virtual bool IsOk () const = 0;

	//--- virtual destructor to ensure derived class destructor gets called.
	virtual ~AudioRenderable() { }

};	// end AudioRenderable class

#endif