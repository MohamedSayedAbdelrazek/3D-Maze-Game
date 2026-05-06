#pragma once
/*
 * Game.h - Game State, Constants, and Logic (merged header + implementation)
 */

#include <cstdio>
#include <cmath>

/* ==================== Constants ==================== */
const int MAZE_SIZE = 20;
const float CELL_SIZE = 1.0f;
const float WALL_HEIGHT = 1.0f;
const float PLAYER_HEIGHT = 0.5f;
const float PLAYER_RADIUS = 0.15f;
const float MOVE_SPEED = 0.05f;
const float MOUSE_SENSITIVITY = 0.2f;
const float PROJECTILE_SPEED = 0.15f;
const float PROJECTILE_LIFETIME = 3.0f;
const int GOAL_ROW = 1;
const int GOAL_COL = 17;
const float START_X = 9.5f;
const float START_Z = 18.5f;
const float START_YAW = 0.0f;
#ifndef GAME_PI
#define GAME_PI 3.14159265358979f
#endif

/* ==================== Data Structures ==================== */

struct Projectile {
    bool active;
    float posX, posY, posZ;
    float dirX, dirZ;
    float rotationAngle;
    float timeAlive;
};

struct GameState {
    int maze[MAZE_SIZE][MAZE_SIZE];
    bool collisionEnabled;
    bool wireframeMode;
    bool lightingEnabled;
    bool texturesEnabled;
    Projectile projectile;
    int cubesCollected;
    int totalCollectibles;
    bool gameWon;
    float collectibleRotation;
    float goalPulseScale;
    bool goalPulseGrowing;
    int windowWidth;
    int windowHeight;

    /* Warning message when reaching goal without all cubes */
    float goalWarningTimer;      /* Countdown timer for the warning message */
    int goalWarningRemaining;    /* How many cubes were still needed */
};

/* ==================== Global Instance ==================== */
GameState g_game;

/* ==================== Maze Layout ==================== */
static const int INITIAL_MAZE[MAZE_SIZE][MAZE_SIZE] = {
    {1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,2,0,1,1,0,0,0,1},
    {1,0,0,0,0,2,0,0,0,1,1,0,0,0,1,1,0,0,0,1},
    {1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,0,2,0,1},
    {1,0,0,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,1},
    {1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,0,1,1,0,0,0,1,1,0,0,1},
    {1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,1,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,1,1,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,1,1,0,1,0,0,0,0,0,1,1,0,1,1,1},
    {1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1},
    {1,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,1,0,1},
    {1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,1},
    {1,1,0,0,1,1,1,0,0,0,0,0,0,0,1,0,0,1,0,1},
    {1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1}
};

/* ==================== Implementation ==================== */

void initGame() {
    g_game.totalCollectibles = 0;
    for (int row = 0; row < MAZE_SIZE; row++) {
        for (int col = 0; col < MAZE_SIZE; col++) {
            g_game.maze[row][col] = INITIAL_MAZE[row][col];
            if (g_game.maze[row][col] == 2) g_game.totalCollectibles++;
        }
    }
    g_game.collisionEnabled = true;
    g_game.wireframeMode = false;
    g_game.lightingEnabled = true;
    g_game.texturesEnabled = true;
    g_game.projectile.active = false;
    g_game.projectile.posX = g_game.projectile.posY = g_game.projectile.posZ = 0.0f;
    g_game.projectile.dirX = g_game.projectile.dirZ = 0.0f;
    g_game.projectile.rotationAngle = g_game.projectile.timeAlive = 0.0f;
    g_game.cubesCollected = 0;
    g_game.gameWon = false;
    g_game.collectibleRotation = 0.0f;
    g_game.goalPulseScale = 1.0f;
    g_game.goalPulseGrowing = true;
    g_game.windowWidth = 800;
    g_game.windowHeight = 600;
    g_game.goalWarningTimer = 0.0f;
    g_game.goalWarningRemaining = 0;
    printf("[Game] Initialized. %d collectible cubes in the maze.\n", g_game.totalCollectibles);
}

