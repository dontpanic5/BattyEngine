#include "AudioMgr.h"
#include "raylib.h"

Noise::Noise(Sound* sound, double* lengths, int numSounds, float audibleRange, int id)
	: m_numSounds(numSounds), m_audibleRange(audibleRange), m_id(id)
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
	if (m_curIdx[random] < MAX_ALIAS_SOUNDS - 1)
	{
		m_curIdx[random]++;
	}
	else
	{
		m_curIdx[random] = 0;
	}

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

Sound& AudioMgr::PlayNoise(int id, double& length, float pitch)
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
			for (int j = 0; j < MAX_ALIAS_SOUNDS; j++)
			{
				UnloadSoundAlias(pair.second.m_aliasSounds[i][j]);
			}
		}
	}
}

const Noise& AudioMgr::GetNoise(int id)
{
	return m_noises.at(id);
}
