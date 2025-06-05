#include "ResultScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"
#include "../../common.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
}

eSceneType ResultScene::Update()
{
	InputControl* input = InputControl::GetInstance();

	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		// Aボタンが押されたらゲームメインシーンに移行
		return eSceneType::TITLE;
	}
	return __super::Update();
}

void ResultScene::Draw() const
{
	//DrawFormatString(10, 10, GetColor(255, 255, 255), "Result Scene");


	SetFontSize(48);
	DrawFormatString(450, 250, GetColor(255, 255, 255), "クリア回数: %d 回", clear_count);


	SetFontSize(24);
	const char* hint = "Press A to continue";
	int hint_width = GetDrawStringWidth(hint, strlen(hint));
	DrawString((SCREEN_WIDTH - hint_width) / 2, SCREEN_HEIGHT / 2 + 90, hint, GetColor(255, 255, 255));
}

void ResultScene::Finalize()
{
}

eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::RESULT;
}