/*
 * updateGame - takes camera position as parameters to avoid
 * circular dependency with Camera.h
 */
void updateGame(float deltaTime, float camX, float camZ) {
    /* Collectible rotation animation (90 degrees/sec) */
    g_game.collectibleRotation += 90.0f * deltaTime;
    if (g_game.collectibleRotation >= 360.0f) g_game.collectibleRotation -= 360.0f;

    /* Warning message countdown */
    if (g_game.goalWarningTimer > 0.0f) {
        g_game.goalWarningTimer -= deltaTime;
    }

    /* Goal pulse animation (gentle breathing effect) */
    float pulseSpeed = 0.3f * deltaTime;
    if (g_game.goalPulseGrowing) {
        g_game.goalPulseScale += pulseSpeed;
        if (g_game.goalPulseScale >= 1.15f) { g_game.goalPulseScale = 1.15f; g_game.goalPulseGrowing = false; }
    } else {
        g_game.goalPulseScale -= pulseSpeed;
        if (g_game.goalPulseScale <= 0.85f) { g_game.goalPulseScale = 0.85f; g_game.goalPulseGrowing = true; }
    }

    /* Projectile update */
    if (g_game.projectile.active) {
        g_game.projectile.posX += g_game.projectile.dirX * PROJECTILE_SPEED;
        g_game.projectile.posZ += g_game.projectile.dirZ * PROJECTILE_SPEED;
        g_game.projectile.rotationAngle += 360.0f * deltaTime;
        if (g_game.projectile.rotationAngle >= 360.0f) g_game.projectile.rotationAngle -= 360.0f;
        g_game.projectile.timeAlive += deltaTime;
        if (g_game.projectile.timeAlive >= PROJECTILE_LIFETIME) g_game.projectile.active = false;
        int pc = (int)g_game.projectile.posX, pr = (int)g_game.projectile.posZ;
        if (pr < 0 || pr >= MAZE_SIZE || pc < 0 || pc >= MAZE_SIZE || g_game.maze[pr][pc] == 1)
            g_game.projectile.active = false;
    }

    /* Collectible pickup & goal check */
    int playerCol = (int)camX, playerRow = (int)camZ;
    if (playerRow >= 0 && playerRow < MAZE_SIZE && playerCol >= 0 && playerCol < MAZE_SIZE) {
        if (g_game.maze[playerRow][playerCol] == 2) {
            g_game.maze[playerRow][playerCol] = 0;
            g_game.cubesCollected++;
            printf("[Game] Collected cube! Total: %d / %d\n", g_game.cubesCollected, g_game.totalCollectibles);
        }
        if (playerRow == GOAL_ROW && playerCol == GOAL_COL && !g_game.gameWon) {
            if (g_game.cubesCollected >= g_game.totalCollectibles) {
                /* All cubes collected - player wins! */
                g_game.gameWon = true;
                printf("[Game] GOAL REACHED with all cubes! You Win!\n");
            } else {
                /* Not all cubes collected - show warning */
                int remaining = g_game.totalCollectibles - g_game.cubesCollected;
                if (g_game.goalWarningTimer <= 0.0f) {
                    g_game.goalWarningRemaining = remaining;
                    g_game.goalWarningTimer = 3.0f;  /* Show message for 3 seconds */
                    printf("[Game] Need %d more cubes before you can win!\n", remaining);
                }
            }
        }
    }
}

/* Takes camera position/direction as params to avoid Camera dependency */
void fireProjectile(float camX, float camY, float camZ, float fwdX, float fwdZ) {
    if (g_game.projectile.active) return;
    g_game.projectile.posX = camX;
    g_game.projectile.posY = camY;
    g_game.projectile.posZ = camZ;
    g_game.projectile.dirX = fwdX;
    g_game.projectile.dirZ = fwdZ;
    g_game.projectile.rotationAngle = 0.0f;
    g_game.projectile.timeAlive = 0.0f;
    g_game.projectile.active = true;
    printf("[Game] Projectile fired!\n");
}
