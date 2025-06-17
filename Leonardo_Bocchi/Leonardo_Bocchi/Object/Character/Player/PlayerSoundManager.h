#pragma once
#include <map>
#include <string>
#include <vector>


enum class SoundType
{
    WALK,
	JUMP,
    LAND,
    DAMAGE,
    HEAL,
    INVINCIBLE,
    GOAL,
    TELEPORT,
	TRAP,
};

class PlayerSoundManager
{
private:
    std::map<SoundType, std::vector<int>> sound_map;; // 音声データを管理するマップ

public:
    void LoadSounds();
    void PlaySoundSE(SoundType sound_type, int volume, bool flg);

    void StopSound(SoundType sound_type);
};

