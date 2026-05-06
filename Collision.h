#pragma once
/*
 * Collision.h - Collision Detection (merged header + implementation)
 */

#include "Game.h"

/*
 * Check if position (x,z) would collide with a wall.
 * Uses 4-corner bounding box for reliable detection.
 */
bool checkWallCollision(float x, float z) {
    for (int dx = -1; dx <= 1; dx += 2) {
        for (int dz = -1; dz <= 1; dz += 2) {
            int col = (int)(x + dx * PLAYER_RADIUS);
            int row = (int)(z + dz * PLAYER_RADIUS);
            if (row < 0 || row >= MAZE_SIZE || col < 0 || col >= MAZE_SIZE) return true;
            if (g_game.maze[row][col] == 1) return true;
        }
    }
    return false;
}

/* Check if player overlaps a collectible cube (value 2) */
bool checkCubeCollision(float x, float z) {
    int col = (int)x, row = (int)z;
    if (row < 0 || row >= MAZE_SIZE || col < 0 || col >= MAZE_SIZE) return false;
    if (g_game.maze[row][col] == 2) {
        g_game.maze[row][col] = 0;
        g_game.cubesCollected++;
        return true;
    }
    return false;
}
