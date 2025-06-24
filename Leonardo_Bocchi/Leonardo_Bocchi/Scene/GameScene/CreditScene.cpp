#include "CreditScene.h"
#include "../../Utility/ResourceManager.h"
#include "../../common.h"
#include "../../Utility/InputControl.h"
#include "TitleScene.h"


CreditScene::CreditScene()
{
}

CreditScene::~CreditScene()
{
}

void CreditScene::Initialize()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	rm->LoadFont("Resource/Font/TepidTerminal.ttf", "Tepid Terminal");
	credit_font = rm->GetFontHandle("Tepid Terminal", 32);


	scroll_pos = SCREEN_HEIGHT;
	is_skip = false;

	LoadCreditText(); // テキストを読み込み
}

eSceneType CreditScene::Update()
{
    InputControl* input = InputControl::GetInstance();

    if (input->GetButtonDown(XINPUT_BUTTON_A)) {
        is_skip = true;
        //StopSoundMem(credit_se);
        return eSceneType::TITLE;
    }

    if (!is_skip) {
        scroll_pos -= 2; // 上にスクロール
    }

    // スクロールがすべて終わったら自動でタイトルに戻る
    int total_scroll_height = static_cast<int>(credit_lines.size()) * 40;
    if (scroll_pos + total_scroll_height < 0) {
        return eSceneType::TITLE;
    }

    return __super::Update();
}


void CreditScene::Draw()
{
    DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 30), TRUE); // 背景

    int y = scroll_pos;
    for (const auto& line : credit_lines) {
        int w = GetDrawStringWidthToHandle(line.c_str(), line.size(), credit_font);
        DrawStringToHandle((SCREEN_WIDTH - w) / 2, y, line.c_str(), GetColor(255, 255, 255), credit_font);
        y += 40;
    }

    if (!is_skip) {
        DrawStringToHandle(20, SCREEN_HEIGHT - 50, "Press A to skip", GetColor(180, 180, 180), credit_font);
    }
}


void CreditScene::Finalize()
{
}

eSceneType CreditScene::GetNowSceneType() const
{
    return eSceneType::CREDITS;
}

void CreditScene::LoadCreditText()
{
    credit_lines = {
        "== CREDITS ==",
        "",
        "[Character Art]",
        "Ansimuz 様",
        "Player / Enemy : Super Grotto Escape Pack",
        "",
        "",
        "",

        "[Illustration]",
        "ACillust",
        "ChatGPT"
        "",
        "",
        "",

        "[Font]",
        "philtacular 様",
        "Font : tepid-terminal",
        "",
        "",
        "",
        "",

        "[BGM]",
        "AKM Music / Audiostock",
        "STUDIO COM / Audiostock",
        "",
        "",
        "",

        "[Sound Effects]",
        "green air / Audiostock",
        "Takumi Hidani / Audiostock",
        "Killy / Audiostock",
        "Sanwa Solution / Audiostock",
        "Suta・Day / Audiostock",
        "66ice / Audiostock",
        "OTOnoniwa / Audiostock",
        "MATSU / Audiostock",
        "FD312 / Audiostock",
        "Hamo / Audiostock",
        "Amagurineko / Audiostock",
        "new_field / Audiostock",
        "",
        "",
        "",
        "",

        "Thank you for playing!",
    };
}

