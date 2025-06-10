#pragma once
#include "../GameObject.h"
#include <map>
#include <vector>

class Trap :
    public GameObject
{
private:
    std::vector<int> animation_data;
    int animation_frame = 0; //���݂̃A�j���[�V�����t���[��

	int trap_image = NULL;  //�g���b�v�̉摜

	bool is_active = false; // �g���b�v���A�N�e�B�u���ǂ���
	int effect_timer = 0;   // �G�t�F�N�g�̃^�C�}�[

public:
    //����������
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //�X�V����
    void Update()override;
    //�`�揈��
    void Draw(Vector2D offset, double rate) override;
    //�I��������
    void Finalize()override;

    //�����������̋���
    void OnHitCollision(GameObject* hit_object)override;

    void LoadTrapImage();
};

