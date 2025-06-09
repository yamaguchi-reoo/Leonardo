#pragma once
#include "../CharaBase.h"
#include <vector>
#include "../Player/Player.h"

#include <map>


class Enemy :
    public CharaBase
{
private:
	//�G�l�~�[�̏��
    std::vector<PlayerMoveRecord> replay_history;                    // ���݂̍Đ��C���f�b�N�X

    ActionState action_state = ActionState::IDLE;

    std::map<ActionState, std::vector<int>> animation_data;  // �A�j���[�V�����f�[�^
    int animation_frame = 0;


    int current_frame = 0;

    int frame_timer = 0;            // �o�߃t���[����
    int frame_duration = 1;         // ���t���[��������1�X�e�b�v�i�ނ�


    Vector2D start_position;
    Vector2D end_position;
public:
    Enemy();
    ~Enemy();

    //����������
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //�X�V����
    void Update()override;
    //�`�揈��
    void Draw(Vector2D offset, double rate)  override;
    //�I��������
    void Finalize()override;

public:
    
    //�v���C���[�̓���
    void Movement();
    //�A�j���[�V�����Ǘ�
    void AnimationControl(ActionState state);
    //�����������̋���
    void OnHitCollision(GameObject* hit_object)override;

	
	//�ړ��������Z�b�g
    void SetReplayHistory(const std::vector<PlayerMoveRecord>& history);

	void LoadEnemyImage();
};

