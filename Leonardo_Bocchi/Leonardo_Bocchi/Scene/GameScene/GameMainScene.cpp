#include "GameMainScene.h" 
#include "DxLib.h"
#include "../../Object/ObjectList.h"
#include "../../Utility/UtilityList.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>

namespace {
	constexpr int GAME_OVER_FADE_MAX = 200;
	constexpr int GAME_OVER_WAIT_FRAMES = 20;
	constexpr int TRAP_LIMIT = 5;
	constexpr int ITEM_LIMIT = 3;
	constexpr int GOAL_Y_THRESHOLD = 850;
	constexpr int CLONE_SPAWN_DELAY = 50;
	constexpr int STAGE_Y_BASE = 720;
	constexpr int BOX_SIZE_F = 48;
	constexpr int INVINCIBLE_LIMIT = 2;
	constexpr int FADE_SPEED = 5;
}

GameMainScene::GameMainScene()
	: stage_width_num(0), stage_height_num(0), stage_data{ 0 }, player(nullptr), back_ground_image(0),
	clone_spawn_timer(0.0f), is_create(false), is_game_over(false), is_decided(false), fade_alpha(0),
	font_48(0), font_24(0), trap_num(0), decision_se(-1), main_bgm(-1), game_over_se(-1), game_over_timer(0)
{
}

GameMainScene::~GameMainScene() {}

void GameMainScene::Initialize()
{
	LoadStage();
	camera_location = Vector2D(0.0f, 0.0f);
	back_ground_image = LoadGraph("Resource/Images/BackGround/Base_Color.png");
	LoadGameMainResource();
	PlaySoundBgm(main_bgm, 60);
	trap_num = 3;
}

eSceneType GameMainScene::Update()
{
	if (IsStageReload() && goal_point && !goal_point->IsActive()) {
		StageClear();
		ReLoadStage();
	}

	UpdateCamera();

	if (!player) FindPlayer();

	if (!is_create && ++clone_spawn_timer >= CLONE_SPAWN_DELAY) {
		CreateClone();
		clone_spawn_timer = 0;
		is_create = true;
	}

	if (!is_game_over && (player->GetHp() <= 0 || player->GetLocation().y > GOAL_Y_THRESHOLD)) {
		player->SetDelete();
		is_game_over = true;
		fade_alpha = 0;
		PlaySoundSe(game_over_se, 100);
	}

	if (is_game_over) {
		fade_alpha = Min(fade_alpha + FADE_SPEED, GAME_OVER_FADE_MAX);

		if (InputControl::GetInstance()->GetButtonDown(XINPUT_BUTTON_A)) {
			PlaySoundSe(decision_se, 80);
			is_decided = true;
			game_over_timer = 0;
		}
	}

	if (is_decided && ++game_over_timer >= GAME_OVER_WAIT_FRAMES) {
		is_game_over = false;
		return eSceneType::RESULT;
	}

	return __super::Update();
}

void GameMainScene::Draw()
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(10, 10, 30), TRUE);
	__super::Draw();

	if (InputControl::GetInstance()->GetKey(KEY_INPUT_0)) {
		for (int i = 0; i < stage_height_num; ++i)
			for (int j = 0; j < stage_width_num; ++j)
				DrawFormatString(j * BOX_SIZE, i * BOX_SIZE, GetColor(255, 255, 255), "%d", stage_data[i][j]);
	}

	DrawUI();

	if (is_game_over) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		const char* msg = "GAME OVER";
		int text_width = GetDrawStringWidthToHandle(msg, strlen(msg), font_48);
		DrawStringToHandle((SCREEN_WIDTH - text_width) / 2, SCREEN_HEIGHT / 2 - 50, msg, GetColor(255, 0, 0), font_48);

		const char* hint = "Press A to continue";
		int hint_width = GetDrawStringWidthToHandle(hint, strlen(hint), font_24);
		DrawStringToHandle((SCREEN_WIDTH - hint_width) / 2, SCREEN_HEIGHT / 2 + 90, hint, GetColor(255, 255, 255), font_24);
	}

	DrawFormatString(10, 200, GetColor(255, 255, 255), "%d", trap_num);
}

