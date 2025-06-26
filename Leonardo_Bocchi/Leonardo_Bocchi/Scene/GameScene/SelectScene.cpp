#include "SelectScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"
#include "../../Utility/ResourceManager.h"
#include "../../common.h"
#include "GameMainScene.h"

void SelectScene::Initialize()
{
	font = ResourceManager::GetInstance()->GetFontHandle("Tepid Terminal", 40);
	select_index = 0;

    LoadResource();
}

eSceneType SelectScene::Update()
{
    InputControl* input = InputControl::GetInstance();

    if (input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN)) {
        PlaySoundMusic(select_se, 70, 1);
        select_index = (select_index + 1) % 2;
    }
    if (input->GetButtonDown(XINPUT_BUTTON_DPAD_UP)) {
        PlaySoundMusic(select_se, 70, 1);
        select_index = (select_index + 1) % 2; 
    }

    if (input->GetButtonDown(XINPUT_BUTTON_A)) {
        PlaySoundMusic(decision_se, 70, 1);
        StopSoundMem(select_bgm);
        GameMainScene::SetStageMode(select_index == 1 ? StageMode::TUTORIAL : StageMode::MAIN);
        return eSceneType::GAME_MAIN;
    }
    else if (input->GetButtonDown(XINPUT_BUTTON_B)) {
        PlaySoundMusic(decision_se, 70, 1);
        StopSoundMem(select_bgm);

        return eSceneType::TITLE;
    }

    return __super::Update();
}

void SelectScene::Draw()
{
    ResourceManager* rm = ResourceManager::GetInstance();

    // 背景
    DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(10, 10, 30), TRUE);

    int title_font = rm->GetFontHandle("Tepid Terminal", 64);
    std::string select_text = "== STAGE SELECT ==";
    int result_width = GetDrawStringWidthToHandle(select_text.c_str(), select_text.size(), title_font);
    int result_x = (SCREEN_WIDTH - result_width) / 2;
    DrawStringToHandle(result_x, 50, select_text.c_str(), GetColor(255, 255, 255), title_font);

    // ▼ 選択肢リスト
    const char* options[] = {
        "Main Stage",
        "Tutorial Stage"
    };

    const int option_count = 2;
    const int line_height = 70;
    const int box_h = 60;

    // 全体の高さを求める → 画面中央に配置
    int total_height = option_count * line_height;
    int base_y = (SCREEN_HEIGHT - total_height) / 2; 

    for (int i = 0; i < option_count; ++i) {
        bool is_selected = (i == select_index);

        int color_text = is_selected ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
        int bg_color = is_selected ? GetColor(80, 80, 120) : GetColor(30, 30, 60);

        int width = GetDrawStringWidthToHandle(options[i], strlen(options[i]), font);
        int box_w = width + 65;
        int x = (SCREEN_WIDTH - box_w) / 2;
        int y = base_y + i * line_height;


        // 背景ボックス
        //DrawBox(x, y, x + box_w, y + box_h, bg_color, TRUE);
        if (is_selected) {
            DrawStringToHandle(x - 10, y + 10, ">>", color_text, font);
            DrawLine((SCREEN_WIDTH - width) / 2, y + 50, (SCREEN_WIDTH - width) / 2 + width, y + 50, GetColor(255, 255, 0));
        }

        DrawStringToHandle(x + 40, y + 10, options[i], color_text, font);
    }

    // 操作ヒント（下部センター）
    int small_font = rm->GetFontHandle("Tepid Terminal", 20);
    const char* hint = "[UP/DOWN]: Select  [A]: Decide  [B]: Back to Title";
    int hint_w = GetDrawStringWidthToHandle(hint, -1, small_font);
    DrawStringToHandle((SCREEN_WIDTH - hint_w) / 2, SCREEN_HEIGHT - 50, hint, GetColor(180, 180, 180), small_font);
}



void SelectScene::Finalize()
{
}

eSceneType SelectScene::GetNowSceneType() const
{
    return eSceneType::SELECT;
}

void SelectScene::LoadResource()
{
    ResourceManager* rm = ResourceManager::GetInstance();

    //BGM
    sounds_data = rm->GetSound("Resource/Sounds/BGM/AS_129910_躍動感のあるテクノサウンド.mp3");
    select_bgm = sounds_data[0];
    if (CheckSoundMem(select_bgm) != 1)  // 1は再生中の意味
    {
        PlaySoundMusic(select_bgm, 80, 0);
    }

    // SE
    sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1587112_選択・決定音（SFサイバー）1.mp3");
    select_se = sounds_data[0];

    sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1296213_サイバーな感じの決定音.mp3");
    decision_se = sounds_data[0];
}

void SelectScene::PlaySoundMusic(int _handle, int volume, int type)
{
    if (type == 0)
    {
        ChangeVolumeSoundMem(volume, _handle);
        PlaySoundMem(_handle, DX_PLAYTYPE_LOOP); // BGMをループ再生
    }
    else
    {
        ChangeVolumeSoundMem(volume, _handle);
        PlaySoundMem(_handle, DX_PLAYTYPE_BACK); // SEは1回のみ再
    }
}

