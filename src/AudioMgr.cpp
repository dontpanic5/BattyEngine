#include "AudioMgr.h"
#include "raylib.h"

Noise::Noise(Sound* sound, double* lengths, int numSounds, float audibleRange, int id, bool pitchShift)
	: m_numSounds(numSounds), m_audibleRange(audibleRange), m_id(id), m_pitchShift(pitchShift)
{
	for (int i = 0; i < numSounds; i++)
	{
		m_sound[i] = sound[i];
		m_length[i] = lengths[i];
	}
}

bool Noise::PlayNoise(double& length, bool overwrite)
{
	if (!overwrite)
	{
		for (int i = 0; i < m_numSounds; i++)
		{
			if (IsSoundPlaying(m_sound[i]))
				return false;
		}
	}

	int random = rand() % m_numSounds;
	PlaySound(m_sound[random]);
	length = m_length[random];
	return true;
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

bool AudioMgr::PlayNoise(int id, double& length, bool overwrite)
{
	return m_noises.at(id).PlayNoise(length, overwrite);
}

void AudioMgr::Unload()
{
	for (auto pair : m_noises)
	{
		for (int i = 0; i < pair.second.m_numSounds; i++)
		{
			UnloadSound(pair.second.m_sound[i]);
		}
	}
}
