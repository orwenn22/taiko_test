#ifndef TAIKORULESETPSP_H
#define TAIKORULESETPSP_H

#include "TaikoRuleset.h"

#include "../../Platform/Psp/Graphics/Vertex.h"
#include "../../Platform/Psp/Audio/AudioStream.h"

struct TaikoHit;
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
    void DrawReceptor();

    void DrawBarlines(int current_time);
    bool GetBarlineVertices(int current_time, TaikoEffectPoint *effect_point, Vertex_Color32_XYZ32 *vertices_out);

    void DrawHits(int current_time);
    bool GetHitVertices(int current_time, TaikoHit *taiko_hit, Vertex_UV32_Color32_XYZ32 *vertices_out, uint8_t *indices_out);

    Texture *m_taiko_sheet;
    AudioStream *m_audio;

    float m_keys_opacity[4];
};



#endif //TAIKORULESETPSP_H
