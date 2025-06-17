#include "RankingScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"
#include "../../Utility/ResourceManager.h"
#include "../RankingManager.h"
#include "../../common.h"

RankingScene::RankingScene() :decision_se(-1)
{
}

RankingScene::~RankingScene()
{
}

void RankingScene::Initialize()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	rm->LoadFont("Resource/Font/TepidTerminal.ttf", "Tepid Terminal");

	RankingManager::GetInstance()->Load(); // ランキングデータを読み込む


	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1296213_サイバーな感じの決定音.mp3");
	decision_se = sounds_data[0];
}

eSceneType RankingScene::Update()
{
	InputControl* input = InputControl::GetInstance();
	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		PlaySoundSe(decision_se, 70); // 決定音を再生
		clear_count = 0;
		return eSceneType::TITLE;

	}

	return __super::Update();
}

void RankingScene::Draw()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	int handle = rm->GetFontHandle("Tepid Terminal", 64);
	const auto& rankings = RankingManager::GetInstance()->GetRankings();

	const int title_y = 50;

	// タイトル
	std::string title = "== RANKING ==";
	int title_width = GetDrawStringWidthToHandle(title.c_str(), title.size(), handle);
	DrawFormatStringToHandle((SCREEN_WIDTH - title_width) / 2, title_y, GetColor(255, 255, 255), handle, title.c_str());

	// 各ランキングの表示
	for (int i = 0; i < rankings.size(); ++i)
	{
		const auto& r = rankings[i];
		int y = 150 + i * 50;

		// ランキング行の背景
		DrawBox(300, y - 5, 980, y + 35, GetColor(30, 30, 30), TRUE); // 薄い背景

		// 順位に応じた色
		int color;
		switch (i)
		{
		case 0: color = GetColor(255, 215, 0); break;   // 金
		case 1: color = GetColor(192, 192, 192); break; // 銀
		case 2: color = GetColor(205, 127, 50); break;  // 銅
		default: color = GetColor(255, 255, 255); break;
		}

		int name_font = rm->GetFontHandle("Tepid Terminal", 30);
		// ランキングの描画
		DrawFormatStringToHandle(320, y + 5, color, name_font, "%2d 位", i + 1);
		DrawFormatStringToHandle(440, y + 5, GetColor(200, 200, 255), name_font, "%-8s", r.name.c_str());
		DrawFormatStringToHandle(700, y + 5, GetColor(255, 255, 200), name_font, "%6d LOOP", r.score);
	}

	// 戻るヒント
	int hint_font = rm->GetFontHandle("Tepid Terminal", 24);
	std::string hint = "Press [A] to return to Title";
	int hint_width = GetDrawStringWidthToHandle(hint.c_str(), hint.size(), hint_font);
	DrawFormatStringToHandle((SCREEN_WIDTH - hint_width) / 2, SCREEN_HEIGHT - 50, GetColor(180, 180, 180), hint_font, hint.c_str());
}


void RankingScene::Finalize()
{
}

eSceneType RankingScene::GetNowSceneType() const
{
	return eSceneType::RESULT;
}

void RankingScene::PlaySoundSe(int _handle, int volume)
{
	ChangeVolumeSoundMem(volume, _handle);
	PlaySoundMem(_handle, DX_PLAYTYPE_BACK); // SEは1回のみ再生
}
