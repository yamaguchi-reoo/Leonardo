#pragma once
#include "../GameObject.h"
#include "GoalParticle.h"
#include <vector>

class GoalPoint :
    public GameObject
{
private:
	std::vector<GoalParticle> particles; // �S�[���p�[�e�B�N���̃��X�g

	bool is_active = false; // �S�[�����A�N�e�B�u���ǂ���
	int effect_timer = 0; // �G�t�F�N�g�̃^�C�}�[

public:
    //����������
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //�X�V����
    void Update()override;
    //�`�揈��
    void Draw(Vector2D offset, double rate) override;
    //�I��������
    void Finalize()override;

    void OnHitCollision(GameObject* hit_object)override;

    void CreateParticles();

    bool IsActive() const;

};

