#pragma once
#include "../SceneBase.h"
#include <string>

class RankingScene :
	public SceneBase
{
private:
	std::vector<int> sounds_data;
	int decision_se;
public:
	RankingScene();
	~RankingScene();

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;

	void PlaySoundSe(int _handle, int volume);
};

