#pragma once
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_dsp_effects.h"
#include "fmod_common.h"
#include "fmod_codec.h"
#include "fmod_output.h"
#include "fmod_errors.h"

#define _7_1_AUDIO
//#define _5_1_AUDIO

const int DISTANCE = 1.0f;

enum Soundtrack
{
	LEVEL1,
	LEVEL2,
	LEVEL3
};

struct Vec3
{
	float x;
	float y;
	float z;

};

/// <summary>
/// Struct for FMOD implementation
/// </summary>
struct FmodInit
{
	FmodInit();
	~FmodInit();

	//Functions
	void Update();

	//Maps
	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	typedef std::map<int, FMOD::Channel*> ChannelMap;


	//Variables
	FMOD::System* pSystem;
	FMOD::Reverb3D* pReverb;
	FMOD::ChannelGroup* pSoundtracks;
	FMOD::ChannelGroup* pEffects;
	FMOD::ChannelGroup* pMasterGroup;
	FMOD::ChannelGroup* pAttacks;
	FMOD::DSP* pDspFader;
	FMOD::DSP* pDspPan;

	int   m_NextChannelID;

	float m_SoundVolume;
	float m_SoundtrackVolume;
	float m_Pitch;
	float m_MasterVolume;
	float m_MinDistance;
	float m_MaxDistance;
	
	bool  m_StopRequested;
	bool  m_IsPlaying;

	unsigned int m_Version;

	FMOD_VECTOR m_ListenerPos;
	FMOD_VECTOR m_ReverbPos;
	FMOD_VECTOR m_Pos;
	FMOD_VECTOR m_Vel;

	SoundMap m_SoundMap;
	ChannelMap m_ChannelMap;

};

class QuackAudio
{
public:

	void  Init();
	void  Update();
	void  Shutdown();

	//functions using map

	/// <summary>
	/// Creates the sound
	/// </summary>
	/// <param name="pathToSound"></param>
	/// <param name="is3D"></param>
	/// <param name="isLoop"></param>
	/// <param name="isStream"></param>
	void CreateSound(const std::string& pathToSound,
		bool is3D = true, bool isLoop = false, bool isStream = false);

	/// <summary>
	/// Erase sounds
	/// </summary>
	/// <param name="pathToSound"></param>
	void EraseSound(const std::string& pathToSound);

	/// <summary>
	/// Plays and create the sound if not created
	/// </summary>
	/// <param name="pathToSound"></param>
	/// <param name="pos"></param>
	/// <param name="volume"></param>
	/// <returns></returns>
	int  PlaySound(const std::string& pathToSound,
		const Vec3& pos, float volume = 0.0f);
	/// <summary>
	/// Overload play so channel groups can be passed
	/// </summary>
	/// <param name="pathToSound"></param>
	/// <param name="channelGroup"></param>
	/// <param name="pos"></param>
	/// <param name="volume"></param>
	/// <returns></returns>
	int  PlaySound(const std::string& pathToSound, FMOD::ChannelGroup* channelGroup,
		const Vec3& pos, float volume = 0.0f);

	//Setter functions
	void  SetVolume(int channelID, float volumeDB);
	void  SetMasterChannelVolume(float volumeDB);
	void  SetPitch(int channelID, float pitch);
	void  SetMasterChannelPitch(float pitch);
	void  SetChannelGroup(int channelID, FMOD::ChannelGroup* channelGroup);
	void  Set3DMinMax(const std::string& pathToSound, float min, float max);
	void  Set3DAttributes(int channelID, const FMOD_VECTOR* pos, const FMOD_VECTOR* vel);
	
	void  AddDsp(int channelID, int index, FMOD::DSP* dsp);

	//Mute channels
	void  MuteChannel(int channelID, bool mute);
	void  MuteMasterChannel(bool mute);

	void  Pause(int channelID, bool pause);
	void  FadeIn(int channelID, float fadeTime);
	/// <summary>
	/// Fade sounds at the end of the song
	/// </summary>
	/// <param name="pathToSound"></param>
	/// <param name="channelID"></param>
	/// <param name="fadeTime"></param>
	void  FadeOut(const std::string& pathToSound, int channelID, float fadeTime);

	//Channel helper functions
	void  CreateChannelGroup(const char* groupName, FMOD::ChannelGroup** channelGroup);
	void  StopChannelGroup(FMOD::ChannelGroup* channelGroup);
	void  MasterChannelManager();
	void  SoundtrackManager();

	void  StopChannel(int channelID, float fadeTime);

	/// <summary>
	/// Changes DB to Volume
	/// </summary>
	/// <param name="DB"></param>
	/// <returns></returns>
	float ChangingDBToVolume(float DB);

	bool  IsPlaying(int channeID);

	/// <summary>
	/// Converting local vector to FMOD_VECTOR
	/// </summary>
	/// <param name="soundPo"></param>
	/// <returns></returns>
	FMOD_VECTOR VectorToFmodVec(const Vec3& soundPo);

	Soundtrack m_Soundtracks;

	//Functions to access the group
	static FMOD::ChannelGroup* Effects();
	static FMOD::ChannelGroup* Soundtrack();
	static FMOD::ChannelGroup* Master();
	static FMOD::ChannelGroup* Attacks();


};

