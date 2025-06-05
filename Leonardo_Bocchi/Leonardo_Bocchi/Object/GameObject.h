#pragma once
#pragma once
#include "../Utility/Vector2D.h"
#include "ObjectTypeList.h"


class GameObject
{
protected:
	Vector2D location;	//�I�u�W�F�N�g�̈ʒu
	Vector2D box_size;	//�I�u�W�F�N�g�̃T�C�Y
	Vector2D hit_box;	//�����蔻��̃T�C�Y

	Vector2D velocity = { 0.0f };
	int image;			//�`�悷��摜�f�[�^
	int flip_flg;		//�`�悷�锽�]�t���O

	eObjectType object_type;		//�I�u�W�F�N�g�̎��

	int hp = 0;
	bool is_delete = false;

public:
	GameObject();
	~GameObject();

	//����������
	virtual void Initialize(Vector2D _location, Vector2D _box_size);
	//�X�V����
	virtual void Update();
	//�`�揈��
	virtual void Draw(Vector2D offset, double rate) const;
	//�I��������
	virtual void Finalize();

public:
	//���W��ݒ�
	void SetLocation(Vector2D _location);

	//���W���擾
	Vector2D GetLocation()const;

	//�{�b�N�X�T�C�Y���擾
	Vector2D GetBoxSize()const;

	//���x�擾
	Vector2D GetVelocity()const;

	//�I�u�W�F�N�g�^�C�v�擾	
	eObjectType GetObjectType();

	virtual void OnHitCollision(GameObject* hit_object);

	//�����蔻��
	bool CheckBoxCollision(GameObject* obj);

	//�폜�t���O���擾
	bool GetIsDelete() { return is_delete; }

	void SetDelete() { is_delete = true; }

	int GetHp() { return hp; }
};


