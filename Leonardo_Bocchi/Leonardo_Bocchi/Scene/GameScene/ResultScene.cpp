#include "ResultScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"
#include "../../Utility/ResourceManager.h"
#include "../RankingManager.h"
#include "../../common.h"

ResultScene::ResultScene(): decision_se(-1)
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

	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1296213_�T�C�o�[�Ȋ����̌��艹.mp3");
	decision_se = sounds_data[0];
}

eSceneType ResultScene::Update()
{
	InputControl* input = InputControl::GetInstance();

	// ���������X�ɑ����鏈��
	if (display_clear_count < clear_count)
	{
		clear_timer++;
		if (clear_timer % 4 == 0)  // 2�t���[�����Ƃɑ���
		{
			display_clear_count++;
		}
	}

	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		PlaySoundSe(decision_se, 70); // ���艹���Đ�
		// �����L���O�Ɣ�r
		const auto& rankings = RankingManager::GetInstance()->GetRankings();
		bool is_high_score = false;

		if (rankings.size() < 10)
		{
			is_high_score = true; // �����L���O��10�������Ȃ�K���n�C�X�R�A
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

		// �X�R�A�������L���O�ɓ���ꍇ�͖��O���͂�
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
	DrawStringToHandle(result_x + 4, 104, result_text.c_str(), GetColor(0, 0, 0), title_font); // �e
	DrawStringToHandle(result_x, 50, result_text.c_str(), GetColor(255, 255, 255), title_font);  // ���F


	std::string clear_text = "Clear Count : " + std::to_string(display_clear_count);
	int clear_width = GetDrawStringWidthToHandle(clear_text.c_str(), clear_text.size(), score_font);
	int clear_x = (SCREEN_WIDTH - clear_width) / 2;


	DrawBox(clear_x - 20, SCREEN_HEIGHT / 2 - 10, clear_x + clear_width + 20, SCREEN_HEIGHT / 2 + 60, GetColor(100, 100, 100), TRUE);
	DrawBox(clear_x - 20, SCREEN_HEIGHT / 2 - 10, clear_x + clear_width + 20, SCREEN_HEIGHT / 2 + 60, GetColor(255, 255, 128), FALSE);

	DrawStringToHandle(clear_x + 2, SCREEN_HEIGHT / 2 + 2, clear_text.c_str(), GetColor(0, 0, 0), score_font); // �e
	DrawStringToHandle(clear_x, SCREEN_HEIGHT / 2, clear_text.c_str(), GetColor(255, 255, 128), score_font);  // �{��


	std::string hint = "Press [A] to";
	int hint_width = GetDrawStringWidthToHandle(hint.c_str(), hint.size(), hint_font);
	int hint_x = (SCREEN_WIDTH - hint_width) / 2;

	DrawStringToHandle(hint_x + 2, SCREEN_HEIGHT - 48, hint.c_str(), GetColor(0, 0, 0), hint_font);   // �e
	DrawStringToHandle(hint_x, SCREEN_HEIGHT - 50, hint.c_str(), GetColor(180, 180, 180), hint_font); // �{��
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
	PlaySoundMem(_handle, DX_PLAYTYPE_BACK); // SE��1��̂ݍĐ�
}

