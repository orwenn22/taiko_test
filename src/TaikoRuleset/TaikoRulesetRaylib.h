#ifndef TAIKORULESETRAYLIB_H
#define TAIKORULESETRAYLIB_H

#include "TaikoRuleset.h"

#include <raylib.h>


struct TaikoDrumRoll;
struct TaikoHit;

class TaikoRulesetRaylib : public TaikoRuleset {
public:
    TaikoRulesetRaylib();
    ~TaikoRulesetRaylib() override;

    void LoadResources() override;
    void OnGameStart() override;
    void OnGameEnd() override;
    void Update(float dt) override;
    void Draw() override;

private:

    //All the methods below allow to draw the objects.
    //In all of these, current_time refer to the current seek position in the map

    void DrawHits(int current_time);
    bool DrawHit(TaikoHit *hit, int current_time);

    void DrawDrumRolls(int current_time);
    bool DrawDrumRoll(TaikoDrumRoll *drum_roll, int current_time);


    Camera3D m_camera;

    Music m_audio;

    Texture m_receptor_texture;
    Texture m_small_hit_texture;
    Texture m_small_hit_overlay_texture;
    Texture m_drum_roll_tail_texture;
    Texture m_drum_roll_middle_texture;

    Shader m_shader;

    float m_playfield_width;
    float m_object_travel_distance;
};



#endif //TAIKORULESETRAYLIB_H