void GameMainScene::Finalize()
{
	DeleteGraph(back_ground_image);
	back_ground_image = 0;

	StopGameMainSound();
	ResourceManager::GetInstance()->UnloadResourcesAll();
	ResourceManager::DeleteInstance();
}

eSceneType GameMainScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}


void GameMainScene::LoadStage()
{
	std::ifstream file("Resource/file/stage.csv");

	if (!file) {
		std::cerr << "�t�@�C�����J���܂���ł���: " << std::endl;
		return;
	}
	// 1�s�ڂ�ǂݍ���ŃX�e�[�W���ƍ������擾
	std::string line;

	//file����1�s�ǂݍ���ŁAline �Ɋi�[�B
	if (std::getline(file, line)) {
		//���������͂��邽�߂̃X�g���[�����쐬�B
		std::stringstream ss(line);
		std::string width, height;

		// �J���}�ŕ������ĕ��ƍ������擾
		std::getline(ss, width, ',');
		std::getline(ss, height, ',');

		//������𐮐��ɕϊ�
		stage_width_num = std::stoi(width);   // �X�e�[�W��
		stage_height_num = std::stoi(height); // �X�e�[�W����
	}

	//�X�e�[�W�f�[�^�̓ǂݍ��݁iCSV��2�s�ڈȍ~�j
	for (int i = 0; i < stage_height_num; i++) {
		//1�s���ǂݍ���
		if (std::getline(file, line)) {
			std::stringstream ss(line);
			for (int j = 0; j < stage_width_num; j++) {
				//�J���}��؂�Ńf�[�^���擾
				std::string value;
				if (std::getline(ss, value, ',')) {
					//������𐮐��ɕϊ����ăX�e�[�W�f�[�^�Ɋi�[
					stage_data[i][j] = std::stoi(value);
				}
			}
		}
	}

	file.close();

	SetStage();
}

void GameMainScene::SetStage()
{
	const Vector2D block_size((float)BOX_SIZE);

	for (int y = 0; y < stage_height_num; ++y) {
		for (int x = 0; x < stage_width_num; ++x) {
			Vector2D world_pos(x * BOX_SIZE, 720 - ((stage_height_num - y) * BOX_SIZE));
			int tile = stage_data[y][x];

			switch (tile) {
				case EMPTY: break;
				case BLOCK:
					CreateObject<Block>(world_pos, block_size);
					break;
				case PLAYER:
					CreateObject<Player>(world_pos, Vector2D(48.0f, 64.0f));
					break;
				case MOVE_BLOCK:
					CreateObject<MoveBlock>(world_pos, Vector2D(BOX_SIZE, 24.0f));
					break;
				case GOAL:
					CreateObject<GoalPoint>(world_pos, Vector2D(BOX_SIZE / 2.0f, BOX_SIZE / 1.5f));
					goal_pos = world_pos;
					break;
			}
		}
	}

	CreateItem();
	CreateGimmick();
}


void GameMainScene::UpdateCamera()
{
	//�v���C���[�����݂���Ȃ�J������Ǐ]������
	if (player)
	{
		float screen_half_width = SCREEN_WIDTH / 2;				//��ʂ̔����̕�
		float stage_limit_left = 0.0f;							//�X�e�[�W�̍��[
		float stage_limit_right = stage_width_num * BOX_SIZE - SCREEN_WIDTH; //�X�e�[�W�̉E�[ 

		//�J�����ʒu �� �v���C���[�̈ʒu - ��ʂ̔����̕� 
		camera_location.x = player->GetLocation().x - screen_half_width;

		//��ʒ[�ł̓X�N���[�����Ȃ��悤����
		if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
		if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
	}
}

