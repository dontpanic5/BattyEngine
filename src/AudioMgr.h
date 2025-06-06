#ifndef AUDIO_MGR_H_INCLUDED
#define AUDIO_MGR_H_INCLUDED

#include <unordered_map>
#include "raylib.h"

constexpr int MAX_SOUNDS		= 4;
constexpr int MAX_ALIAS_SOUNDS	= 4;

struct Noise
{
	Noise(Sound* sound, float* volumes, int numSounds, float audibleRange, int id);

	Sound& PlayNoise(float pitch = 1.0f, float volume = 1.0f);

	Sound m_sound[MAX_SOUNDS] = { 0 };
	float m_volumes[MAX_SOUNDS];
	Sound m_aliasSounds[MAX_SOUNDS][MAX_ALIAS_SOUNDS] = { 0 };
	int m_curIdx[MAX_SOUNDS];
	int m_numSounds;
	float m_audibleRange;
	int m_id;
	bool m_speakingNoise;
};

class AudioMgr final
{
public:
	AudioMgr();

	static AudioMgr& Instance();

	void AddNoise(Noise noise);
	Sound& PlayNoise(int id, float pitch = 1.0f, float volume = 1.0f);
	const Noise& GetNoise(int id);
	void Unload();

	std::unordered_map<int, Noise> m_noises;

private:
	AudioMgr(AudioMgr const&) = delete;
	AudioMgr& operator=(AudioMgr const&) = delete;
};

#endif // !AUDIO_MGR_H_INCLUDED
