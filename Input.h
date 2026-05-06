#pragma once
/*
 * Input.h - Keyboard and Mouse Input (merged header + implementation)
 */

#include "Game.h"
#include "Camera.h"
#include "Collision.h"
#include "Lighting.h"
#include <Windows.h>
#include <GL/glut.h>
#include <cstdio>

/* Internal state for mouse warping */
static bool g_mouseWarping = false;
static bool g_firstMouse = true;

/*
 * Try to move the player with wall-sliding collision.
 * Attempts full move, then X-only, then Z-only.
 */
static void tryMovePlayer(float deltaX, float deltaZ) {
    if (g_game.gameWon) return;
    float newX = g_camera.posX + deltaX;
    float newZ = g_camera.posZ + deltaZ;

    if (!g_game.collisionEnabled) {
        g_camera.posX = newX; g_camera.posZ = newZ; return;
    }
    if (!checkWallCollision(newX, newZ)) {
        g_camera.posX = newX; g_camera.posZ = newZ; return;
    }
    if (!checkWallCollision(newX, g_camera.posZ)) {
        g_camera.posX = newX; return;
    }
    if (!checkWallCollision(g_camera.posX, newZ)) {
        g_camera.posZ = newZ; return;
    }
}

/* Handle regular keyboard keys */
void handleKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': case 'W':
        tryMovePlayer(g_camera.forwardX*MOVE_SPEED, g_camera.forwardZ*MOVE_SPEED); break;
    case 's': case 'S':
        tryMovePlayer(-g_camera.forwardX*MOVE_SPEED, -g_camera.forwardZ*MOVE_SPEED); break;
    case 'a': case 'A':
        tryMovePlayer(-g_camera.rightX*MOVE_SPEED, -g_camera.rightZ*MOVE_SPEED); break;
    case 'd': case 'D':
        tryMovePlayer(g_camera.rightX*MOVE_SPEED, g_camera.rightZ*MOVE_SPEED); break;
    case ' ':
        fireProjectile(g_camera.posX, g_camera.posY, g_camera.posZ,
                        g_camera.forwardX, g_camera.forwardZ);
        break;
    case 'q': case 'Q':
        g_game.wireframeMode = !g_game.wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, g_game.wireframeMode ? GL_LINE : GL_FILL);
        printf("[Input] Wireframe %s\n", g_game.wireframeMode?"ON":"OFF"); break;
    case 'c': case 'C':
        g_game.collisionEnabled = !g_game.collisionEnabled;
        printf("[Input] Collision %s\n", g_game.collisionEnabled?"ON":"OFF"); break;
    case 'l': case 'L':
        toggleLighting();
        printf("[Input] Lighting %s\n", g_game.lightingEnabled?"ON":"OFF"); break;
    case 't': case 'T':
        g_game.texturesEnabled = !g_game.texturesEnabled;
        if (g_game.texturesEnabled) glEnable(GL_TEXTURE_2D); else glDisable(GL_TEXTURE_2D);
        printf("[Input] Textures %s\n", g_game.texturesEnabled?"ON":"OFF"); break;
    case 27: printf("[Input] ESC - exiting\n"); exit(0); break;
    }
    glutPostRedisplay();
}

/* Handle arrow keys */
void handleSpecialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        tryMovePlayer(g_camera.forwardX*MOVE_SPEED, g_camera.forwardZ*MOVE_SPEED); break;
    case GLUT_KEY_DOWN:
        tryMovePlayer(-g_camera.forwardX*MOVE_SPEED, -g_camera.forwardZ*MOVE_SPEED); break;
    case GLUT_KEY_LEFT:
        tryMovePlayer(-g_camera.rightX*MOVE_SPEED, -g_camera.rightZ*MOVE_SPEED); break;
    case GLUT_KEY_RIGHT:
        tryMovePlayer(g_camera.rightX*MOVE_SPEED, g_camera.rightZ*MOVE_SPEED); break;
    }
    glutPostRedisplay();
}

/* Handle mouse motion for FPS camera look */
void handleMouseMotion(int x, int y) {
    int cx = g_game.windowWidth/2, cy = g_game.windowHeight/2;
    if (g_firstMouse) { g_firstMouse=false; glutWarpPointer(cx,cy); return; }
    if (g_mouseWarping) { g_mouseWarping=false; return; }
    float dx=(float)(x-cx), dy=(float)(y-cy);
    if (dx!=0||dy!=0) {
        g_camera.rotateYaw(dx*MOUSE_SENSITIVITY);
        g_camera.rotatePitch(-dy*MOUSE_SENSITIVITY);
        g_mouseWarping=true;
        glutWarpPointer(cx,cy);
    }
}
