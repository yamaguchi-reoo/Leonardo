#pragma once
#include "../SceneBase.h"
#include <string>

class RankingScene :
	public SceneBase
{
public:
	RankingScene();
	~RankingScene();

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;
};

