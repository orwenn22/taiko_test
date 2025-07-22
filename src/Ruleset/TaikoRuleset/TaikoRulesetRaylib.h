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
    void StartAudio(float offset) override;
    void OnGameEnd() override;
    void HandleInput(const RulesetInputMessage &message) override;
    void Update(float dt) override;
    void Draw() override;

private:
    void DrawLeftPart();

    //All the methods below allow to draw the objects.
    //In all of these, current_time refer to the current seek position in the map
    void DrawBarLines(int current_time);

    void DrawHits(int current_time);
    bool DrawHit(TaikoHit *hit, int current_time);

    void DrawDrumRolls(int current_time);
    bool DrawDrumRoll(TaikoDrumRoll *drum_roll, int current_time);

    void SetPlayfieldY(float y);
    void SetPlayfieldWidth(float width);
    void SetPlayfieldHeight(float height);
    void UpdatePlayfieldGeometry(float y_offset = 0.f);

    Camera3D m_camera;

    Music m_audio;

    Texture m_receptor_texture;
    Texture m_small_hit_texture;
    Texture m_small_hit_overlay_texture;
    Texture m_drum_roll_tail_texture;
    Texture m_drum_roll_middle_texture;
    Texture m_playfield_left_texture;
    Texture m_playfield_left_keys_textures[4];

    Shader m_shader;

    //These are updated every frame by Update()
    float m_playfield_width;

    //This is meant to be customizable
    float m_playfield_height;

    //These are calculated based on the playfield's width and height
    float m_playfield_origin_y;
    float m_playfield_center_y;
    float m_object_travel_distance; //Travel distance of the notes in pixel (from the point where they are not visible on screen to the receptor's position)
    float m_receptor_center_x; //The objects are supposed to be hit when they reach this point

    //This is used by the left part of the playfield when displayed the pressed keys
    float m_keys_opacity[4];
};



#endif //TAIKORULESETRAYLIB_H
