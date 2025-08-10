#ifndef TAIKORULESETPSP_H
#define TAIKORULESETPSP_H

#include "../../../Base/Ruleset/TaikoRuleset/TaikoRuleset.h"

#include "../../Platform/Graphics/Vertex.h"
#include "../../Platform/Audio/AudioStream.h"

struct TaikoDrumRoll;
struct TaikoHit;
struct Texture;

class TaikoRulesetPsp : public TaikoRuleset {
public:
    TaikoRulesetPsp();
    static TaikoRulesetPsp *build();
    ~TaikoRulesetPsp() override;

    bool LoadResourcesInternal() override;
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

    //16 vertices and 24 indices per drum roll
    void DrawDrumRolls(int current_time);
    bool GetDrumRollVertices(int current_time, TaikoDrumRoll *drum_roll, Vertex_UV32_Color32_XYZ32 *vertices_out, uint8_t *indices_out);

    Texture *m_taiko_sheet;
    AudioStream *m_audio;

    float m_keys_opacity[4];
};



#endif //TAIKORULESETPSP_H
