#include "pch.h"
#include "QuackAudio.h"



FmodInit::FmodInit()
{
	FMOD_RESULT result;
	pSystem = nullptr;
	pDspFader = nullptr;
	pDspPan = nullptr;
	pMasterGroup = nullptr;
	pReverb = nullptr;

	m_IsPlaying = false;

	m_SoundVolume = 1.0f;
	m_SoundtrackVolume = 2.0f;
	m_NextChannelID = 0;
	m_Pitch = 1.0f;
	m_MasterVolume = 1.0f;
	m_MinDistance = 10.0f;
	m_MaxDistance = 20.0f;
	m_SoundtrackVolume = 0.1f;	
	m_ListenerPos = { 0.0f, 0.0f, -1.0f * DISTANCE };
	m_ReverbPos = { -10.0f, 0.0f, 0.0f };
	m_Pos = { 50.0f * DISTANCE, 0.0f, 0.0f };
	m_Vel = { 0.0f, 0.0f, 0.0f };



#pragma region Fmod setup
	result = FMOD::System_Create(&pSystem);
	if (result != FMOD_OK)
	{
		printf("Error creating fmod system:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = pSystem->getVersion(&m_Version);
	if (m_Version < FMOD_VERSION)
	{
		printf("Wrong FMOD Version: (%d) %s ", result, FMOD_ErrorString(result));
	}

#ifdef _5_1_AUDIO

	result = pSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
	if (result != FMOD_OK)
	{
		printf("Error setting 5.1 spatial audio:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = pSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
	if (result != FMOD_OK)
	{
		printf("Error setting output:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}


#endif // _5_1_AUDIO

#ifdef  _7_1_AUDIO

	result = pSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_7POINT1POINT4, 0);
	if (result != FMOD_OK)
	{
		printf("Error setting 7.1 spatial audio:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}


	result = pSystem->setOutput(FMOD_OUTPUTTYPE_WINSONIC);
	if (result != FMOD_OK)
	{
		printf("Error setting output:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}

#endif //  _7_1_AUDIO


	result = pSystem->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		printf("Error initializing fmod:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}



	//Setting up 3D sound
	result = pSystem->set3DSettings(1.0, DISTANCE, 1.0f);
	if (result != FMOD_OK)
	{
		printf("Error setting 3D settings:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = pSystem->set3DListenerAttributes(0, &m_ListenerPos, 0, 0, 0);
	if (result != FMOD_OK)
	{
		printf("Error setting 3D listener attributes:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}


	//Setting up Reverb
	result = pSystem->createReverb3D(&pReverb);
	if (result != FMOD_OK)
	{
		printf("Error creating Reverb:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = pReverb->set3DAttributes(&m_ReverbPos, m_MinDistance, m_MaxDistance);
	if (result != FMOD_OK)
	{
		printf("Error setting reverb 3D attributes:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}

	//Setting up Dsp
	result = pSystem->createDSPByType(FMOD_DSP_TYPE_FADER, &pDspFader);
	if (result != FMOD_OK)
	{
		printf("Error setting DSP Fader settings:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = pSystem->createDSPByType(FMOD_DSP_TYPE_PAN, &pDspPan);
	if (result != FMOD_OK)
	{
		printf("Error setting DSP pan settings:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}
#pragma endregion Fmod Setup

}


FmodInit::~FmodInit()
{

	pSystem->release();
}

void FmodInit::Update()
{
	std::vector<ChannelMap::iterator> channelsStopped;

	for (auto it = m_ChannelMap.begin(), itEnd = m_ChannelMap.end(); it != itEnd; ++it)
	{
		bool isPlaying = false;
		it->second->isPlaying(&isPlaying);
		if (!isPlaying) channelsStopped.push_back(it);
	}
	for (auto& it : channelsStopped)
	{
		m_ChannelMap.erase(it);
	}
	pSystem->update();
}
FmodInit* pFmod;

void QuackAudio::Init()
{
	 pFmod = new FmodInit;

	
	CreateChannelGroup("Soundtrack", &pFmod->pSoundtracks);
	CreateChannelGroup("Effects", &pFmod->pEffects);
	CreateChannelGroup("Attacks", &pFmod->pAttacks);
	MasterChannelManager();
	
	//Soundtrack
	CreateSound("res/Sounds/CityTheme.wav", false, true, true);
	CreateSound("res/Sounds/TorchAmbience.wav", false, true, true);


	SoundtrackManager();
	
	//Swish
	CreateSound("res/Sounds/LightAttack.wav");
	
	//Heavy Swish
	CreateSound("res/Sounds/HeavyAttack.wav");
	
	//Death
	CreateSound("res/Sounds/JessicaDeathScream.wav");
	
	//Enemy Hit
	CreateSound("res/Sounds/EnemyHit.wav");
	
	//Jessica getting hit
	CreateSound("res/Sounds/JessicaHit.wav");
	
	//Jessica voice Light
	CreateSound("res/Sounds/JessicaLight.wav");
	
	//Jessica voice Heavy
	CreateSound("res/Sounds/JessicaHeavy.wav");

	CreateSound("res/Sounds/JessicaHit.wav");
	CreateSound("res/Sounds/JessicaDeathScream.wav");
	
	
	

	
	//Volume control
	pFmod->pSoundtracks->setVolume(0.1f);
	pFmod->pEffects->setVolume(0.3f);
	pFmod->pAttacks->setVolume(0.3f);
	pFmod->pMasterGroup->setVolume(0.5f);

}

void QuackAudio::Update()
{
	pFmod->Update();
}

void QuackAudio::Shutdown()
{
	delete pFmod;
}

void QuackAudio::CreateSound(const std::string& pathToSound,
	bool is3D, bool isLoop, bool isStream)
{
	auto found = pFmod->m_SoundMap.find(pathToSound);

	//avoiding double loading
	if (found != pFmod->m_SoundMap.end()) return;

	//Creating mode
	FMOD_MODE dMode = FMOD_DEFAULT;
	dMode |= is3D ? FMOD_3D : FMOD_2D;
	dMode |= isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	dMode |= isStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	//Creating the sound and pushing it to the map
	FMOD::Sound* sound = nullptr;
	pFmod->pSystem->createSound(pathToSound.c_str(),
		dMode, nullptr, &sound);

	if (sound) pFmod->m_SoundMap[pathToSound] = sound;

}

void QuackAudio::SoundtrackManager()
{
	PlaySound("res/Sounds/CityTheme.wav", pFmod->pSoundtracks, Vec3{ 0,0,0 }, 0.3f);
	PlaySound("res/Sounds/TorchAmbience.wav", pFmod->pSoundtracks, Vec3{ 0,0,0 }, 0.1f);

}

void QuackAudio::EraseSound(const std::string& pathToSound)
{
	//Avoiding double loading
	auto found = pFmod->m_SoundMap.find(pathToSound);
	if (found == pFmod->m_SoundMap.end()) return;

	//Unloading the sound 
	found->second->release();
	pFmod->m_SoundMap.erase(found);
}

int QuackAudio::PlaySound(const std::string& pathToSound,
	const Vec3& pos, float volume)
{
	//Setting the channel ID for the map
	int currentChannelID = pFmod->m_NextChannelID++;

	//Avoinding double loading
	auto found = pFmod->m_SoundMap.find(pathToSound);
	if (found == pFmod->m_SoundMap.end())
	{
		CreateSound(pathToSound);
		found = pFmod->m_SoundMap.find(pathToSound);
		if (found == pFmod->m_SoundMap.end()) return currentChannelID;

	}


		//Creating channel and passing it to the map
		FMOD::Channel* channel = nullptr;
		pFmod->pSystem->playSound(found->second,
			nullptr, true, &channel);

		if (channel)
		{
			FMOD_VECTOR position = VectorToFmodVec(pos);
			channel->set3DAttributes(&position, nullptr);
			channel->setVolume(ChangingDBToVolume(volume));
			channel->setPaused(false);
			pFmod->m_ChannelMap[currentChannelID] = channel;
		}
	

	return currentChannelID;
}

int QuackAudio::PlaySound(const std::string& pathToSound, FMOD::ChannelGroup* channelGroup, const Vec3& pos, float volume)
{
	//Setting the channel ID for the map
	int currentChannelID = pFmod->m_NextChannelID++;

	//Avoinding double loading
	auto found = pFmod->m_SoundMap.find(pathToSound);
	if (found == pFmod->m_SoundMap.end())
	{
		CreateSound(pathToSound);
		found = pFmod->m_SoundMap.find(pathToSound);
		if (found == pFmod->m_SoundMap.end()) return currentChannelID;

	}



	//Creating channel and passing it to the map
	FMOD::Channel* channel = nullptr;
	pFmod->pSystem->playSound(found->second,
		channelGroup, true, &channel);

	if (channel)
	{
		FMOD_VECTOR position = VectorToFmodVec(pos);
		channel->set3DAttributes(&position, nullptr);
		channel->setVolume(ChangingDBToVolume(volume));
		channel->setPaused(false);
		pFmod->m_ChannelMap[currentChannelID] = channel;
	}


	return currentChannelID;
}



void QuackAudio::CreateChannelGroup(const char* groupName, FMOD::ChannelGroup** channelGroup)
{
	FMOD_RESULT result;

	result = pFmod->pSystem->createChannelGroup(groupName, channelGroup);

	if (result != FMOD_OK)
	{
		printf("Error creating Channel group:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);

	}
}

void QuackAudio::StopChannelGroup(FMOD::ChannelGroup* channelGroup)
{
	channelGroup->stop();
}

void QuackAudio::SetVolume(int channelID, float volume)
{
	auto found = pFmod->m_ChannelMap.find(channelID);

	//Avoiding double loading
	if (found == pFmod->m_ChannelMap.end()) return;

	found->second->setVolume(ChangingDBToVolume(volume));
}

void QuackAudio::MasterChannelManager()
{
	FMOD_RESULT result;

	result = pFmod->pSystem->getMasterChannelGroup(&pFmod->pMasterGroup);
	if (result != FMOD_OK)
	{
		printf("Error getting master Channel group:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);

	}

	//Setting other groups to be child of master group
	result = pFmod->pMasterGroup->addGroup(pFmod->pSoundtracks);
	if (result != FMOD_OK)
	{
		printf("Error adding channel to master group:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = pFmod->pMasterGroup->addGroup(pFmod->pEffects);
	if (result != FMOD_OK)
	{
		printf("Error adding channel to master group: (%d) %s", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = pFmod->pMasterGroup->addGroup(pFmod->pAttacks);
	if (result != FMOD_OK)
	{
		printf("Error adding channel to master group: (%d) %s", result, FMOD_ErrorString(result));
		exit(-1);
	}
}


void QuackAudio::SetMasterChannelVolume(float volumeDB)
{
	FMOD_RESULT result;
	result = pFmod->pMasterGroup->setVolume(ChangingDBToVolume(volumeDB));
	if (result != FMOD_OK)
	{
		printf("Error setting up master channel volume:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}
}


void QuackAudio::SetPitch(int channelID, float pitch)
{
	FMOD_RESULT result;
	auto found = pFmod->m_ChannelMap.find(channelID);

	//avoiding double loading
	if (found == pFmod->m_ChannelMap.end()) return;


	result = found->second->setPitch(pitch);
	if (result != FMOD_OK)
	{
		printf("Error setting up pitch:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void QuackAudio::SetMasterChannelPitch(float pitch)
{
	FMOD_RESULT result;
	result = pFmod->pMasterGroup->setPitch(pitch);
	if (result != FMOD_OK)
	{
		printf("Error setting up master channel volume:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void QuackAudio::MuteChannel(int channelID, bool mute)
{
	FMOD_RESULT result;

	auto found = pFmod->m_ChannelMap.find(channelID);

	//avoiding double loading
	if (found == pFmod->m_ChannelMap.end()) return;

	if (mute == true)
	{
		bool mut = true;
		result = found->second->getMute(&mut);
		if (result != FMOD_OK)
		{
			printf("Error getting mute:  (%d) %s ", result, FMOD_ErrorString(result));
			exit(-1);
		}

		result = found->second->setMute(!mut);
		if (result != FMOD_OK)
		{
			printf("Error setting up mute :  (%d) %s ", result, FMOD_ErrorString(result));
			exit(-1);
		}

	}
}

void QuackAudio::MuteMasterChannel(bool mute)
{
	FMOD_RESULT result;

	if (mute == true)
	{
		bool mut = true;

		result = pFmod->pMasterGroup->getMute(&mut);
		if (result != FMOD_OK)
		{
			printf("Error getting mute:  (%d) %s ", result, FMOD_ErrorString(result));
			exit(-1);
		}
		result = pFmod->pMasterGroup->setMute(!mut);
		if (result != FMOD_OK)
		{
			printf("Error setting up mute :  (%d) %s ", result, FMOD_ErrorString(result));
			exit(-1);
		}
	}
}

void  QuackAudio::FadeIn(int channelID, float fadeTime)
{
	auto found = pFmod->m_ChannelMap.find(channelID);

	//avoiding double loading 
	if (found == pFmod->m_ChannelMap.end()) return;

	int rate = 0;
	unsigned long long dspClock;

	found->second->getSystemObject(&pFmod->pSystem);
	pFmod->pSystem->getSoftwareFormat(&rate, 0, 0);
	found->second->getDSPClock(0, &dspClock);
	found->second->addFadePoint(dspClock, 0.0f);
	found->second->addFadePoint(dspClock + (rate * fadeTime), 1.0f);

}

void QuackAudio::FadeOut(const std::string& pathToSound, int channelID, float fadeTime)
{
	FMOD_RESULT result;

	auto found = pFmod->m_ChannelMap.find(channelID);
	auto foundSound = pFmod->m_SoundMap.find(pathToSound);

	//Avoiding double loading
	if (found == pFmod->m_ChannelMap.end()) return;

	if (foundSound == pFmod->m_SoundMap.end()) return;

	int rate = 0;

	//Getting the sound lenght
	unsigned int lenght;
	foundSound->second->getLength(&lenght, FMOD_TIMEUNIT_MS);
	
	//Getting the sound format
	FMOD_SOUND_TYPE soundType;
	FMOD_SOUND_FORMAT soundFormat;
	int soundChannels;
	int soundBits;
	foundSound->second->getFormat(&soundType, &soundFormat, &soundChannels, &soundBits);

	//Getting the reference clock
	unsigned long long dspClock;
	unsigned long long parentClock;
	found->second->getDSPClock(0, &parentClock);

	//Getting software format
	FMOD_SPEAKERMODE speakerMode;
	int  sampleRate;
	int  rawSpeakers;
	pFmod->pSystem->getSoftwareFormat(&sampleRate, &speakerMode, &rawSpeakers);

	/*if (sampleRate > 0 && soundBits > 0)
	{
		found->second->addFadePoint(parentClock, 0.0f);
		found->second->addFadePoint(parentClock + (long)(sampleRate * fadeTime), 1.0f);
	}*/

	auto convertedLength = static_cast<int>(round(lenght * sampleRate / soundBits));

	auto endOfAudio = (lenght * sampleRate) / soundBits;

	int test = 100000;
	int sec = sampleRate * (lenght / 1000); // sample rate work by secods, so first I need to find how much is 1 secondon the lenght of the audio, 
	found->second->addFadePoint(parentClock + (sec)-(sampleRate * fadeTime), 1.0f); // adding a fade point to the close to the end of the audio
	result = found->second->addFadePoint(parentClock + (sec), 0.0f);//Tells the fade point to set volume to 0

}

void QuackAudio::Set3DMinMax(const std::string& pathToSound, float min, float max)
{
	FMOD_RESULT result;
	auto found = pFmod->m_SoundMap.find(pathToSound);

	if (found == pFmod->m_SoundMap.end()) return;

	result = found->second->set3DMinMaxDistance(min, max);
	if (result != FMOD_OK)
	{
		printf("Error setting 3D min max:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void QuackAudio::Set3DAttributes(int channelID, const FMOD_VECTOR* pos, const FMOD_VECTOR* vel)
{
	FMOD_RESULT result;

	auto found = pFmod->m_ChannelMap.find(channelID);

	//avoiding double loading 
	if (found == pFmod->m_ChannelMap.end()) return;

	result = found->second->set3DAttributes(&pFmod->m_Pos, &pFmod->m_Vel);
	if (result != FMOD_OK)
	{
		printf("Error setting 3D attributes:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void QuackAudio::AddDsp(int channelID, int index, FMOD::DSP* dsp)
{
	FMOD_RESULT result;

	auto found = pFmod->m_ChannelMap.find(channelID);

	//avoiding double loading 
	if (found == pFmod->m_ChannelMap.end()) return;

	result = found->second->addDSP(index, dsp);
	if (result != FMOD_OK)
	{
		printf("Error adding dsp:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void QuackAudio::SetChannelGroup(int channelID, FMOD::ChannelGroup* channelGroup)
{
	FMOD_RESULT result;

	auto found = pFmod->m_ChannelMap.find(channelID);

	//avoiding double loading 
	if (found == pFmod->m_ChannelMap.end()) return;

	result = found->second->setChannelGroup(channelGroup);
	if (result != FMOD_OK)
	{
		printf("Error setting channel group:  (%d) %s ", result, FMOD_ErrorString(result));
		exit(-1);
	}

}


void QuackAudio::Pause(int channelID, bool pause)
{
	FMOD_RESULT result;

	auto found = pFmod->m_ChannelMap.find(channelID);

	//avoiding double loading 
	if (found == pFmod->m_ChannelMap.end()) return;


	if (pause == false)
	{
		result = found->second->setPaused(false);
		if (result != FMOD_OK)
		{
			printf("Error unpausing:  (%d) %s ", result, FMOD_ErrorString(result));
			exit(-1);
		}
	}
	if (pause == true)
	{
		result = found->second->setPaused(true);
		if (result != FMOD_OK)
		{
			printf("Error pausing:  (%d) %s ", result, FMOD_ErrorString(result));
			exit(-1);
		}
	}
}

bool QuackAudio::IsPlaying(int channelID)
{
	auto found = pFmod->m_ChannelMap.find(channelID);

	//avoiding double loading 
	//if (found == pFmod->m_ChannelMap.end()) return;


	found->second->isPlaying(&pFmod->m_IsPlaying);
	
	if (pFmod->m_IsPlaying)
	{
		return true;
	}
	else
	{
		return false;
	}
}

FMOD_VECTOR  QuackAudio::VectorToFmodVec(const Vec3& soundPo)
{
	FMOD_VECTOR vecFmod;
	vecFmod.x = soundPo.x;
	vecFmod.y = soundPo.y;
	vecFmod.z = soundPo.z;
	return vecFmod;
}

FMOD::ChannelGroup* QuackAudio::Effects()
{
	return pFmod->pEffects;
}

FMOD::ChannelGroup* QuackAudio::Soundtrack()
{
	return pFmod->pSoundtracks;
}

FMOD::ChannelGroup* QuackAudio::Master()
{
	return pFmod->pMasterGroup;
}

FMOD::ChannelGroup* QuackAudio::Attacks()
{
	return pFmod->pAttacks;
}

float QuackAudio::ChangingDBToVolume(float DB)
{
	return powf(10.0/*X*/, 0.05f * DB/*Y*/);
}


