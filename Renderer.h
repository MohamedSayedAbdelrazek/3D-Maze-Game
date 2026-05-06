#pragma once
/*
 * Renderer.h - OpenGL Rendering (merged header + implementation)
 * All drawing: walls, floor, ceiling, collectibles, goal, projectile, HUD.
 */

#include "Game.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "Lighting.h"
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>
#include <cmath>

/* ==================== Texture IDs ==================== */
GLuint wallTextureID = 0;
GLuint floorTextureID = 0;
GLuint ceilingTextureID = 0;

/* ==================== Helpers ==================== */

/* Draw a textured box from (x,y,z) to (x+sx, y+sy, z+sz) */
static void drawTexturedCube(float x, float y, float z, float sx, float sy, float sz) {
    glBegin(GL_QUADS);
    /* Front (+Z) */
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(x,    y,    z+sz);
    glTexCoord2f(1,0); glVertex3f(x+sx, y,    z+sz);
    glTexCoord2f(1,1); glVertex3f(x+sx, y+sy, z+sz);
    glTexCoord2f(0,1); glVertex3f(x,    y+sy, z+sz);
    /* Back (-Z) */
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(x+sx, y,    z);
    glTexCoord2f(1,0); glVertex3f(x,    y,    z);
    glTexCoord2f(1,1); glVertex3f(x,    y+sy, z);
    glTexCoord2f(0,1); glVertex3f(x+sx, y+sy, z);
    /* Left (-X) */
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(x, y,    z);
    glTexCoord2f(1,0); glVertex3f(x, y,    z+sz);
    glTexCoord2f(1,1); glVertex3f(x, y+sy, z+sz);
    glTexCoord2f(0,1); glVertex3f(x, y+sy, z);
    /* Right (+X) */
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(x+sx, y,    z+sz);
    glTexCoord2f(1,0); glVertex3f(x+sx, y,    z);
    glTexCoord2f(1,1); glVertex3f(x+sx, y+sy, z);
    glTexCoord2f(0,1); glVertex3f(x+sx, y+sy, z+sz);
    /* Top (+Y) */
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(x,    y+sy, z+sz);
    glTexCoord2f(1,0); glVertex3f(x+sx, y+sy, z+sz);
    glTexCoord2f(1,1); glVertex3f(x+sx, y+sy, z);
    glTexCoord2f(0,1); glVertex3f(x,    y+sy, z);
    /* Bottom (-Y) */
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(x,    y, z);
    glTexCoord2f(1,0); glVertex3f(x+sx, y, z);
    glTexCoord2f(1,1); glVertex3f(x+sx, y, z+sz);
    glTexCoord2f(0,1); glVertex3f(x,    y, z+sz);
    glEnd();
}

static void drawBitmapString(const char* text, void* font) {
    while (*text) { glutBitmapCharacter(font, *text); text++; }
}

/* ==================== Drawing Functions ==================== */

static void drawFloor() {
    if (g_game.texturesEnabled && floorTextureID != 0) {
        glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, floorTextureID);
    }
    glColor3f(0.4f, 0.45f, 0.35f);
    for (int r = 0; r < MAZE_SIZE; r++)
        for (int c = 0; c < MAZE_SIZE; c++)
            if (g_game.maze[r][c] != 1) {
                float x=(float)c, z=(float)r;
                glBegin(GL_QUADS);
                glNormal3f(0,1,0);
                glTexCoord2f(0,0); glVertex3f(x,          0, z);
                glTexCoord2f(1,0); glVertex3f(x+CELL_SIZE, 0, z);
                glTexCoord2f(1,1); glVertex3f(x+CELL_SIZE, 0, z+CELL_SIZE);
                glTexCoord2f(0,1); glVertex3f(x,          0, z+CELL_SIZE);
                glEnd();
            }
    if (g_game.texturesEnabled && floorTextureID != 0) glDisable(GL_TEXTURE_2D);
}

static void drawCeiling() {
    if (g_game.texturesEnabled && ceilingTextureID != 0) {
        glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, ceilingTextureID);
    }
    glColor3f(0.3f, 0.3f, 0.4f);
    for (int r = 0; r < MAZE_SIZE; r++)
        for (int c = 0; c < MAZE_SIZE; c++)
            if (g_game.maze[r][c] != 1) {
                float x=(float)c, z=(float)r;
                glBegin(GL_QUADS);
                glNormal3f(0,-1,0);
                glTexCoord2f(0,0); glVertex3f(x,          WALL_HEIGHT, z+CELL_SIZE);
                glTexCoord2f(1,0); glVertex3f(x+CELL_SIZE, WALL_HEIGHT, z+CELL_SIZE);
                glTexCoord2f(1,1); glVertex3f(x+CELL_SIZE, WALL_HEIGHT, z);
                glTexCoord2f(0,1); glVertex3f(x,          WALL_HEIGHT, z);
                glEnd();
            }
    if (g_game.texturesEnabled && ceilingTextureID != 0) glDisable(GL_TEXTURE_2D);
}

