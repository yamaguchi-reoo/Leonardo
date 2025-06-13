#include "RankingScene.h"
#include <DxLib.h>

RankingScene::RankingScene()
{
}

RankingScene::~RankingScene()
{
}

void RankingScene::Initialize()
{
}

eSceneType RankingScene::Update()
{
	return __super::Update();
}

void RankingScene::Draw() const
{
	DrawString(100, 100, "Ranking Scene", GetColor(255, 255, 255));
}

void RankingScene::Finalize()
{
}

eSceneType RankingScene::GetNowSceneType() const
{
	return eSceneType::RESULT;
}
