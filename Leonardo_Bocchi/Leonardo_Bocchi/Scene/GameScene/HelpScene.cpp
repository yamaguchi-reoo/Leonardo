#include "HelpScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"
#include "../../Utility/ResourceManager.h"
#include "../../common.h"

HelpScene::HelpScene() : font_small(-1), font_large(-1), current_page(0), total_pages(2)
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

	// �y�[�W�؂�ւ�
	if (input->GetButtonDown(XINPUT_BUTTON_DPAD_RIGHT))
	{
		current_page++;
		if (current_page >= total_pages) current_page = 0; // �z��
	}
	else if (input->GetButtonDown(XINPUT_BUTTON_DPAD_LEFT))
	{
		current_page--;
		if (current_page < 0) current_page = total_pages - 1; // �z��
	}

	//Title�֖߂鏈��
	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		return eSceneType::TITLE; // �߂�{�^���Ń^�C�g����
	}
	return __super::Update();
}

void HelpScene::Draw()
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(10, 10, 30), TRUE);
	ResourceManager* rm = ResourceManager::GetInstance();
	int title_font = rm->GetFontHandle("Tepid Terminal", 64);

	std::string help_text = "== HELP ==";
	int result_width = GetDrawStringWidthToHandle(help_text.c_str(), help_text.size(), title_font);
	int help_x = (SCREEN_WIDTH - result_width) / 2;
	DrawStringToHandle(help_x, 50, help_text.c_str(), GetColor(255, 255, 255), title_font);


	// �߂�q���g
	int hint_font = rm->GetFontHandle("Tepid Terminal", 24);
	std::string hint = "Press [A] to return to Title";
	int hint_width = GetDrawStringWidthToHandle(hint.c_str(), hint.size(), hint_font);
	DrawFormatStringToHandle((SCREEN_WIDTH - hint_width) / 2, SCREEN_HEIGHT - 50, GetColor(180, 180, 180), hint_font, hint.c_str());


	// �y�[�W�\���E�i�r�Q�[�V����
	int page_font = rm->GetFontHandle("Tepid Terminal", 24);
	char page_info[32];
	sprintf_s(page_info, "<< %d / %d >>", current_page + 1, total_pages);
	int page_width = GetDrawStringWidthToHandle(page_info, -1, page_font);
	int page_x = (SCREEN_WIDTH - page_width) / 2;

	DrawStringToHandle(page_x, SCREEN_HEIGHT - 80, page_info, GetColor(200, 200, 200), page_font);


	switch (current_page){
		case 0:DrawPlayerControls();break;
		case 1:DrawObjectPage(); break;
	}

}

void HelpScene::Finalize()
{
}

eSceneType HelpScene::GetNowSceneType() const
{
	return eSceneType::HELP;
}

void HelpScene::DrawPlayerControls()
{
}

void HelpScene::DrawObjectPage()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	int font = rm->GetFontHandle("Tepid Terminal", 24);

	std::vector<HelpInfo> items = {
		{"Resource/Images/Items/heal.png", "Heal Item", "HP Healing", HelpObjectType::Image},
		{"Resource/Images/Items/adamas.png", "Barrier Item", "Invincible", HelpObjectType::Image},
		{"Resource/Images/Gimmick/trap01.png", "Trap", "Damage on touch", HelpObjectType::Image},
		{"", "Teleport", "Go to next loop", HelpObjectType::Shapes}
	};

	int spacing_y = 100;

	// �� �����񂹂̂��߂̏���
	int max_item_width = 300; // ���̍ő啝�i�摜�{�e�L�X�g�j�B�K�v�ɉ����Ĕ������B
	int x = (SCREEN_WIDTH - max_item_width) / 2;
	int y = 200;

	for (const auto& item : items) {
		if (item.type == HelpObjectType::Image) {
			int img = LoadGraph(item.image_path.c_str());
			DrawGraph(x, y, img, TRUE);
		}
		else if (item.type == HelpObjectType::Shapes) {
			DrawTeleport(x, y);
		}

		DrawStringToHandle(x + 100, y, item.name.c_str(), GetColor(255, 255, 255), font);
		DrawStringToHandle(x + 100, y + 30, item.text.c_str(), GetColor(200, 200, 200), font);
		y += spacing_y;
	}
}


void HelpScene::DrawTeleport(int x, int y)
{
	Vector2D center = { static_cast<float>(x), static_cast<float>(y) };
	float radius_x = 30.0f;
	float radius_y = 40.0f;

	// �{��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawEllipseAA(center.x + 20, center.y + 20, radius_x, radius_y, 64, GetColor(100, 200, 255), TRUE, 2);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �O�������O
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	for (int i = 0; i < 3; ++i)
	{
		float ring_rx = radius_x + i * 2.0f;
		float ring_ry = radius_y + i * 2.0f;
		DrawEllipseAA(center.x + 20, center.y + 20, ring_rx, ring_ry, 52, GetColor(0, 255, 255), FALSE, 2);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void HelpScene::DrawEllipseAA(float cx, float cy, float rx, float ry, int num_segments, int color, bool fill, int line_thickness)
{
	float angle_step = 2.0f * DX_PI_F / num_segments;

	if (fill)
	{
		// ������h��ꍇ�͑��p�`�Ƃ��ēh��
		for (int i = 0; i < num_segments; ++i)
		{
			float theta1 = angle_step * i;
			float theta2 = angle_step * (i + 1);

			float x1 = cx + rx * cosf(theta1);
			float y1 = cy + ry * sinf(theta1);

			float x2 = cx + rx * cosf(theta2);
			float y2 = cy + ry * sinf(theta2);

			DrawTriangle(cx, cy, x1, y1, x2, y2, color, TRUE);
		}
	}
	else
	{
		// �������`���i�ւ����j
		float prev_x = cx + rx * cosf(0);
		float prev_y = cy + ry * sinf(0);

		for (int i = 1; i <= num_segments; ++i)
		{
			float theta = angle_step * i;

			float x = cx + rx * cosf(theta);
			float y = cy + ry * sinf(theta);

			DrawLine(prev_x, prev_y, x, y, color, line_thickness);

			prev_x = x;
			prev_y = y;
		}
	}
}