static void drawMaze() {
    if (g_game.texturesEnabled && wallTextureID != 0) {
        glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, wallTextureID);
    }
    glColor3f(0.6f, 0.45f, 0.3f);
    for (int r = 0; r < MAZE_SIZE; r++)
        for (int c = 0; c < MAZE_SIZE; c++)
            if (g_game.maze[r][c] == 1)
                drawTexturedCube((float)c, 0.0f, (float)r, CELL_SIZE, WALL_HEIGHT, CELL_SIZE);
    if (g_game.texturesEnabled && wallTextureID != 0) glDisable(GL_TEXTURE_2D);
}

static void drawCollectibles() {
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.9f, 0.1f, 0.1f);
    float cs = 0.25f;
    for (int r = 0; r < MAZE_SIZE; r++)
        for (int c = 0; c < MAZE_SIZE; c++)
            if (g_game.maze[r][c] == 2) {
                glPushMatrix();
                glTranslatef(c+0.5f, WALL_HEIGHT*0.5f, r+0.5f);
                glRotatef(g_game.collectibleRotation, 0,1,0);
                glRotatef(30.0f, 1,0,0);
                drawTexturedCube(-cs/2,-cs/2,-cs/2, cs,cs,cs);
                glPopMatrix();
            }
}

static void drawGoal() {
    glDisable(GL_TEXTURE_2D);
    float cs = 0.2f;
    if (g_game.lightingEnabled) {
        GLfloat em[] = {0.5f,0.4f,0.0f,1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, em);
    }
    glColor3f(1.0f, 0.84f, 0.0f);
    glPushMatrix();
    glTranslatef(GOAL_COL+0.5f, WALL_HEIGHT*0.5f, GOAL_ROW+0.5f);
    float s = g_game.goalPulseScale;
    glScalef(s,s,s);
    glRotatef(g_game.collectibleRotation*0.5f, 0,1,0);
    glRotatef(g_game.collectibleRotation*0.3f, 1,0,0);
    drawTexturedCube(-cs/2,-cs/2,-cs/2, cs,cs,cs);
    glPopMatrix();
    if (g_game.lightingEnabled) {
        GLfloat no[] = {0,0,0,1}; glMaterialfv(GL_FRONT, GL_EMISSION, no);
    }
}

static void drawProjectile() {
    if (!g_game.projectile.active) return;
    glDisable(GL_TEXTURE_2D);
    float px=g_game.projectile.posX, py=g_game.projectile.posY, pz=g_game.projectile.posZ;
    float cs=0.1f;
    /* Trailing effect */
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i=3;i>=1;i--) {
        float tx=px-g_game.projectile.dirX*0.15f*i, tz=pz-g_game.projectile.dirZ*0.15f*i;
        float a=0.3f/(float)i, ts=cs*(1.0f-i*0.15f);
        glColor4f(0,0.8f,1,a);
        glPushMatrix(); glTranslatef(tx,py,tz);
        glRotatef(g_game.projectile.rotationAngle*0.5f,1,1,0);
        drawTexturedCube(-ts/2,-ts/2,-ts/2, ts,ts,ts);
        glPopMatrix();
    }
    glDisable(GL_BLEND);
    /* Main projectile */
    glColor3f(0,0.9f,1);
    if (g_game.lightingEnabled) {
        GLfloat em[]={0,0.3f,0.5f,1}; glMaterialfv(GL_FRONT,GL_EMISSION,em);
    }
    glPushMatrix(); glTranslatef(px,py,pz);
    glRotatef(g_game.projectile.rotationAngle,1,0,0);
    glRotatef(g_game.projectile.rotationAngle*1.5f,0,1,0);
    drawTexturedCube(-cs/2,-cs/2,-cs/2, cs,cs,cs);
    glPopMatrix();
    if (g_game.lightingEnabled) {
        GLfloat no[]={0,0,0,1}; glMaterialfv(GL_FRONT,GL_EMISSION,no);
    }
}

static void drawCrosshair() {
    int w=g_game.windowWidth, h=g_game.windowHeight;
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0,w,0,h);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glDisable(GL_TEXTURE_2D);
    glColor3f(1,1,1); glLineWidth(2);
    int cx=w/2, cy=h/2, sz=10;
    glBegin(GL_LINES);
    glVertex2f((float)(cx-sz),(float)cy); glVertex2f((float)(cx+sz),(float)cy);
    glVertex2f((float)cx,(float)(cy-sz)); glVertex2f((float)cx,(float)(cy+sz));
    glEnd();
    glLineWidth(1); glEnable(GL_DEPTH_TEST);
    if (g_game.lightingEnabled) glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

