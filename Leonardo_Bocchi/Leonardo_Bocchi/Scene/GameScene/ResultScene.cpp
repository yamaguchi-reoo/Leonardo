#include "ResultScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/UserTemplate.h"
#include "../RankingManager.h"
#include "../../common.h"

ResultScene::ResultScene() : decision_se(-1), result_se(-1)
{
}

ResultScene::~ResultScene()
{
	display_clear_count = 0;
	clear_timer = 0;
}

void ResultScene::Initialize()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	rm->LoadFont("Resource/Font/TepidTerminal.ttf", "Tepid Terminal");

	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1296213_サイバーな感じの決定音.mp3");
	decision_se = sounds_data[0];

	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_97539_画面表示／スクリーンON／解析／デジタル_Audio Trimmer.mp3");
	result_se = sounds_data[0];

	PlaySoundSe(result_se, 100);
}

eSceneType ResultScene::Update()
{
	InputControl* input = InputControl::GetInstance();

	// ボックス展開アニメーション
	if (!is_box_expanded)
	{
		box_anim_timer++;
		if (box_anim_timer >= box_anim_duration)
		{
			box_anim_timer = box_anim_duration;
			is_box_expanded = true;
		}
	}
	else
	{
		// スコア表示アニメーション
		if (display_clear_count < clear_count)
		{
			clear_timer++;
			if (clear_timer % 5 == 0)
			{
				display_clear_count++;
			}
		}
		else
		{
			is_score_count_done = true; // アニメーション完了フラグ
		}
	}

	// スキップ入力（※アニメーション完了後のみ許可）
	if (is_box_expanded && is_score_count_done)
	{
		if (input->GetButtonDown(XINPUT_BUTTON_A))
		{
			PlaySoundSe(decision_se, 70); // 決定音を再生

			// ランキング比較
			const auto& rankings = RankingManager::GetInstance()->GetRankings();
			bool is_high_score = false;

			if (rankings.size() < 10)
			{
				is_high_score = true;
			}
			else
			{
				for (const auto& r : rankings)
				{
					if (clear_count >= r.score)
					{
						is_high_score = true;
						break;
					}
				}
			}

			// スコアがランキングに入る場合は名前入力へ
			if (is_high_score)
			{
				return eSceneType::NAME_INPUT;
			}
			else
			{
				clear_count = 0;
				return eSceneType::TITLE;
			}
		}
	}

	return __super::Update();
}



void ResultScene::Draw()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	int title_font = rm->GetFontHandle("Tepid Terminal", 64);
	int score_font = rm->GetFontHandle("Tepid Terminal", 48);
	int hint_font = rm->GetFontHandle("Tepid Terminal", 24);


	std::string result_text = "== RESULT ==";
	int result_width = GetDrawStringWidthToHandle(result_text.c_str(), result_text.size(), title_font);
	int result_x = (SCREEN_WIDTH - result_width) / 2;
	DrawStringToHandle(result_x + 4, 104, result_text.c_str(), GetColor(0, 0, 0), title_font); // 影
	DrawStringToHandle(result_x, 50, result_text.c_str(), GetColor(255, 255, 255), title_font);  // 金色


	std::string clear_text = "Clear Count : " + std::to_string(display_clear_count);
	int clear_width = GetDrawStringWidthToHandle(clear_text.c_str(), clear_text.size(), score_font);
	int clear_x = (SCREEN_WIDTH - clear_width) / 2;


	//////////////////////////////////////

	// 中央のスコア表示用ボックスの展開アニメーション
	float t = (float)box_anim_timer / box_anim_duration; // 0.0〜1.0
	t = Clamp(t, 0.0f, 1.0f);

	// 展開する矩形サイズの補間
	int box_max_width = clear_width + 40;
	int box_max_height = 70;

	int box_width = static_cast<int>(box_max_width * t);
	int box_height = static_cast<int>(box_max_height * t);

	// 中心を基準に描画
	int center_x = SCREEN_WIDTH / 2;
	int center_y = SCREEN_HEIGHT / 2 + 25;

	int left = center_x - box_width / 2;
	int right = center_x + box_width / 2;
	int top = center_y - box_height / 2;
	int bottom = center_y + box_height / 2;

	// 展開するボックスの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(t * 255));
	DrawBox(left, top, right, bottom, GetColor(100, 100, 100), TRUE);  // 中身
	DrawBox(left, top, right, bottom, GetColor(255, 255, 128), FALSE); // 枠線
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 展開が完了したら文字を描画
	if (is_box_expanded)
	{
		DrawStringToHandle(clear_x + 2, SCREEN_HEIGHT / 2 + 2, clear_text.c_str(), GetColor(0, 0, 0), score_font); // 影
		DrawStringToHandle(clear_x, SCREEN_HEIGHT / 2, clear_text.c_str(), GetColor(255, 255, 128), score_font);  // 本体
	}

	/////////////////////////////////


	std::string hint = "Press [A] to";
	int hint_width = GetDrawStringWidthToHandle(hint.c_str(), hint.size(), hint_font);
	int hint_x = (SCREEN_WIDTH - hint_width) / 2;

	DrawStringToHandle(hint_x + 2, SCREEN_HEIGHT - 48, hint.c_str(), GetColor(0, 0, 0), hint_font);   // 影
	DrawStringToHandle(hint_x, SCREEN_HEIGHT - 50, hint.c_str(), GetColor(180, 180, 180), hint_font); // 本体
}


void ResultScene::Finalize()
{
}

eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::RESULT;
}

void ResultScene::PlaySoundSe(int _handle, int volume)
{
	ChangeVolumeSoundMem(volume, _handle);
	PlaySoundMem(_handle, DX_PLAYTYPE_BACK); // SEは1回のみ再生
}

