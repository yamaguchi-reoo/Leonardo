#pragma once
#include <string>
#include <vector>

struct RankingEntry
{
	std::string name;  // �v���C���[��
	int score;        // �X�R�A
};
class RankingManager
{
public:
	void Load();
	void Save();
	void AddEntry(const std::string& name, int score);
	const std::vector<RankingEntry>& GetRankings() const;

	static RankingManager* GetInstance();

private:
	std::vector<RankingEntry> rankings;  // �����L���O�G���g���̃��X�g
	const std::string file_path = "Resource/file/ranking.txt";  // �����L���O�t�@�C���̃p�X
};

