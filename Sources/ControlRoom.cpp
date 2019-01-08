#include <time.h>

// following define required in order to use the semitones/frequency ratio helper functions.
#define XAUDIO2_HELPER_FUNCTIONS
#include<XAudio2.h>

#include "XACore.hpp"
#include "WaveFileManager.hpp"
#include "PCMWave.hpp"
#include "WaveFmt.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::WaveFileManager;
using AllanMilne::Audio::PCMWave;
using AllanMilne::Audio::WaveFmt;

#include "ControlRoom.h"

//--- internal helper function prototypes.
bool LoadCRoomFile (XAUDIO2_BUFFER *aBuffer, WAVEFORMATEX *aFormat);
int GetRandomPauses();


//--- called every game frame to check if random time has passed and frogs should play.
//--- Parameter is time in seconds since last call.
void ControlRoom::RenderAudio (const float deltaTime)
{
	// Guard against invalid sound elements.
	if (!IsOk()) return;
	mElapsedTime += deltaTime;
	// do nothing if random pause time has not passed.
	if (mElapsedTime < mPause) return;
	// play the harmonised frog sounds.
	for (int i=0; i<3; ++i) {
		mCRoomVoices[i]->SubmitSourceBuffer(&mCRoomData);

		//Randomise Sound Of Control Room
		mCRoomVoices[i]->SetVolume(0.65);

	}
	// Restart timing for the next play.
	mPause = GetRandomPauses();
		mElapsedTime = 0.0f;
} // end Renderaudio function.

//--- Indicate if object has been created correctly.
inline bool ControlRoom::IsOk() const { return mOk; }

//--- constructor loads and sets up the sound elements.
//--- Each step is checked and if not ok the constructor returns, with MOk being false.
	ControlRoom::ControlRoom(XACore *aCore)
	: mOk (false), mPause (0), mElapsedTime (0.0f), mCRoomVoices (NULL)
{
	// load the frogs.wav file.
	WAVEFORMATEX wFmt;
	if (!LoadCRoomFile (&mCRoomData, &wFmt)) return;		// file not loaded correctly.

	// Create the array of source voices for the harmonised frogs.
	mCRoomVoices = new IXAudio2SourceVoice*[3];
	IXAudio2 *engine = aCore->GetEngine();
	if (engine == NULL) return;		// XAudio2 engine was not created correctly.
	HRESULT hr = 0;
	for (int i=0; i<3; ++i)
	{
		// with 2 parameters the source voices are all routed to the mastering voice.
		hr= hr & engine->CreateSourceVoice (&(mCRoomVoices[i]), &wFmt );
	}
	if( FAILED( hr ) ) 
	{
		delete [] mCRoomVoices;
		return;
	}
	// Now set up frog harmonies.
	float frequencyRatio;
	mCRoomVoices[0]->GetFrequencyRatio(&frequencyRatio);
	float semitones = XAudio2FrequencyRatioToSemitones (frequencyRatio);
	//mFrogVoices[1]->SetFrequencyRatio(XAudio2SemitonesToFrequencyRatio (semitones+4));
	//mFrogVoices[2]->SetFrequencyRatio(XAudio2SemitonesToFrequencyRatio(semitones+7));
	// Add these voices to the audio processing graph.
	for (int i=0; i<3; ++i) {
		mCRoomVoices[i]->Start();
	}
	
	// See the random number genrator and get the first random pause time..
	srand (time (NULL));
	mPause = GetRandomPauses();



	// Everything has been created correctly.
	mOk = true;
} // end Frogs constructor function.

//--- release the frogs audio resources.
//--- Note we must not delete the source voices here; they are owned by the XAudio2 engine and will be deleted by it.
ControlRoom::~ControlRoom()
{
	if (mCRoomVoices != NULL) {
		for (int i=0; i<3; ++i) {
			if (mCRoomVoices[i]!= NULL) {
				mCRoomVoices[i]->Stop();
				mCRoomVoices[i]->FlushSourceBuffers();
				mCRoomVoices[i]->DestroyVoice();
			}
		} // end for each source voice.
		delete [] mCRoomVoices;
	} // end if have an array.
} // end Frogs destructor function.


//=== private helper functions.

//--- Load wave file and assign XAudio2 buffer and wave format struct to reference parameters.
//--- returns false if any error in loading .wav file.
bool LoadCRoomFile (XAUDIO2_BUFFER *aBuffer, WAVEFORMATEX *aFormat)
{
	PCMWave *wave = WaveFileManager::GetInstance().LoadWave ("Sounds/controlroom_misc.wav");
	// If the file was not loaded correctly then can go no further.
	if (wave->GetStatus() != PCMWave::OK) { return false; }
	// Initialise the XAudio2 buffer struct from the PCMWave object.
	memset ((void*)aBuffer, 0, sizeof (XAUDIO2_BUFFER));
	aBuffer->AudioBytes = wave->GetDataSize ();
	aBuffer->pAudioData = (BYTE*)(wave->GetWaveData ());
	// copy windows wave format struct from the PCMWave struct field.
	memset ((void*)aFormat, 0, sizeof (WAVEFORMATEX));
	memcpy_s ((void*)aFormat, sizeof (WaveFmt), (void*)&(wave->GetWaveFormat()), sizeof (WaveFmt));
	return true;
	} // end LoadFrogsFile function.

//--- Get a random pause time between frogs playing; units are seconds.
int GetRandomPauses ()
{
	// Time range to use.
	static const int minPause = 7;
	static const int range = 10;

	return rand() % range + minPause;
} // end GetRandomPauses function.

//=== end of code.