void GameMainScene::StageClear()
{
	clear_count++;
	//�v���C���[���擾
	Player* p = static_cast<Player*>(player);

	if (p){
		// �v���C���[�̈ړ�������ۑ�
		stage_clear_history.push_back(p->GetMoveHistory());
	}
	trap_num += 1;
}

void GameMainScene::ReLoadStage()
{
	//�I�u�W�F�N�g�̍폜
	for (auto obj : objects)
	{
		delete obj;
		obj = nullptr;
	}
	player = nullptr;
	objects.clear();
	stage_reload = false;
	is_create = false;
	LoadStage();

}

void GameMainScene::FindPlayer()
{
	//�v���C���[�I�u�W�F�N�g��T���Ď擾
	for (auto obj : objects)
	{
		if (obj->GetObjectType() == PLAYER)
		{
			player = obj;
			break;
		}
	}
}

void GameMainScene::CreateClone()
{
	//�X�e�[�W�N���A���̃v���C���[�̗�������ɃG�l�~�[�𐶐�
	for (const auto& history : stage_clear_history)
	{
		//�V�����G�l�~�[�i�ߋ��̃v���C���[�j�𐶐�
		Enemy* enemy = CreateObject<Enemy>(Vector2D(0.0f, 0.0f) , Vector2D(48.0f, 64.0f));

		// �������G�l�~�[�ɃZ�b�g
		enemy->SetReplayHistory(history);
	}
}

void GameMainScene::CreateItem()
{
	std::vector<Vector2D> item_positions;

	// �A�C�e����u�����ʒu��T���i��{���̉����u���b�N�j
	for (int i = 0; i < stage_height_num - 1; ++i) {
		for (int j = 0; j < stage_width_num; ++j) {
			if (stage_data[i][j] != EMPTY) continue;
			if (stage_data[i + 1][j] != BLOCK) continue;

			int x = j * BOX_SIZE;
			int y = 720 - ((stage_height_num - i) * BOX_SIZE);

			// �v���C���[�����������Ȃ��ʒu���S�[���O
			if (x > (SCREEN_WIDTH - 280) && x < goal_pos.x) {
				item_positions.emplace_back(x, y);
			}
		}
	}

	// �����V���b�t��
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(item_positions.begin(), item_positions.end(), gen);

	// �ő�3�܂ŃA�C�e���𐶐�
	int item_count = Min(3, static_cast<int>(item_positions.size()));
	for (int i = 0; i < item_count; ++i) {
		const Vector2D& pos = item_positions[i];

		int grid_x = pos.x / BOX_SIZE;
		int grid_y = stage_height_num - ((720 - pos.y) / BOX_SIZE);

		if (i % 2 == 0) {
			CreateObject<HealItem>(pos, Vector2D((float)BOX_SIZE));
			stage_data[grid_y][grid_x] = HEAL;
		}
		else {
			CreateObject<InvincibleItem>(pos, Vector2D((float)BOX_SIZE));
			stage_data[grid_y][grid_x] = INVINCIBLE;
		}
	}
}


void GameMainScene::CreateGimmick()
{
	std::vector<Vector2D> trap_positions;

	if (trap_num >= 7)
	{
		trap_num = 7;
	}

	// �g���b�v��u�����ʒu��T���i��{���̉����u���b�N�j
	for (int i = 0; i < stage_height_num - 1; ++i) {
		for (int j = 0; j < stage_width_num; ++j) {
			if (stage_data[i][j] != EMPTY) continue;
			if (stage_data[i + 1][j] != BLOCK) continue;

			// ���̃}�X�ɃA�C�e��������΃X�L�b�v�i���Ԃ�h�~�j
			if (stage_data[i][j] == HEAL || stage_data[i][j] == INVINCIBLE) continue;

			int x = j * BOX_SIZE;
			int y = 720 - ((stage_height_num - i) * BOX_SIZE); // �� CreateItem�Ɠ���

			// �S�[���O�̉�ʉE�[�G���A
			if (x > (SCREEN_WIDTH - 280) && x < goal_pos.x) {
				trap_positions.emplace_back(x, y);
			}
		}
	}

	// �����V���b�t��
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(trap_positions.begin(), trap_positions.end(), gen);

	// �ő�5�܂Ńg���b�v�𐶐�
	const int trap_count = Min(trap_num, static_cast<int>(trap_positions.size()));
	for (int i = 0; i < trap_count; ++i) {
		const Vector2D& pos = trap_positions[i];

		int grid_x = pos.x / BOX_SIZE;
		int grid_y = stage_height_num - ((720 - pos.y) / BOX_SIZE);

		CreateObject<Trap>(pos, Vector2D((float)BOX_SIZE));
		stage_data[grid_y][grid_x] = TRAP;
	}
}

