#include "TitleScene.h"
#include "DxLib.h"
#include "../../Utility/InputControl.h"

TitleScene::TitleScene():select_index(0),menu_font(-1),title_font(-1),small_font(-1)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    menu_font = CreateFontToHandle("ＭＳ ゴシック", 28, 1);
    title_font = CreateFontToHandle("ＭＳ ゴシック", 48, 1);
    small_font = CreateFontToHandle("ＭＳ ゴシック", 18, 1);
}

eSceneType TitleScene::Update()
{
	InputControl* input = InputControl::GetInstance();

	if (input->GetKeyDown(KEY_INPUT_DOWN) || input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		select_index = (select_index + 1) % 4;
	}
	else if (input->GetKeyDown(KEY_INPUT_UP) || input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		select_index = (select_index - 1 + 4) % 4;
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
        case MENU_END:
            DxLib_End(); // DxLib を終了してアプリ終了
            exit(0);
        }
    }



	return __super::Update();
}

void TitleScene::Draw() const
{
	DrawFormatString(10, 10, GetColor(255, 255, 255), "Title Scene");

    const char* menu_items[] = {
        "START",
        "HELP",
        "RANKING",
        "END"
    };

    DrawStringToHandle(480, 150, "【タイトル画面】", GetColor(200, 255, 255), title_font);

    for (int i = 0; i < 4; ++i)
    {
        int color = (i == select_index) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
        DrawStringToHandle(550, 250 + i * 40, menu_items[i], color, menu_font);
    }

    //DrawStringToHandle(480, 500, "↑↓で選択 / Aで決定", GetColor(150, 150, 150), smallFont);
}

void TitleScene::Finalize()
{
}

eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::TITLE;
}
