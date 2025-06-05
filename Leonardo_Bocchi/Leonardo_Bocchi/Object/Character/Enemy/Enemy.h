#pragma once
#include "../CharaBase.h"
#include <vector>

class Enemy :
    public CharaBase
{
private:
	//�G�l�~�[�̏��
    std::vector<MoveRecord> replay_history;
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
    void Draw(Vector2D offset, double rate) const override;
    //�I��������
    void Finalize()override;

public:
    
    //�v���C���[�̓���
    void Movement();
    //�A�j���[�V�����Ǘ�
    void AnimationControl();
    //�����������̋���
    void OnHitCollision(GameObject* hit_object)override;

	
	//�ړ��������Z�b�g
    void SetReplayHistory(const std::vector<MoveRecord>& history);
};

