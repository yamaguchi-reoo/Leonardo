#pragma once
#include "../SceneBase.h"
#include <string>

struct RankingEntry
{
	std::string name;  // �v���C���[��
	int score;        // �X�R�A
};

class RankingScene :
	public SceneBase
{
public:
	RankingScene();
	~RankingScene();

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() const override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;
};

