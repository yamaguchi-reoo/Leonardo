#pragma once
#include "../SceneBase.h"
#include <vector>

class SelectScene :
	public SceneBase
{
private:
    int select_index = 0; // 0 = Tutorial, 1 = MainStage
    int font = -1;

    std::vector<int> sounds_data;
    int select_bgm = -1;
    int select_se = -1; 
    int decision_se = -1;

public:
    void Initialize() override;
    eSceneType Update() override;
    void Draw() override;
    void Finalize() override;
    eSceneType GetNowSceneType() const override;


    void LoadResource();
    void PlaySoundMusic(int _handle, int volume, int type);
};

