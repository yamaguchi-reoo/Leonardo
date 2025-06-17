#pragma once
#include "../SceneBase.h"
#include <string>

class RankingScene :
	public SceneBase
{
private:
	std::vector<int> sounds_data;
	int decision_se;
public:
	RankingScene();
	~RankingScene();

	void Initialize() override;		//‰Šú‰»ˆ—
	eSceneType Update() override;	//XVˆ—
	void Draw() override;		//•`‰æˆ—
	void Finalize() override;		//I—¹ˆ—

	eSceneType GetNowSceneType()const override;

	void PlaySoundSe(int _handle, int volume);
};

