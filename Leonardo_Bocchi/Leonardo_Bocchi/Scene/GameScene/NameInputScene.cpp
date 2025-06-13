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
	keys.push_back("Space");
	keys.push_back("Delete");
	keys.push_back("OK");

	grid_width = 10; // 必要なら明示的にセット
	grid_height = (keys.size() + grid_width - 1) / grid_width;

	player_name.clear();
	cursor_x = 0;
	cursor_y = 0;
	is_enter_name = false;

	ResourceManager* rm = ResourceManager::GetInstance();
	rm->LoadFont("Resource/Font/TepidTerminal.ttf", "Tepid Terminal");
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

void NameInputScene::Draw() const
{
	ResourceManager* rm = ResourceManager::GetInstance();
	// 名前表示
	//DrawFormatString(240, 100, GetColor(255, 255, 255), "NAME : %s_", player_name.c_str());
	DrawFormatStringToHandle(240, 100, GetColor(255, 255, 255), rm->GetFontHandle("Tepid Terminal", 24), "NAME : % s_", player_name.c_str());

	// キーボード描画
	DrawKeyboard();
}

void NameInputScene::Finalize()
{
}

eSceneType NameInputScene::GetNowSceneType() const
{
	return eSceneType();
}

void NameInputScene::HandleInput()
{
	InputControl* input = InputControl::GetInstance();

	input_cooldown--;
	if (input_cooldown > 0) return; // クールダウン中は入力を無視

	if (input->GetButtonDown(XINPUT_BUTTON_DPAD_RIGHT)) 
	{
		cursor_x = (cursor_x + 1) % grid_width; // 右に移動
		input_cooldown = 10; // クールダウンタイマーを設定
	}
	else if (input->GetButtonDown(XINPUT_BUTTON_DPAD_LEFT))
	{
		cursor_x = (cursor_x - 1 + grid_width) % grid_width; // 左に移動
		input_cooldown = 10; // クールダウンタイマーを設定
	}
	else if (input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		cursor_y = (cursor_y + 1) % grid_height;
		input_cooldown = 10;
	}
	else if (input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		cursor_y = (cursor_y - 1 + grid_height) % grid_height;
		input_cooldown = 10;
	}
	

	int index = cursor_y * grid_width + cursor_x;
	if (index < keys.size() && input->GetButtonDown(XINPUT_BUTTON_A))
	{
		const std::string& key = keys[index];

		if (key == "Delete")
		{
			if (!player_name.empty())
			{
				player_name.pop_back(); // 最後の文字を削除
			}
		}
		else if (key == "OK")
		{
			if (!player_name.empty())
			{
				RankingManager::GetInstance()->AddEntry(player_name, clear_count);
				is_enter_name = true; // 名前入力完了
			}
		}
		else if (key == "Space")
		{
			if (player_name.size() < max_name_length)
			{
				player_name += " "; // スペースを追加
			}
		}
		else
		{
			if (player_name.size() < max_name_length)
			{
				player_name += key; // キーを追加
			}
		}
	}
}

void NameInputScene::DrawKeyboard() const
{
	ResourceManager* rm = ResourceManager::GetInstance();

	const int start_x = 100; // キーボードの開始X座標
	const int start_y = 150; // キーボードの開始Y座標
	const int key_size = 48; // キーのサイズ
	const int padding = 10; // キー間のパディング

	for (int i = 0; i < keys.size(); ++i)
	{
		int x = i % grid_width;
		int y = i / grid_width;
		int pos_x = start_x + x * (key_size + padding);
		int pos_y = start_y + y * (key_size + padding);

		// カーソルがある位置は赤、それ以外は白
		int color = (cursor_x == x && cursor_y == y) ? GetColor(255, 0, 0) : GetColor(255, 255, 255);

		DrawBox(pos_x, pos_y, pos_x + key_size, pos_y + key_size, color, TRUE); // カーソル位置を反映

		//DrawFormatString(pos_x + 10, pos_y + 10, GetColor(0, 0, 0), "%s", keys[i].c_str()); // キーのテキスト

		DrawFormatStringToHandle(pos_x + 10, pos_y + 10, GetColor(0, 0, 0), rm->GetFontHandle("Tepid Terminal", 24), "%s", keys[i].c_str());
	}
}
