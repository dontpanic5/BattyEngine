#ifndef AUDIO_MGR_H_INCLUDED
#define AUDIO_MGR_H_INCLUDED

#include <unordered_map>
#include "raylib.h"

constexpr int MAX_SOUNDS = 8;

struct Noise
{
	Noise(Sound* sound, double* lengths, int numSounds, float audibleRange, int id, bool pitchShift = false);

	bool PlayNoise(double& length, bool overwrite);

	Sound m_sound[MAX_SOUNDS] = { 0 };
	double m_length[MAX_SOUNDS] = { 0 };
	int m_numSounds;
	float m_audibleRange;
	int m_id;
	bool m_pitchShift;
};

class AudioMgr final
{
public:
	static AudioMgr& Instance();

	void AddNoise(Noise noise);
	bool PlayNoise(int id, double& length, bool overwrite = true);
	void Unload();

	std::unordered_map<int, Noise> m_noises;

private:
	AudioMgr();
	AudioMgr(AudioMgr const&) = delete;
	AudioMgr& operator=(AudioMgr const&) = delete;
};

#endif // !AUDIO_MGR_H_INCLUDED
