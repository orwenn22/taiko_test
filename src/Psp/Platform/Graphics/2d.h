#ifndef INC_2D_H
#define INC_2D_H

#include "../Vector2.h"

class Font;

/**
 * Setup matrices for 2D drawing
 * NOTE: objects with a high Z coordinate will be closer to the camera in this mode
 * @param origin The point we want to look at (will be placed at the top left of the screen with an offset of 0)
 * @param offset displacement from origin (an offset of SCREEN_WIDTH/2, SCREEN_HEIGHT/2 will put the origin at the center of the screen)
 * @param zoom zoom
 */
void Begin2D(const Vector2<float> &origin, const Vector2<float> &offset = Vector2<float>(0.f, 0.f), float zoom = 1.f);

/**
 * Setup matrices for 2D drawing, but objects far away from the camera will appear smaller
 * (this is technically 3D but with only 2D elements, though there is nothing actually preventing rendering actual 3D elements in this mode)
 * NOTE: objects with a low Z coordinate will be closer to the camera in this mode
 * @param target The point we want to look at (will be placed at the center of the screen)
 */
void Begin2DPerspective(const Vector2<float> &target);
void End2D();

void DrawText(const char *text, Font *font, const Vector2<float> &pos, float z, float scale = 1.f, unsigned int color = 0xFFFFFFFF);

#endif //INC_2D_H
