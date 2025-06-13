#include "ResultScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"
#include "../../Utility/ResourceManager.h"
#include "../../common.h"

ResultScene::ResultScene()
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
}

eSceneType ResultScene::Update()
{
	InputControl* input = InputControl::GetInstance();

	// 数字が徐々に増える処理
	if (display_clear_count < clear_count)
	{
		clear_timer++;
		if (clear_timer % 4 == 0)  // 2フレームごとに増加
		{
			display_clear_count++;
		}
	}

	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		// Aボタンが押されたらゲームメインシーンに移行
		//return eSceneType::TITLE;
		return eSceneType::NAME_INPUT;
		clear_count = 0;
	}
	return __super::Update();
}

void ResultScene::Draw() const
{
	ResourceManager* rm = ResourceManager::GetInstance();

	const char* result = "RESULT";
	int result_width = GetDrawStringWidthToHandle(result, -1, rm->GetFontHandle("Tepid Terminal", 64));
	DrawStringToHandle((SCREEN_WIDTH - result_width) / 2, 100, result, GetColor(255, 215, 0), rm->GetFontHandle("Tepid Terminal", 64));


	const char* clear_text = "Clear Count : %d";
	int clear_width = GetDrawStringWidthToHandle(clear_text, -1, rm->GetFontHandle("Tepid Terminal", 48));
	DrawFormatStringToHandle(((SCREEN_WIDTH - result_width) / 2) - 100, (SCREEN_HEIGHT / 2) + 10, GetColor(255, 255, 128), rm->GetFontHandle("Tepid Terminal", 48), clear_text, display_clear_count);


	const char* hint = "Press A to continue";
	int hint_width = GetDrawStringWidthToHandle(hint, -1, rm->GetFontHandle("Tepid Terminal", 24));
	DrawStringToHandle((SCREEN_WIDTH - hint_width) / 2, SCREEN_HEIGHT - 50, hint, GetColor(180, 180, 180), rm->GetFontHandle("Tepid Terminal", 24));

}

void ResultScene::Finalize()
{
}

eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::RESULT;
}

