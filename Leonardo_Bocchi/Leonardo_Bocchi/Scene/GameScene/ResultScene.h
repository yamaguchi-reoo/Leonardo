#pragma once
#include "../SceneBase.h"
class ResultScene :
	public SceneBase
{
public:
	ResultScene();
	~ResultScene();

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() const override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;
};

