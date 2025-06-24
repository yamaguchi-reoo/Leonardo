#include "TitleScene.h"
#include "DxLib.h"
#include "../../common.h"
#include "../../Utility/InputControl.h"
#include "../../Utility/ResourceManager.h"
#include <time.h>

#define  MENU_NUM 5

TitleScene::TitleScene() :select_index(0), menu_font(-1), title_font(-1), small_font(-1), cursor_image(-1), cursor_frame(0), cursor_timer(0), title_bgm(-1), select_se(-1), decision_se(-1), is_decided(false), decision_timer(-1), next_scene(eSceneType::TITLE)
{
}

TitleScene::~TitleScene()
{
}
void TitleScene::Initialize()
{
    ResourceManager* rm = ResourceManager::GetInstance();
    rm->LoadFont("Resource/Font/TepidTerminal.ttf", "Tepid Terminal");

    menu_font = rm->GetFontHandle("Tepid Terminal", 40);
    title_font = rm->GetFontHandle("Tepid Terminal", 80);
    small_font = rm->GetFontHandle("Tepid Terminal", 20);

	LoadResource();
}

eSceneType TitleScene::Update()
{
	InputControl* input = InputControl::GetInstance();

    //効果音再生中は何もしないで、再生完了を待つ
    if (is_decided)
    {
		decision_timer++;
        if (decision_timer >= 30)
        {
            return next_scene; // 効果音が終わったら遷移
        }
        else
        {
            return eSceneType::TITLE; // 待機
        }
    }

	if (input->GetKeyDown(KEY_INPUT_DOWN) || input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		select_index = (select_index + 1) % 5;
        PlaySoundSe(select_se, 70); // 選択音を再生
	}
	else if (input->GetKeyDown(KEY_INPUT_UP) || input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		select_index = (select_index - 1 + 5) % 5;
        PlaySoundSe(select_se, 70); // 選択音を再生
	}


    if (input->GetButtonDown(XINPUT_BUTTON_A))
    {
        PlaySoundSe(decision_se, 50); // タイトルBGMを再生
        is_decided = true;
        decision_timer = 0;

        switch (select_index)
        {
        case MENU_START:
            //return eSceneType::GAME_MAIN;
            next_scene = eSceneType::GAME_MAIN;
            StopTitleSound(); // タイトルBGMを停止
			break;
        case MENU_HELP:
			next_scene = eSceneType::HELP; // ヘルプシーンへ移行
			break;
        case MENU_RANKING:
			// ランキングシーンへ移行
			next_scene = eSceneType::RANKING;
			break;
		case MENU_CREDITS: 
			next_scene = eSceneType::CREDITS; // クレジットシーンへ移行
			break;
        case MENU_END:
            DxLib_End(); // DxLib を終了してアプリ終了
            StopTitleSound(); // タイトルBGMを停止
            exit(0);
        }

    }

    cursor_timer++;
	if (cursor_timer >= 10)
	{
       if(!animation_data.empty())
       {
           cursor_frame = (cursor_frame + 1) % animation_data.size();
       }
        cursor_timer = 0;
	}

	return __super::Update();
}

void TitleScene::Draw()
{
    DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(10, 10, 30), TRUE);

    const char* title = "Echo Run";
    int title_w = GetDrawStringWidthToHandle(title, -1, title_font);

    // タイトル影
    DrawStringToHandle((SCREEN_WIDTH - title_w) / 2 + 2, 102, title, GetColor(0, 0, 0), title_font);
    DrawStringToHandle((SCREEN_WIDTH - title_w) / 2, 100, title, GetColor(200, 255, 255), title_font);

    const char* menu_items[] = { "START", "HELP", "RANKING", "CREDITS", "END" };

    for (int i = 0; i < MENU_NUM; ++i)
    {
        const char* text = menu_items[i];
        int width = GetDrawStringWidthToHandle(text, -1, menu_font);
        int y = 250 + i * 60;
        int color = (i == select_index) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);

        if (i == select_index)
        {
            // 左の黄色い四角はアニメーションカーソルに置き換えます
            int cursor_x = (SCREEN_WIDTH - width) / 2 - 50;
            int cursor_y = y + 10;

            DrawGraph(cursor_x, cursor_y - 10, animation_data[cursor_frame], TRUE);

            // 下線は残す
            DrawLine((SCREEN_WIDTH - width) / 2, y + 40, (SCREEN_WIDTH - width) / 2 + width, y + 40, GetColor(255, 255, 0));
        }

        DrawStringToHandle((SCREEN_WIDTH - width) / 2, y, text, color, menu_font);
    }

    const char* hint = "[UP/DOWN]: SELECT  [A]: DECISION";
    int hint_w = GetDrawStringWidthToHandle(hint, -1, small_font);
    DrawStringToHandle((SCREEN_WIDTH - hint_w) / 2, SCREEN_HEIGHT - 50, hint, GetColor(180, 180, 180), small_font);
}


void TitleScene::Finalize()
{
}

eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::TITLE;
}

void TitleScene::LoadResource()
{
	ResourceManager* rm = ResourceManager::GetInstance();

	// アニメーションデータの読み込み
    animation_data = rm->GetImages("Resource/Images/Character/Player/Player-jump/player-jump", 2);
    cursor_image = animation_data[0];

    //BGM
	sounds_data = rm->GetSound("Resource/Sounds/BGM/AS_129910_躍動感のあるテクノサウンド.mp3");
	title_bgm = sounds_data[0];
    if (CheckSoundMem(title_bgm) != 1)  // 1は再生中の意味
    {
        PlaySoundBgm(title_bgm, 80);
    }

	// SE
	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1587112_選択・決定音（SFサイバー）1.mp3");
	select_se = sounds_data[0];

    sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1296213_サイバーな感じの決定音.mp3");
    decision_se = sounds_data[0];
}

void TitleScene::PlaySoundBgm(int _handle, int volume)
{
    ChangeVolumeSoundMem(volume, _handle);
    PlaySoundMem(_handle, DX_PLAYTYPE_LOOP); // BGMをループ再生
}

void TitleScene::PlaySoundSe(int _handle, int volume)
{
    ChangeVolumeSoundMem(volume, _handle);
    PlaySoundMem(_handle, DX_PLAYTYPE_BACK); // SEは1回のみ再生
}


void TitleScene::StopTitleSound()
{
    StopSoundMem(title_bgm);
}
