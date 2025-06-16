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

	LoadResource();

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

	// �A�j���[�V��������
	ObjectAnimation();

	return __super::Update();
}

void HelpScene::Draw()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	int title_font = rm->GetFontHandle("Tepid Terminal", 64);

	std::string help_text = "== HELP ==";
	int result_width = GetDrawStringWidthToHandle(help_text.c_str(), help_text.size(), title_font);
	int help_x = (SCREEN_WIDTH - result_width) / 2;
	DrawStringToHandle(help_x, 100, help_text.c_str(), GetColor(255, 255, 255), title_font);


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

	DrawObject();



}

void HelpScene::Finalize()
{
}

eSceneType HelpScene::GetNowSceneType() const
{
	return eSceneType::HELP;
}

void HelpScene::ObjectAnimation()
{
	for (auto& pair : animation_data)
	{
		PlayerAnimationType type = pair.first;
		auto& frames = pair.second;

		animation_timer[type]++;
		if (animation_timer[type] >= 10)
		{
			if (!frames.empty())
			{
				animation_frame[type] = (animation_frame[type] + 1) % frames.size();
			}
			animation_timer[type] = 0;
		}
	}
}


void HelpScene::LoadResource()
{
	ResourceManager* rm = ResourceManager::GetInstance();

	auto idle_imgs = rm->GetImages("Resource/Images/Character/Player/Player-idle/player-idle", 6);
	auto walk_imgs = rm->GetImages("Resource/Images/Character/Player/Player-run/player-run", 6);
	auto jump_imgs = rm->GetImages("Resource/Images/Character/Player/Player-jump/player-jump", 2);

	animation_data[PlayerAnimationType::IDLE] = idle_imgs;
	animation_data[PlayerAnimationType::WALK] = walk_imgs;
	animation_data[PlayerAnimationType::JUMP] = jump_imgs;

	

	// ������
	for (auto& pair : animation_data) {
		animation_frame[pair.first] = 0;
		animation_timer[pair.first] = 0;
	}
}

void HelpScene::DrawObject()
{
	// �\���ʒu
	const int spacing_x = 200;

	std::map<PlayerAnimationType, std::string> label = {
		{PlayerAnimationType::IDLE, "Idle"},
		{PlayerAnimationType::WALK, "Run"},
		{PlayerAnimationType::JUMP, "Jump"},
	};

	int i = 0;
	for (auto& pair : animation_data)
	{
		PlayerAnimationType type = pair.first;
		const auto& frames = pair.second;
		if (!frames.empty())
		{
			int frame_index = animation_frame[type];
			int handle = frames[frame_index];
;
			int x = 435 + spacing_x * i;
			int y = 300;

			// �摜�̕��E�������擾
			int w, h;
			GetGraphSize(handle, &w, &h);

			float scale = 2.0f;
			int padding = 20;

			// �w�iBox�T�C�Y
			int bg_w = w + padding * 2 + 70;
			int bg_h = h + padding * 2 + 70; // �� ������Ɖ��ɃX�y�[�X�𑝂₷�i���p�j

			// Box����E�E��
			int left = x - bg_w / 2;
			int top = y - bg_h / 2;
			int right = x + bg_w / 2;
			int bottom = y + bg_h / 2;

			//�w�i�{�b�N�X�i�S�́j
			DrawBox(left, top, right, bottom, GetColor(50, 50, 50), TRUE);

			//�����ۂ������`��Box�̉����ɕ`��
			int floor_height = 20;
			DrawBox(left, bottom - 37, right, bottom, GetColor(30, 10, 200), TRUE);

			//�L�����`��
			DrawRotaGraphF(x, y, scale, 0.0, handle, TRUE, 0);

			//���x��
			DrawString(x - 20, bottom + 5, label[type].c_str(), GetColor(255, 255, 255));
		}
		i++;
	}
}