void GameMainScene::LoadGameMainResource()
{
	ResourceManager* rm = ResourceManager::GetInstance();

	//�T�E���h�Ǎ�
	//rm->GetSound("Resource/Sounds/BGM/AS_259735_�X�g�C�b�N�ȃT�C�o�[��4�ł�.mp3");
	sounds_data = rm->GetSound("Resource/Sounds/BGM/AS_259735_�X�g�C�b�N�ȃT�C�o�[��4�ł�.mp3");
	main_bgm = sounds_data[0];

	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1296213_�T�C�o�[�Ȋ����̌��艹.mp3");
	decision_se = sounds_data[0];

	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1593387_��������A�_���Ȋ����̉�.mp3");
	game_over_se = sounds_data[0];

	//�t�H���g�Ǎ�
	rm->LoadFont("Resource/Font/TepidTerminal.ttf", "Tepid Terminal");
	font_48 = rm->GetFontHandle("Tepid Terminal", 48);
	font_24 = rm->GetFontHandle("Tepid Terminal", 24);

	//UI�摜�Ǎ�
	heart_img = LoadGraph("Resource/Images/UI/Hp.png"); // �n�[�g�摜��ǂݍ���
}


void GameMainScene::PlaySoundBgm(int _handle, int volume)
{
	ChangeVolumeSoundMem(volume, _handle);
	PlaySoundMem(_handle, DX_PLAYTYPE_LOOP); // BGM�����[�v�Đ�
}

void GameMainScene::PlaySoundSe(int _handle, int volume)
{
	ChangeVolumeSoundMem(volume, _handle);
	PlaySoundMem(_handle, DX_PLAYTYPE_BACK); // SE��1��̂ݍĐ�
}

void GameMainScene::StopGameMainSound()
{
	if (!sounds_data.empty()) {
		StopSoundMem(sounds_data[0]);
	}
}

void GameMainScene::DrawUI() const
{
	if (player) 
	{
		// �n�[�g�`��
		for (int i = 0; i < player->GetHp(); ++i)
		{
			DrawGraph(20 + i * 50, 20, heart_img, TRUE);
		}

	}

	// LOOP �e�L�X�g����
	std::string loop_text = "LOOP : " + std::to_string(clear_count);
	int text_width = GetDrawStringWidthToHandle(loop_text.c_str(), -1, font_48);
	int text_height = GetFontSizeToHandle(font_48); // �������擾

	// �\���ʒu�i�E��j
	int padding = 10;
	int box_x = SCREEN_WIDTH - text_width - padding * 2 - 50;
	int box_y = 20;
	int box_w = text_width + padding * 2;
	int box_h = text_height + padding * 2;

	// �w�i�`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(box_x, box_y, box_x + box_w, box_y + box_h, GetColor(30, 30, 60), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �g���i���j
	DrawBox(box_x, box_y, box_x + box_w, box_y + box_h, GetColor(255, 255, 255), FALSE);

	// �e�L�X�g�`��
	DrawStringToHandle(box_x + padding, box_y + padding, loop_text.c_str(), GetColor(255, 255, 455), font_48);

}

