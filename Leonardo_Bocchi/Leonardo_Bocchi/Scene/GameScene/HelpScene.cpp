#include "HelpScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"
#include "../../Utility/ResourceManager.h"
#include "../../common.h"

HelpScene::HelpScene() : font_small(-1), font_large(-1), current_page(0), total_pages(3)
{
}

HelpScene::~HelpScene()
{
}

void HelpScene::Initialize()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	font_small = rm->GetFontHandle("Tepid Terminal", 30);
	font_large = rm->GetFontHandle("Tepid Terminal", 50);


}

eSceneType HelpScene::Update()
{
	InputControl* input = InputControl::GetInstance();

	// ページ切り替え
	if (input->GetButtonDown(XINPUT_BUTTON_DPAD_RIGHT))
	{
		current_page++;
		if (current_page >= total_pages) current_page = 0; // 循環
	}
	else if (input->GetButtonDown(XINPUT_BUTTON_DPAD_LEFT))
	{
		current_page--;
		if (current_page < 0) current_page = total_pages - 1; // 循環
	}

	//Titleへ戻る処理
	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		return eSceneType::TITLE; // 戻るボタンでタイトルへ
	}
	return __super::Update();
}

void HelpScene::Draw() const
{
	ResourceManager* rm = ResourceManager::GetInstance();
	int title_font = rm->GetFontHandle("Tepid Terminal", 64);

	std::string help_text = "== HELP ==";
	int result_width = GetDrawStringWidthToHandle(help_text.c_str(), help_text.size(), title_font);
	int help_x = (SCREEN_WIDTH - result_width) / 2;
	DrawStringToHandle(help_x, 100, help_text.c_str(), GetColor(255, 255, 255), title_font);


	// 戻るヒント
	int hint_font = rm->GetFontHandle("Tepid Terminal", 24);
	std::string hint = "Press [A] to return to Title";
	int hint_width = GetDrawStringWidthToHandle(hint.c_str(), hint.size(), hint_font);
	DrawFormatStringToHandle((SCREEN_WIDTH - hint_width) / 2, SCREEN_HEIGHT - 50, GetColor(180, 180, 180), hint_font, hint.c_str());


	// ページ表示・ナビゲーション
	int page_font = rm->GetFontHandle("Tepid Terminal", 24);
	char page_info[32];
	sprintf_s(page_info, "<< %d / %d >>", current_page + 1, total_pages);
	int page_width = GetDrawStringWidthToHandle(page_info, -1, page_font);
	int page_x = (SCREEN_WIDTH - page_width) / 2;

	DrawStringToHandle(page_x, SCREEN_HEIGHT - 80, page_info, GetColor(200, 200, 200), page_font);

}

void HelpScene::Finalize()
{
}

eSceneType HelpScene::GetNowSceneType() const
{
	return eSceneType::HELP;
}

void HelpScene::LoadResource()
{

}
