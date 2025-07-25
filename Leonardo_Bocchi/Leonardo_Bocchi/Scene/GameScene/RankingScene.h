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

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() override;		//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;

	void PlaySoundSe(int _handle, int volume);
};

