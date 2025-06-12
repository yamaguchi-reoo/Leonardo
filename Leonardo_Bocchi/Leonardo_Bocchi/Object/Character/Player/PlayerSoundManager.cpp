#include "PlayerSoundManager.h"
#include "../../../Utility/ResourceManager.h"
#include <DxLib.h>

void PlayerSoundManager::LoadSounds()
{
	ResourceManager* rm = ResourceManager::GetInstance();

	sound_map[SoundType::TELEPORT] = rm->GetSound("Resource/Sounds/SE/Player/AS_1296266_�T�C�o�[�Ȋ����̃��[�v��_Audio Trimmer.mp3");
	sound_map[SoundType::WALK] = rm->GetSound("Resource/Sounds/SE/Player/AS_1512910_�Ղɂ�_�����Ȃǂ̉�������_�P��_4.mp3");
	sound_map[SoundType::JUMP] = rm->GetSound("Resource/Sounds/SE/Player/AS_14458_���g���Q�[���̌��ʉ��B�W�����v.mp3");
	sound_map[SoundType::LAND] = rm->GetSound("Resource/Sounds/SE/Player/AS_52495_�W�����v�̒��n�i�X�^�b�j.mp3");
	sound_map[SoundType::DAMAGE] = rm->GetSound("Resource/Sounds/SE/Player/AS_1458605_�h���b�Q�i�_���[�W���󂯂鉹�j.mp3");
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
                PlaySoundMem(handles[0], DX_PLAYTYPE_BACK);  // SE��1��̂ݍĐ�
            }
            else
            {
                PlaySoundMem(handles[0], DX_PLAYTYPE_LOOP);  // ��ɐ擪�̉������Đ�
            }
            
        }
    }
}

void PlayerSoundManager::StopSound(SoundType sound_type)
{
    auto it = sound_map.find(sound_type);
    if (it != sound_map.end() && !it->second.empty())
    {
        StopSoundMem(it->second[0]); // �ŏ��̉�������~
    }
}
