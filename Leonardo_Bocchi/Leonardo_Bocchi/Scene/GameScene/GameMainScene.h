#pragma once
#include "../SceneBase.h"
#include "../../common.h"
#include "../../Object/Character/Player/Player.h"
#include "../../Object/Character/CharaBase.h"



class GameMainScene :
    public SceneBase
{
private:
	int stage_width_num;    //�X�e�[�W�̉��̃u���b�N��
	int stage_height_num;   //�X�e�[�W�̏c�̃u���b�N��

	int stage_data[STAGE_MAX_HEIGHT][STAGE_MAX_WIDTH];

	GameObject* player;	//�v���C���[�̃|�C���^
	Vector2D goal_pos;

	std::vector<std::vector<MoveRecord>>stage_clear_history; //�X�e�[�W�N���A����

	int back_ground_image; //�w�i�摜
	int back_ground_img[5];

	float clone_spawn_timer;
	bool is_create;

	bool is_game_over;         // �Q�[���I�[�o�[��Ԃ��ǂ���
	int fade_alpha;

public:
	GameMainScene();
	~GameMainScene();

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() const override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;

	//�X�e�[�W�f�[�^��ǂݍ���
	void LoadStage();

	//�X�e�[�W�f�[�^�𐶐�
	void SetStage();

	//�J�����̈ʒu���X�V
	void UpdateCamera();

	//�X�e�[�W�N���A����
	void StageClear();

	//�X�e�[�W�����[�h
	void ReLoadStage();

	//�v���C���[��T��
	void FindPlayer();

	//�G�l�~�[�𐶐�
	void CreateClone();

	//�A�C�e���̐���
	void CreateItem();

	//�M�~�b�N�̐���
	void CreateGimmick();

};

