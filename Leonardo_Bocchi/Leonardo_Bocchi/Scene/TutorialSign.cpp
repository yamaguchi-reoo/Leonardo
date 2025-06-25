#include "TutorialSign.h"
#include <DxLib.h>
#include "../Utility/ResourceManager.h"
#include <cmath>
#include "../common.h"

TutorialSign::TutorialSign(const Vector2D& pos, const std::string& msg, float dis):
	font(-1),position(pos),message(msg), trigger_distance(dis), is_visible(false),visible_timer(0)
{
	auto rm = ResourceManager::GetInstance();
	font = rm->GetFontHandle("Tepid Terminal", 36);
}

TutorialSign::~TutorialSign()
{
}

void TutorialSign::Initialize()
{
    box_width = 0;
    box_height = 0;
    box_max_width = 300;
    box_max_height = 150;
    anim_timer = 0;
    is_animating = false;
}

void TutorialSign::Update(const Vector2D& player_pos)
{
    if (is_already_shown) return; // 既に表示完了なら何もしない

    float dx = std::abs(player_pos.x - position.x);

    if (!is_visible && !is_close && dx < trigger_distance)
    {
        // 表示開始
        is_visible = true;
        is_animating = true;
        anim_timer = 0;
        visible_timer = 0;
        box_width = 0;
        box_height = 0;
    }

    if (is_visible && !is_close)
    {
        if (anim_timer < 30)
        {
            anim_timer++;
            float t = static_cast<float>(anim_timer) / 30;
            box_width = box_max_width * t;
            box_height = box_max_height * t;
        }
        else
        {
            visible_timer++;
            if (visible_timer > 240)
            {
                is_close = true;
                is_visible = false;
                anim_timer = 30;
            }
        }
    }
    else if (is_close)
    {
        if (anim_timer > 0)
        {
            anim_timer--;
            float t = static_cast<float>(anim_timer) / 30;
            box_width = box_max_width * t;
            box_height = box_max_height * t;
        }
        else
        {
            is_close = false;
            visible_timer = 0;
            box_width = 0;
            box_height = 0;
            is_already_shown = true;  
        }
    }
}


void TutorialSign::Draw()
{
    if (!is_visible && !is_animating && !is_close) return;

    int x = static_cast<int>(position.x - box_width / 2);
    int y = static_cast<int>(position.y - box_height / 2);

    int bg_color = GetColor(50, 50, 50);
    int frame_color = GetColor(255, 255, 200);

    // ボックス描画（展開・縮小中）
    if (box_width > 0 && box_height > 0)
    {
        DrawBox(x, y, x + static_cast<int>(box_width), y + static_cast<int>(box_height), bg_color, TRUE);
        DrawBox(x, y, x + static_cast<int>(box_width), y + static_cast<int>(box_height), frame_color, FALSE);
    }

    // 展開完了したら文字描画
    if (anim_timer >= 30 && !is_close)
    {
        int font = ResourceManager::GetInstance()->GetFontHandle("Tepid Terminal", 24);

        int line_y = y + 20;
        size_t start = 0;
        size_t end = 0;

        while ((end = message.find('\n', start)) != std::string::npos)
        {
            std::string line = message.substr(start, end - start);
            int line_w = GetDrawStringWidthToHandle(line.c_str(), line.size(), font);
            int line_x = (SCREEN_WIDTH - line_w) / 2;
            DrawStringToHandle(line_x, line_y, line.c_str(), GetColor(255, 255, 255), font);
            line_y += 28;
            start = end + 1;
        }

        std::string last_line = message.substr(start);
        int last_w = GetDrawStringWidthToHandle(last_line.c_str(), last_line.size(), font);
        int last_x = (SCREEN_WIDTH - last_w) / 2;
        DrawStringToHandle(last_x, line_y, last_line.c_str(), GetColor(255, 255, 255), font);
    }
}