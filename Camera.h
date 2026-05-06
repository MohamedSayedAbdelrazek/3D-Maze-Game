#pragma once
/*
 * Camera.h - First-Person Camera (merged header + implementation)
 * Uses gluLookAt for view transformation.
 */

#include "Game.h"
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

struct Camera {
    float posX, posY, posZ;
    float yaw, pitch;
    float forwardX, forwardZ;
    float rightX, rightZ;

    void init(float startX, float startY, float startZ, float startYaw) {
        posX = startX; posY = startY; posZ = startZ;
        yaw = startYaw; pitch = 0.0f;
        updateVectors();
    }

    /* Recalculate forward/right vectors from yaw */
    void updateVectors() {
        float yawRad = yaw * GAME_PI / 180.0f;
        forwardX = sinf(yawRad);
        forwardZ = -cosf(yawRad);
        rightX = cosf(yawRad);
        rightZ = sinf(yawRad);
    }

    /* Apply camera view using gluLookAt */
    void applyView() {
        float yawRad = yaw * GAME_PI / 180.0f;
        float pitchRad = pitch * GAME_PI / 180.0f;
        float lookX = posX + cosf(pitchRad) * sinf(yawRad);
        float lookY = posY + sinf(pitchRad);
        float lookZ = posZ - cosf(pitchRad) * cosf(yawRad);
        gluLookAt(posX, posY, posZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);
    }

    void moveForward(float speed)  { posX += forwardX * speed; posZ += forwardZ * speed; }
    void moveBackward(float speed) { posX -= forwardX * speed; posZ -= forwardZ * speed; }
    void strafeLeft(float speed)   { posX -= rightX * speed;   posZ -= rightZ * speed; }
    void strafeRight(float speed)  { posX += rightX * speed;   posZ += rightZ * speed; }

    void rotateYaw(float angleDeg) {
        yaw += angleDeg;
        if (yaw >= 360.0f) yaw -= 360.0f;
        if (yaw < 0.0f) yaw += 360.0f;
        updateVectors();
    }

    void rotatePitch(float angleDeg) {
        pitch += angleDeg;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }
};

/* Global camera instance */
Camera g_camera;
