/*
 * main.cpp - Application Entry Point
 *
 * This is the ONLY .cpp file in the project.
 * All other modules are header-only (.h files with implementations).
 *
 * Include order matters: Game → Camera → Collision → Lighting →
 * TextureLoader → Renderer → Input
 */

#include "Renderer.h"   /* Includes Game.h, Camera.h, TextureLoader.h, Lighting.h */
#include "Input.h"      /* Includes Game.h, Camera.h, Collision.h, Lighting.h */

/* ==================== Constants ==================== */
const int INITIAL_WINDOW_WIDTH = 1024;
const int INITIAL_WINDOW_HEIGHT = 768;
const char* WINDOW_TITLE = "3D Maze Game - Computer Graphics Project";
const int TIMER_INTERVAL_MS = 16;  /* ~60 FPS */

/* ==================== Timer Callback ==================== */

/* Called every ~16ms to update game logic and request redraw */
void timerCallback(int value) {
    float deltaTime = (float)TIMER_INTERVAL_MS / 1000.0f;
    updateGame(deltaTime, g_camera.posX, g_camera.posZ);
    glutPostRedisplay();
    glutTimerFunc(TIMER_INTERVAL_MS, timerCallback, 0);
}

/* ==================== Main ==================== */

int main(int argc, char** argv) {
    printf("===========================================\n");
    printf("  3D Maze Game - Computer Graphics Project\n");
    printf("===========================================\n\n");

    /* Initialize GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
    glutInitWindowPosition(100, 50);
    glutCreateWindow(WINDOW_TITLE);

    /* Initialize game state and camera */
    initGame();
    g_camera.init(START_X, PLAYER_HEIGHT, START_Z, START_YAW);

    /* Set up OpenGL lighting and renderer */
    setupLighting();
    initRenderer();
    glEnable(GL_TEXTURE_2D);

    /* Register GLUT callbacks */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(handleSpecialKeys);
    glutPassiveMotionFunc(handleMouseMotion);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutTimerFunc(TIMER_INTERVAL_MS, timerCallback, 0);

    /* Print controls */
    printf("\n--- Controls ---\n");
    printf("  WASD / Arrows : Move\n");
    printf("  Mouse         : Look around\n");
    printf("  Space         : Fire projectile\n");
    printf("  Q/C/L/T       : Toggle wireframe/collision/lighting/textures\n");
    printf("  ESC           : Exit\n");
    printf("----------------\n\n");

    glutMainLoop();
    return 0;
}
