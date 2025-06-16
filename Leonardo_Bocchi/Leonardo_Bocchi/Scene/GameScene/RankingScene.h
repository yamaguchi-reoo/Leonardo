#pragma once
#include "../SceneBase.h"
#include <string>

class RankingScene :
	public SceneBase
{
public:
	RankingScene();
	~RankingScene();

	void Initialize() override;		//‰Šú‰»ˆ—
	eSceneType Update() override;	//XVˆ—
	void Draw() override;		//•`‰æˆ—
	void Finalize() override;		//I—¹ˆ—

	eSceneType GetNowSceneType()const override;
};

