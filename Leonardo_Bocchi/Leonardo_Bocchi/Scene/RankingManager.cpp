#include "RankingManager.h"
#include <DxLib.h>
#include <fstream>
#include <algorithm>

RankingManager* RankingManager::GetInstance()
{
	static RankingManager instance;
	return &instance;
}


void RankingManager::Load()
{
	rankings.clear();
	std::ifstream ifs(file_path);
	std::string name;
	int score;
	while (ifs >> name >> score) 
	{
		rankings.push_back({ name, score });
	}

	std::sort(rankings.begin(), rankings.end(), [](const RankingEntry& a, const RankingEntry& b) {
		return a.score > b.score;
		});
}

void RankingManager::Save()
{
	std::ofstream ofs(file_path);
	for (const auto& entry : rankings)
	{
		ofs << entry.name << " " << entry.score << "\n";
	}
}

void RankingManager::AddEntry(const std::string& name, int score)
{
	// 新しいエントリを追加
	rankings.push_back({ name, score });
	// スコアでソート
	std::sort(rankings.begin(), rankings.end(), [](const RankingEntry& a, const RankingEntry& b) {
		return a.score > b.score;
		});
	// 上位10件だけ保持
	if (rankings.size() > 10) {
		rankings.resize(10);
	}
}

const std::vector<RankingEntry>& RankingManager::GetRankings() const
{
	return rankings;

}


