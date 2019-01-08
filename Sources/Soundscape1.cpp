
#include <windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include <cmath>
#include<list>
using std::list;

#include "XACore.hpp"
#include "XASound.hpp"
using AllanMilne::Audio::XACore;
using AllanMilne::Audio::XASound;

#include "Soundscape1.hpp"
#include "Explosion.h"
#include "ControlRoom.h"
#include "GamePlay.h"

#include "WaveFileManager.hpp"
#include "PCMWave.hpp"
#include "WaveFmt.hpp"
using namespace AllanMilne::Audio;


//Instance Of GamePlay
GamePlay GPlay;

//Detect If Ship Is Moving
bool ShipMoving = false;

//=== use anonymous namespace to define array of compass point orientation vectors scoped for this file.
namespace {
	// Calculate value to normalize (1,0,1) type 45 degree cardinal points.
	const float coordValue = 1.0f / sqrt(2.0f);
//=== vectors are normalised.
X3DAUDIO_VECTOR compassVectors[8] = {
	{0.0f, 0.0f, 1.0f}, {coordValue, 0.0f, coordValue}, {1.0f, 0.0f, 0.0f}, {coordValue, 0.0f, -coordValue},
	{0.0f, 0.0f, -1.0f}, {-coordValue, 0.0f, -coordValue}, {-1.0f, 0.0f, 0.0f}, {-coordValue, 0.0f, coordValue}
};
} 

//=== Implementation of the IGameCore interface.

