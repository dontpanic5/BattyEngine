#include "AudioMgr.h"
#include "raylib.h"

Noise::Noise(Sound* sound, double* lengths, int numSounds, float audibleRange, int id, int entityId)
	: m_numSounds(numSounds), m_audibleRange(audibleRange), m_id(id), m_entityId(entityId)
{
	for (int i = 0; i < numSounds; i++)
	{
		m_sound[i] = sound[i];
		for (int j = 0; j < MAX_ALIAS_SOUNDS; j++)
		{
			m_aliasSounds[i][j] = LoadSoundAlias(m_sound[i]);
		}
		m_length[i] = lengths[i];
		m_curIdx[i] = 0;
	}
}

Sound& Noise::PlayNoise(double& length, float pitch)
{
	int random = rand() % m_numSounds;

	Sound& pickedSound = m_aliasSounds[random][m_curIdx[random]];
	m_curIdx[random]++;

	SetSoundPitch(pickedSound, pitch);

	PlaySound(pickedSound);
	length = m_length[random];
	return pickedSound;
}

AudioMgr::AudioMgr()
{
}

AudioMgr& AudioMgr::Instance()
{
	static AudioMgr instance;
	return instance;
}

void AudioMgr::AddNoise(Noise noise)
{
	m_noises.insert({noise.m_id, noise});
}

bool AudioMgr::PlayNoise(int id, double& length, float pitch)
{
	return m_noises.at(id).PlayNoise(length, pitch);
}

void AudioMgr::Unload()
{
	for (const auto pair : m_noises)
	{
		for (int i = 0; i < pair.second.m_numSounds; i++)
		{
			UnloadSound(pair.second.m_sound[i]);
		}
	}
}

const Noise& AudioMgr::GetNoise(int id)
{
	return m_noises.at(id);
}
