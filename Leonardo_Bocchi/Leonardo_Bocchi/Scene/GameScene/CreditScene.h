#pragma once
#include "../SceneBase.h"
#include <vector>
#include <string>


class CreditScene :
    public SceneBase
{
private:
    int scroll_pos;  // スクロール位置
    int credit_font;
    std::vector<std::string> credit_lines;
    int credit_se;
    bool is_skip;


public:

    CreditScene();
    ~CreditScene();

    void Initialize() override;
    eSceneType Update() override;
    void Draw() override;
    void Finalize() override;
    eSceneType GetNowSceneType() const override;

    void LoadCreditText();
};