//--- create and initialize XACore and the sounds to be played.
//--- Since this game has no graphics the HWND argument is not used.
//--- this will create all the sound elements and play the rain & bird sounds.
bool Soundscape1::SetupGame (HWND aWindow)
{
	// set up the XAudio2 engine and mastering voice; check if ok.
	mXACore = new XACore ();
	if (mXACore->GetEngine() == NULL || mXACore->GetMasterVoice() == NULL) {
		return false;
	}

	// Create the Underwater Ambience sound object; check if ok.
	mAmbience = mXACore->CreateSound ("Sounds/underwater_ambience.wav");
	if (mAmbience == NULL) {
		MessageBox (NULL, "Error loading Underwater Ambience.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mAmbience->SetLooped(true);

	// Create the Sub Movement sound object; check if ok.
	mSubMove = mXACore->CreateSound ("Sounds/sub_halt.wav");
	if (mSubMove == NULL) {
		MessageBox (NULL, "Error loading Sub Movement.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}

	// Create the Sub Slowing Down sound object; check if ok.
	mSubHalt = mXACore->CreateSound ("Sounds/sub_halt.wav");
	if (mSubHalt == NULL) {
		MessageBox (NULL, "Error loading Sub Movement.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}

	// Create the Forward Alarm sound object; check if ok.
	alarmForward = mXACore->CreateSound ("Sounds/danger_mid.wav");
	if (alarmForward == NULL) {
	MessageBox (NULL, "Error loading ForwardAlarm.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
	return false;
	}

	// Create the Left Alarm Left object; check if ok.
	alarmLeft = mXACore->CreateSound ("Sounds/danger_left.wav");
	if (alarmLeft == NULL) {
		MessageBox (NULL, "Error loading Left Alarm.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}

	// Create the Alarm Right sound object; check if ok.
	alarmRight = mXACore->CreateSound ("Sounds/danger_right.wav");
	if (alarmRight == NULL) {
		MessageBox (NULL, "Error loading Alarm Right.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}

	// Create the Player Death sound object; check if ok.
	playerDeath = mXACore->CreateSound ("Sounds/dead_player.wav");
	if (playerDeath == NULL) {
		MessageBox (NULL, "Error loading playerDeath.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}


	// Create Game Win Noise sound object; check if ok.
	gameWin = mXACore->CreateSound ("Sounds/game_win.wav");
	if (gameWin == NULL) {
		MessageBox (NULL, "Error loading game_win.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}

	// Create the renderable objects and add to the renderable component list.
	Explosion *explosion = new Explosion(mXACore);
	if (!explosion->IsOk())
	{
		MessageBox (NULL, "Error loading Explosion.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		delete mAmbience;
		delete explosion;
		return false;
	}

		ControlRoom *cRoom= new ControlRoom(mXACore);
	if (!cRoom->IsOk())
	{
		MessageBox (NULL, "Error loading CRoom.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		delete mAmbience;
		return false;
	}

	//Set Sounds
	//Random Control Room Noises
	mRenderedSounds.push_back((AudioRenderable*)cRoom);
	//Nearby Explosions
	mRenderedSounds.push_back((AudioRenderable*)explosion);
	
	// Play the Underwater Ambience sounds; 
	mAmbience->Play(0);
	// Play Nearby Explosions At Varying Volumes 
	explosion->Play();

	// Create the sound object; check if ok.
	mSound = LoadXASound ();
	if (mSound == NULL) {
		MessageBox (NULL, "Error loading MySound.wav", TEXT ("SetupGame() - FAILED"), MB_OK | MB_ICONERROR );
		return false;
	}
	mSound->SetLooped(true);

	InitX3DAudio();
	InitListener();
	InitEmitter();
	InitDSP();

	Apply3D();		// raw .wav volume is not consistent with (0,0,0).
	mSound->Play(0);

	return true;		// All has been setup without error.
} // end SetupGame function.

//--- process a single game frame.
//--- this will call the relevant AudioRender() functions.
void Soundscape1::ProcessGameFrame (const float deltaTime)
{
	//If Any Movement Pressed
	if(GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_RIGHT)){ShipMoving = true;}else{ShipMoving = false;}

	//Make Sure Sound Doesn't Cut Out - Fade Out Movement
	//If Moving Move Sub Sound
	if(ShipMoving == true && mSubHalt->IsPlaying() == false)
	{mSubMove->Play(1);}

	//Always Be Slowing Down To Promote Slow Gameplay 

	//Handle Gameplay Logic
	MainLogic();

	list<AudioRenderable*>::const_iterator iter;
	for (iter=mRenderedSounds.begin(); iter!=mRenderedSounds.end(); ++iter)
	{
		(*iter)->RenderAudio(deltaTime);
	}
} // end ProcessGameFrame function.

//--- Release all XACore and XASound resources.
//--- Note the order of destruction is important; XAudio2 destroys voices when the engine is destroyed, any calls to the voices AFTER this is an error, so any voice->DestroyVoice() should always be called before the engine is destroyed.
void Soundscape1::CleanupGame ()
{
	if (mAmbience != NULL) {
		delete mAmbience;
		mAmbience = NULL;
	}

	if (mSubMove != NULL) {
		delete mSubMove;
		mSubMove = NULL;
	}

	if (mSubHalt != NULL) {
		delete mSubHalt;
		mSubHalt = NULL;
	}

	if (alarmForward != NULL) {
		delete alarmForward;
		alarmForward = NULL;
	}
	
	if (alarmLeft != NULL) {
		delete alarmLeft;
		alarmLeft = NULL;
	}	
	
	if (alarmRight != NULL) {
		delete alarmRight;
		alarmRight = NULL;
	}	

	if (mSound != NULL) {
		delete mSound;
		mSound = NULL;
	}
	
	list<AudioRenderable*>::const_iterator iter;
	for (iter=mRenderedSounds.begin(); iter!=mRenderedSounds.end(); ++iter)
	{
		delete *iter;
	}
	mRenderedSounds.clear();
if (mXACore != NULL) {
		delete mXACore;
		mXACore = NULL;
	}
} // end CleanupGame function.

XASound* Soundscape1::LoadXASound ()
{
	// First load the wave file and set up the format and buffer.
	WAVEFORMATEX wfmt;
	XAUDIO2_BUFFER xabuffer;
	PCMWave *wave = WaveFileManager::GetInstance().LoadWave ("Sounds/target_beacon.wav");
	// Exit with NULL if file was not loaded correctly.
	if (wave->GetStatus() != PCMWave::OK) { return NULL; }
	memset ((void*)&wfmt, 0, sizeof (WAVEFORMATEX));
	memcpy_s ((void*)&wfmt, sizeof (WaveFmt), (void*)&(wave->GetWaveFormat()), sizeof (WaveFmt));
	memset ((void*)&xabuffer, 0, sizeof (XAUDIO2_BUFFER));
	xabuffer.AudioBytes = wave->GetDataSize ();
	xabuffer.pAudioData = (BYTE*)(wave->GetWaveData ());

	// Create the source voice.
	IXAudio2SourceVoice *source;
	HRESULT hr = mXACore->GetEngine()->CreateSourceVoice (&source, &wfmt, XAUDIO2_VOICE_USEFILTER);
	// exit with NULL if cannot create source voice.
	if (FAILED(hr)) { return NULL; }

	// return the XASound object.
	return (new XASound (source, xabuffer));
} // end LoadXASound function.

//=== Helper functions for coding steps of X3DAudio.

void Soundscape1::InitX3DAudio ()
{
	DWORD channelMask;
	XAUDIO2_DEVICE_DETAILS voiceDetails = mXACore->GetDeviceDetails();
	channelMask = voiceDetails.OutputFormat.dwChannelMask; 

	X3DAudioInitialize (channelMask, X3DAUDIO_SPEED_OF_SOUND, mX3DInstance);
} // end InitX3DAudio function.

// set up the listener at the origin.
void Soundscape1::InitListener ()
{
	X3DAUDIO_VECTOR front = { 0.0f, 0.0f, 1.0f }; 
	X3DAUDIO_VECTOR top = { 0.0f, 1.0f, 0.0f }; 
	mListener.OrientFront = front;
	mListener.OrientTop = top;
	X3DAUDIO_VECTOR pos = { 3.0f, 0.0f, 1.0f }; 
	mListener.Position = pos;
} // end InitListener function.

void Soundscape1::InitEmitter()
{
	XAUDIO2_VOICE_DETAILS details;
	mSound->GetSourceVoice()->GetVoiceDetails(&details); 
	mEmitter.ChannelCount = details.InputChannels; 
	mEmitter.CurveDistanceScaler = 1.0f;
	X3DAUDIO_VECTOR pos = { 3.5f, 0.0f, 10.0f }; 
	mEmitter.Position = pos;
	X3DAUDIO_VECTOR vel = { 0.0f, 0.0f, 0.0f }; 
	mEmitter.Velocity =vel;
} // end InitEmitter function.

void Soundscape1::InitDSP()
{
	mDSPSettings.SrcChannelCount = mEmitter.ChannelCount; 
	mDSPSettings.DstChannelCount = mXACore->GetChannelCount();
	mDSPSettings.pMatrixCoefficients = new FLOAT32 [mDSPSettings.SrcChannelCount * mDSPSettings.DstChannelCount]; 
} // end InitDSP function.

void Soundscape1::Apply3D()
{
	X3DAudioCalculate (mX3DInstance, &mListener, &mEmitter, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT, &mDSPSettings );

	IXAudio2SourceVoice *source = mSound->GetSourceVoice();
	source->SetOutputMatrix ( NULL, mDSPSettings.SrcChannelCount, mDSPSettings.DstChannelCount, mDSPSettings.pMatrixCoefficients); 
	source->SetFrequencyRatio (mDSPSettings.DopplerFactor); 
	XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 
		2.0f * sinf(X3DAUDIO_PI/6.0f * mDSPSettings.LPFDirectCoefficient), 
		1.0f }; 
	source->SetFilterParameters (&FilterParameters);
}

void Soundscape1::MainLogic()
{
	//Warning Alarm For Close Mines 
	//Check Logic For Mines
	GPlay.GamePlayLogic();
	
	//change the source voice settings if the position has changed.
	Apply3D();

	//If Function Returns Sound Should Be Played - Play Sound
	if(GPlay.PlayForward() == 1){alarmForward->Play(0);}
	if(GPlay.PlayLeft()    == 1){alarmLeft->Play(0);}
	if(GPlay.PlayRight()   == 1){alarmRight->Play(0);}

	//Play Death Noise & Reset Player To Beginning
	if(GPlay.MineHit() == 1){playerDeath->Play(0); GPlay.ResetPlayer();}

	//Play Congratulations Noise If Game Won
	if(GPlay.GameWin() == 1){gameWin->Play(0);GPlay.ResetPlayer();}

	//Move Listener Along With The Ship
	if(GPlay.MovedUp    == true){mListener.Position.z++; GPlay.ResetMovements();}
	if(GPlay.MovedLeft  == true){mListener.Position.x--; GPlay.ResetMovements();}
	if(GPlay.MovedRight == true){mListener.Position.x++; GPlay.ResetMovements();}
	if(GPlay.MovedDown  == true){mListener.Position.z--; GPlay.ResetMovements();}

}