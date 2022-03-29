#pragma once

#include "Include.h"

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)
public:
	enum ChannelID { Channel_Player, Channel_Unit0, Channel_Unit1, Channel_Unit2, 
					 Channel_Unit3, Channel_Unit4, Channel_Unit5, Channel_Unit6,
					 Channel_Unit7, Channel_Unit8, Channel_Unit9, Channel_Unit10, Channel_Mon, Channel_Eff, Channel_BGM,  Channel_End };

public:
	CSoundMgr(void);
	~CSoundMgr(void);

public:
	void Initialize();
	void LoadSoundFile();
	void PlaySound(TCHAR* pSoundKey, ChannelID eID);
	void PlayBGM(TCHAR* pSoundKey, ChannelID eID);
	void UpdateSound();
	void StopSound(ChannelID eID);
	void StopAll();
	void Release();

private:
	FMOD_SYSTEM*	m_pSystem;
	FMOD_CHANNEL*	m_pChannelArr[Channel_End]; // 채널은 중첩되지 않게 준비.

	map<const TCHAR*, FMOD_SOUND*>	m_MapSound;
};
