#include "HelpScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"

HelpScene::HelpScene()
{
}

HelpScene::~HelpScene()
{
}

void HelpScene::Initialize()
{
}

eSceneType HelpScene::Update()
{
	InputControl* input = InputControl::GetInstance();
	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		return eSceneType::TITLE; // 戻るボタンでタイトルへ
	}
	return __super::Update();
}

void HelpScene::Draw() const
{
	DrawString(20, 20, "Help Scene", GetColor(255, 255, 255));
}

void HelpScene::Finalize()
{
}

eSceneType HelpScene::GetNowSceneType() const
{
	return eSceneType::HELP;
}
