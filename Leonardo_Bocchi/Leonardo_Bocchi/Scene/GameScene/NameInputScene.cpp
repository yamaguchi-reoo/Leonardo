#include "NameInputScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"	
#include "../../Utility/ResourceManager.h"
#include "../../common.h"
#include "../RankingManager.h"

void NameInputScene::Initialize()
{
	// キー配列作成
	for (char c = 'A'; c <= 'Z'; ++c) keys.push_back(std::string(1, c));
	for (char c = '0'; c <= '9'; ++c) keys.push_back(std::string(1, c));
	keys.push_back("Sp");
	keys.push_back("Del");
	keys.push_back("OK");

	grid_width = 10; // 必要なら明示的にセット
	grid_height = (keys.size() + grid_width - 1) / grid_width;

	player_name.clear();
	cursor_x = 0;
	cursor_y = 0;
	is_enter_name = false;

	ResourceManager* rm = ResourceManager::GetInstance();
	rm->LoadFont("Resource/Font/TepidTerminal.ttf", "Tepid Terminal");


	LoadResource();
}


eSceneType NameInputScene::Update()
{
	if (!is_enter_name)
	{
		// 入力処理
		HandleInput();
	}
	else
	{
		return eSceneType::RANKING; // ランキングシーンへ移行
	}
	return __super::Update();
}

void NameInputScene::Draw()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	// 名前表示
	std::string display_text = "NAME : " + player_name + "_";
	int text_width = GetDrawStringWidthToHandle(display_text.c_str(), -1, rm->GetFontHandle("Tepid Terminal", 24));
	DrawFormatStringToHandle((SCREEN_WIDTH - text_width) / 2, 100, GetColor(255, 255, 255), rm->GetFontHandle("Tepid Terminal", 24), "NAME : % s_", player_name.c_str());

	// キーボード描画
	DrawKeyboard();
}

void NameInputScene::Finalize()
{
}

eSceneType NameInputScene::GetNowSceneType() const
{
	return eSceneType::NAME_INPUT; // 現在のシーンタイプを返す
}

void NameInputScene::HandleInput()
{
	InputControl* input = InputControl::GetInstance();

	input_cooldown--;
	if (input_cooldown > 0) return; // クールダウン中は入力を無視

	int prev_x = cursor_x;
	int prev_y = cursor_y;

	if (input->GetButtonDown(XINPUT_BUTTON_DPAD_RIGHT))
	{
		cursor_x = (cursor_x + 1) % grid_width;
		if (cursor_y * grid_width + cursor_x >= keys.size()) cursor_x = prev_x;
		else PlaySoundSe(select_se, 90);
		input_cooldown = 5;
	}
	else if (input->GetButtonDown(XINPUT_BUTTON_DPAD_LEFT))
	{
		cursor_x = (cursor_x - 1 + grid_width) % grid_width;
		if (cursor_y * grid_width + cursor_x >= keys.size()) cursor_x = prev_x;
		else PlaySoundSe(select_se, 90);
		input_cooldown = 5;
	}
	else if (input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		cursor_y = (cursor_y + 1) % grid_height;
		if (cursor_y * grid_width + cursor_x >= keys.size()) cursor_y = prev_y;
		else PlaySoundSe(select_se, 90);
		input_cooldown = 5;
	}
	else if (input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		cursor_y = (cursor_y - 1 + grid_height) % grid_height;
		if (cursor_y * grid_width + cursor_x >= keys.size()) cursor_y = prev_y;
		else PlaySoundSe(select_se, 90);
		input_cooldown = 5;
	}


	int index = cursor_y * grid_width + cursor_x;
	if (index < keys.size() && input->GetButtonDown(XINPUT_BUTTON_A))
	{
		const std::string& key = keys[index];

		if (key == "Del")
		{
			PlaySoundSe(decision_se, 90); // 選択音を再生
			if (!player_name.empty())
			{
				player_name.pop_back(); // 最後の文字を削除
			}
		}
		else if (key == "OK")
		{
			PlaySoundSe(decision_se, 90); // 決定音を再生
			if (!player_name.empty())
			{
				RankingManager::GetInstance()->AddEntry(player_name, clear_count);
				RankingManager::GetInstance()->Save(); // ランキングを保存
				is_enter_name = true; // 名前入力完了
			}
		}
		else if (key == "Sp")
		{
			PlaySoundSe(decision_se, 90); // 選択音を再生
			if (player_name.size() < max_name_length)
			{
				player_name += " "; // スペースを追加
			}
		}
		else
		{
			if (player_name.size() < max_name_length)
			{
				PlaySoundSe(decision_se, 50); // 選択音を再生
				player_name += key; // キーを追加
			}
		}
	}
}

void NameInputScene::DrawKeyboard() const
{
	ResourceManager* rm = ResourceManager::GetInstance();
	int handle = rm->GetFontHandle("Tepid Terminal", 24);

	const int key_size = 48;
	const int padding = 10;
	const int key_width_total = key_size + padding;

	int total_columns = grid_width;
	int total_rows = (keys.size() + grid_width - 1) / grid_width;

	int total_width = total_columns * key_width_total - padding;
	int screen_width = 1280;
	int start_x = (screen_width - total_width) / 2;

	const int start_y = 150;

	for (int i = 0; i < keys.size(); ++i)
	{
		int x = i % grid_width;
		int y = i / grid_width;
		int pos_x = start_x + x * (key_size + padding);
		int pos_y = start_y + y * (key_size + padding);

		int color = (cursor_x == x && cursor_y == y) ? GetColor(255, 0, 0) : GetColor(255, 255, 255);

		DrawBox(pos_x, pos_y, pos_x + key_size, pos_y + key_size, color, TRUE);
		DrawFormatStringToHandle(pos_x + 10, pos_y + 10, GetColor(0, 0, 0), handle, "%s", keys[i].c_str());
	}
}

void NameInputScene::LoadResource()
{
	ResourceManager* rm = ResourceManager::GetInstance();

	// SE
	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1587112_選択・決定音（SFサイバー）1.mp3");
	select_se = sounds_data[0];

	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1296213_サイバーな感じの決定音.mp3");
	decision_se = sounds_data[0];
}

void NameInputScene::PlaySoundSe(int _handle, int volume)
{
	ChangeVolumeSoundMem(volume, _handle);
	PlaySoundMem(_handle, DX_PLAYTYPE_BACK); // SEは1回のみ再生
}

