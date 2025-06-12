#include "PlayerSoundManager.h"
#include "../../../Utility/ResourceManager.h"
#include <DxLib.h>

void PlayerSoundManager::LoadSounds()
{
	ResourceManager* rm = ResourceManager::GetInstance();

	sound_map[SoundType::TELEPORT] = rm->GetSound("Resource/Sounds/SE/Player/AS_1296266_サイバーな感じのワープ音_Audio Trimmer.mp3");
	sound_map[SoundType::WALK] = rm->GetSound("Resource/Sounds/SE/Player/AS_154568_コツッ（足音）.mp3");
}

void PlayerSoundManager::PlaySoundSE(SoundType sound_type, int volume, bool flg)
{
    ResourceManager* rm = ResourceManager::GetInstance();
    auto it = sound_map.find(sound_type);
    if (it != sound_map.end()) {
        const auto& handles = it->second;
        if (!handles.empty()) {
            ChangeVolumeSoundMem(volume, handles[0]);
            if (flg)
            {
                PlaySoundMem(handles[0], DX_PLAYTYPE_BACK);  // SEは1回のみ再生
            }
            else
            {
                PlaySoundMem(handles[0], DX_PLAYTYPE_LOOP);  // 常に先頭の音だけ再生
            }
            
        }
    }
}

void PlayerSoundManager::StopSound(SoundType sound_type)
{
    auto it = sound_map.find(sound_type);
    if (it != sound_map.end() && !it->second.empty())
    {
        StopSoundMem(it->second[0]); // 最初の音だけ停止
    }
}
