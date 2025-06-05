#pragma once
#include <vector>
#include "../Object/GameObject.h"

enum class eSceneType
{
	TITLE,
	GAME_MAIN,
	RESULT
};

class SceneBase
{
protected:
	std::vector<GameObject*>objects;
	Vector2D camera_location;

	int clear_count = 0;	//�N���A�J�E���g
	bool stage_reload = false;

public:
	//�R���X�g���N�^
	SceneBase();
	//�f�X�g���N�^
	virtual ~SceneBase();

	virtual void Initialize();		//����������
	virtual eSceneType Update();	//�X�V����
	virtual void Draw() const;		//�`�揈��
	virtual void Finalize();		//�I��������

	//���݂̃V�[���^�C�v���擾����
	virtual eSceneType GetNowSceneType()const = 0;

protected:
	//�Q�[���I�u�W�F�N�g�𐶐�����e���v���[�g�֐�
	template <class T>
	T* CreateObject(const Vector2D& _location, const Vector2D& _box_size)
	{
		//�V�����C���X�^���X�𐶐�
		T* new_instance = new T();

		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		if (new_object == nullptr)
		{
			delete new_instance;
			throw("�Q�[���I�u�W�F�N�g�������ł��܂���ł���\n");
		}

		new_object->SetLocation(_location);

		new_object->Initialize(_location, _box_size);

		objects.push_back(new_object);

		//���������C���X�^���X��Ԃ�
		return new_instance;
	}

public:
	void DeleteObject(GameObject* obj);

public:
	bool IsStageReload() const { return stage_reload; }
	void SetStageReload(bool flg) { stage_reload = flg; }

	void SetClearCount(int count) { clear_count = count; }
	int GetClearCount()const { return clear_count; }
};



