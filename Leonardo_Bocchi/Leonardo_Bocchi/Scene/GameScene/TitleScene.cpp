#include "TitleScene.h"
#include "DxLib.h"
#include "../../common.h"
#include "../../Utility/InputControl.h"
#include "../../Utility/ResourceManager.h"
#include <time.h>

#define  MENU_NUM 5

TitleScene::TitleScene():select_index(0),menu_font(-1),title_font(-1),small_font(-1)
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
}

eSceneType TitleScene::Update()
{
	InputControl* input = InputControl::GetInstance();

	if (input->GetKeyDown(KEY_INPUT_DOWN) || input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		select_index = (select_index + 1) % 5;
	}
	else if (input->GetKeyDown(KEY_INPUT_UP) || input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		select_index = (select_index - 1 + 5) % 5;
	}

    if (input->GetButtonDown(XINPUT_BUTTON_A))
    {
        switch (select_index)
        {
        case MENU_START:
            return eSceneType::GAME_MAIN;
        case MENU_HELP:
            // 仮にヘルプもタイトルへ戻す
            return eSceneType::TITLE;
        case MENU_RANKING:
            // 仮にランキングもタイトルへ戻す
            return eSceneType::TITLE;
		case MENU_CREDITS: 
			// 仮にクレジットもタイトルへ戻す
			return eSceneType::TITLE;
        case MENU_END:
            DxLib_End(); // DxLib を終了してアプリ終了
            exit(0);
        }
    }



	return __super::Update();
}

void TitleScene::Draw() const
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

        // 選択中は左に四角を表示
        if (i == select_index)
        {
            DrawBox((SCREEN_WIDTH - width) / 2 - 50, y + 10, (SCREEN_WIDTH - width) / 2 - 30, y + 30, GetColor(255, 255, 0), TRUE);
            DrawLine((SCREEN_WIDTH - width) / 2, y + 40, (SCREEN_WIDTH - width) / 2 + width, y + 40, GetColor(255, 255, 0));
        }

        DrawStringToHandle((SCREEN_WIDTH - width) / 2, y, text, color, menu_font);
    }

    // 操作説明は日本語使えないので英語の簡単な文で
    const char* hint = "UP/DOWN: SELECT  A: DECISION";
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
