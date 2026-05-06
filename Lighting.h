#pragma once
/*
 * Lighting.h - OpenGL Lighting (merged header + implementation)
 */

#include "Game.h"
#include <Windows.h>
#include <GL/gl.h>

/* Set up GL_LIGHT0 with ambient, diffuse, specular components */
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    GLfloat ambientLight[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat diffuseLight[]  = { 0.8f, 0.8f, 0.75f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { MAZE_SIZE / 2.0f, WALL_HEIGHT * 5.0f, MAZE_SIZE / 2.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

/* Toggle lighting on/off */
void toggleLighting() {
    g_game.lightingEnabled = !g_game.lightingEnabled;
    if (g_game.lightingEnabled) glEnable(GL_LIGHTING);
    else glDisable(GL_LIGHTING);
}
