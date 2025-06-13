#pragma once
#include <string>
#include <vector>

struct RankingEntry
{
	std::string name;  // プレイヤー名
	int score;        // スコア
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
	std::vector<RankingEntry> rankings;  // ランキングエントリのリスト
	const std::string file_path = "Resource/file/ranking.txt";  // ランキングファイルのパス
};

