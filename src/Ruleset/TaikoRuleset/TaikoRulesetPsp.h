#ifndef TAIKORULESETPSP_H
#define TAIKORULESETPSP_H

#include "TaikoRuleset.h"

struct Texture;

class TaikoRulesetPsp : public TaikoRuleset {
public:
    TaikoRulesetPsp();
    ~TaikoRulesetPsp() override;

    void LoadResources() override;
    void StartAudio(float offset) override;
    void OnGameEnd() override;
    void HandleInput(const RulesetInputMessage &message) override;
    void Update(float dt) override;
    void Draw() override;


private:
    void DrawPlayfield();
    void DrawLeftPart();

    Texture *m_taiko_sheet;

    float m_keys_opacity[4];
};



#endif //TAIKORULESETPSP_H