static void drawHUD() {
    int w=g_game.windowWidth, h=g_game.windowHeight;
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0,w,0,h);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glDisable(GL_TEXTURE_2D);

    /* Score */
    char buf[256];
    sprintf(buf, "Cubes: %d / %d", g_game.cubesCollected, g_game.totalCollectibles);
    glColor3f(1,1,1);
    glRasterPos2f(10, (float)(h-25));
    drawBitmapString(buf, GLUT_BITMAP_HELVETICA_18);

    /* Status line */
    sprintf(buf, "Collision:%s | Light:%s | Texture:%s | Wire:%s",
        g_game.collisionEnabled?"ON":"OFF", g_game.lightingEnabled?"ON":"OFF",
        g_game.texturesEnabled?"ON":"OFF", g_game.wireframeMode?"ON":"OFF");
    glColor3f(0.7f,0.7f,0.7f);
    glRasterPos2f(10, (float)(h-45));
    drawBitmapString(buf, GLUT_BITMAP_HELVETICA_12);

    /* Warning: reached goal without all cubes */
    if (g_game.goalWarningTimer > 0.0f && !g_game.gameWon) {
        glColor3f(1.0f, 0.2f, 0.2f);  /* Red warning text */
        glRasterPos2f((float)(w/2-120),(float)(h/2+30));
        drawBitmapString("You need ALL red cubes first!", GLUT_BITMAP_TIMES_ROMAN_24);
        sprintf(buf, "Still missing %d cube(s). Go find them!", g_game.goalWarningRemaining);
        glColor3f(1,1,1);
        glRasterPos2f((float)(w/2-130),(float)(h/2));
        drawBitmapString(buf, GLUT_BITMAP_HELVETICA_18);
    }

    /* Win message */
    if (g_game.gameWon) {
        glColor3f(1,0.84f,0);
        glRasterPos2f((float)(w/2-60),(float)(h/2+20));
        drawBitmapString("YOU WIN!", GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(1,1,1);
        glRasterPos2f((float)(w/2-120),(float)(h/2-10));
        drawBitmapString("All cubes collected & goal reached!", GLUT_BITMAP_HELVETICA_18);
        sprintf(buf, "Final Score: %d / %d cubes", g_game.cubesCollected, g_game.totalCollectibles);
        glRasterPos2f((float)(w/2-120),(float)(h/2-40));
        drawBitmapString(buf, GLUT_BITMAP_HELVETICA_12);
    }

    glEnable(GL_DEPTH_TEST);
    if (g_game.lightingEnabled) glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

/* ==================== Public Functions ==================== */

void initRenderer() {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glFrontFace(GL_CCW);

    printf("[Renderer] Loading textures...\n");
    wallTextureID    = loadTexture("assets/brick.jpg");
    floorTextureID   = loadTexture("assets/floor.jpg");
    ceilingTextureID = loadTexture("assets/ceiling.jpg");
    if (!wallTextureID)    wallTextureID    = loadTexture("assets/brick.png");
    if (!floorTextureID)   floorTextureID   = loadTexture("assets/floor.png");
    if (!ceilingTextureID) ceilingTextureID = loadTexture("assets/ceiling.png");
    printf("[Renderer] Textures: Wall=%u, Floor=%u, Ceiling=%u\n",
           wallTextureID, floorTextureID, ceilingTextureID);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    g_camera.applyView();

    if (g_game.lightingEnabled) {
        GLfloat lp[]={MAZE_SIZE/2.0f, WALL_HEIGHT*5, MAZE_SIZE/2.0f, 1};
        glLightfv(GL_LIGHT0, GL_POSITION, lp);
        glEnable(GL_LIGHT1);
        GLfloat pp[]={g_camera.posX, WALL_HEIGHT*0.8f, g_camera.posZ, 1};
        GLfloat pd[]={0.4f,0.4f,0.35f,1}; GLfloat pa[]={0.1f,0.1f,0.1f,1};
        glLightfv(GL_LIGHT1,GL_POSITION,pp);
        glLightfv(GL_LIGHT1,GL_DIFFUSE,pd);
        glLightfv(GL_LIGHT1,GL_AMBIENT,pa);
        glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,1);
        glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0.3f);
        glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0.1f);
    } else { glDisable(GL_LIGHT1); }

    drawFloor(); drawCeiling(); drawMaze();
    drawCollectibles(); drawGoal(); drawProjectile();
    drawCrosshair(); drawHUD();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    g_game.windowWidth = width; g_game.windowHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(60.0, (float)width/(float)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
