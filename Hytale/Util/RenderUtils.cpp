#include "Util.h"
#include "core.h"

Matrix4x4 Util::getViewProjMat() {
	return viewProjMat;
}

bool Util::WorldToScreen(Vector3 pos, Vector2& out) {
    Camera* camera = Util::getCamera();
    ValidPtrBool(camera);

    SceneRenderer* scene = Util::getGameInstance()->SceneRenderer;
    ValidPtrBool(scene);

    Vector3 camPos = camera->Position;
    Vector3 camRelativePos = pos - camPos;

    float clipX = scene->MPV.m[0][0] * camRelativePos.x +
        scene->MPV.m[1][0] * camRelativePos.y +
        scene->MPV.m[2][0] * camRelativePos.z +
        scene->MPV.m[3][0];

    float clipY = scene->MPV.m[0][1] * camRelativePos.x +
        scene->MPV.m[1][1] * camRelativePos.y +
        scene->MPV.m[2][1] * camRelativePos.z +
        scene->MPV.m[3][1];

    float clipZ = scene->MPV.m[0][2] * camRelativePos.x +
        scene->MPV.m[1][2] * camRelativePos.y +
        scene->MPV.m[2][2] * camRelativePos.z +
        scene->MPV.m[3][2];

    float clipW = scene->MPV.m[0][3] * camRelativePos.x +
        scene->MPV.m[1][3] * camRelativePos.y +
        scene->MPV.m[2][3] * camRelativePos.z +
        scene->MPV.m[3][3];

    if (clipW < 0.001f) return false;

    float ndcX = clipX / clipW;
    float ndcY = clipY / clipW;
    float ndcZ = clipZ / clipW;

    if (ndcZ < -1.0f || ndcZ > 1.0f) return false;

    out.x = (ndcX + 1.0f) * 0.5f * Util::app->Engine->Window->WindowWidth;
    out.y = (1.0f - (ndcY + 1.0f) * 0.5f) * Util::app->Engine->Window->WindowHeight;

    return true;
}