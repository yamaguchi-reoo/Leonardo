#include "RankingScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"
#include "../../Utility/ResourceManager.h"
#include "../RankingManager.h"

RankingScene::RankingScene()
{
}

RankingScene::~RankingScene()
{
}

void RankingScene::Initialize()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	rm->LoadFont("Resource/Font/TepidTerminal.ttf", "Tepid Terminal");

	RankingManager::GetInstance()->Load(); // �����L���O�f�[�^��ǂݍ���
}

eSceneType RankingScene::Update()
{
	InputControl* input = InputControl::GetInstance();
	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		return eSceneType::TITLE; // A�{�^���Ń^�C�g���֖߂�
	}
	return __super::Update();
}

void RankingScene::Draw() const
{
	const auto& rankings = RankingManager::GetInstance()->GetRankings();

	SetFontSize(32);
	DrawString(400, 100, "RANKING", GetColor(255, 255, 255));

	for (int i = 0; i < rankings.size(); ++i) {
		DrawFormatString(300, 150 + i * 40, GetColor(255, 255, 255),
			"%2d��: %-8s - %d", i + 1, rankings[i].name.c_str(), rankings[i].score);
	}
}

void RankingScene::Finalize()
{
}

eSceneType RankingScene::GetNowSceneType() const
{
	return eSceneType::RESULT;
}
