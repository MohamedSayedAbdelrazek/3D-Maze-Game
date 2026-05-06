# 3D Maze Game — Project Documentation

---

## Page 1: Cover Page

---

<div align="center">

# 3D Maze Game

### Computer Graphics Course Project

<br/><br/>

**Team Members:**

| # | Name | ID |
|---|------|----|
| 1 | ______________________________ | ____________ |
| 2 | ______________________________ | ____________ |
| 3 | ______________________________ | ____________ |
| 4 | ______________________________ | ____________ |

<br/>

**Under the Supervision of:**

**Dr.** ______________________________

**Eng.** ______________________________

<br/>

**University:** ______________________________

**Faculty:** ______________________________

**Department:** ______________________________

**Academic Year:** ______________________________

</div>

---

## Page 2: Project Description

---

### Introduction

The **3D Maze Game** is an interactive first-person game developed using **C++** and **OpenGL**. The player navigates a 20×20 maze in a first-person perspective, collects items, avoids dead ends, and reaches a goal to win. The project demonstrates key computer graphics concepts learned in the course.

---

### Computer Graphics Features Implemented

The project implements the following **7 computer graphics features** as required by the course:

#### 1. Lighting
The scene is illuminated using OpenGL's fixed-function lighting system. **GL_LIGHT0** provides ambient, diffuse, and specular lighting from a positional light above the maze center. A secondary light follows the player for local illumination. Lighting can be toggled on/off at runtime.

#### 2. Texture Mapping
Wall, floor, and ceiling surfaces are textured using images loaded with the **stb_image** library. Textures are applied using `glTexCoord2f` with each vertex, and configured with `GL_LINEAR` filtering and `GL_REPEAT` wrapping for seamless tiling. Textures can be toggled on/off.

#### 3. Camera (First-Person View)
The camera uses **`gluLookAt`** to create a first-person perspective. The camera position moves with the player, while yaw (horizontal) and pitch (vertical) angles control the viewing direction. This replaces the simpler `glRotate` approach with a proper FPS camera system.

#### 4. Collision Detection
Wall collision is detected using the maze grid as a spatial lookup. Before each movement, the player's bounding box is checked against neighboring cells. If any corner overlaps a wall cell, the movement is blocked. A wall-sliding algorithm allows smooth movement along walls. Collision can be toggled for debugging.

#### 5. Keyboard and Mouse Input
**Keyboard:** WASD keys and arrow keys control movement. Additional keys toggle wireframe mode (Q), collision (C), lighting (L), and textures (T). Space fires a projectile, and ESC exits.
**Mouse:** Passive mouse motion (`glutPassiveMotionFunc`) provides FPS-style camera rotation. The cursor is warped to the window center after each frame to enable unlimited rotation.

#### 6. 3D Environment
The maze is stored as a 20×20 integer grid and rendered as 3D cubes using `glBegin(GL_QUADS)`. Each wall cell is drawn as a textured box. The environment includes a textured floor plane at ground level and a ceiling plane at wall height, creating a fully enclosed corridor experience.

#### 7. Animation
Multiple animation techniques are used:
- **Collectible cubes** rotate continuously around the Y-axis at 90°/second.
- **Goal cube** pulses in size (scales between 0.7× and 1.3×) to create a "breathing" glow effect.
- **Projectile** spins on multiple axes during flight and leaves a transparent trailing effect using alpha blending.

---

### Technologies Used

| Technology | Purpose |
|---|---|
| **C++ (C++11)** | Programming language |
| **OpenGL 1.x/2.x** | Graphics rendering (fixed-function pipeline) |
| **GLUT (OpenGL Utility Toolkit)** | Window management, input handling, main loop |
| **stb_image.h** | Image file loading (JPG, PNG) for textures |
| **Visual Studio 2022** | IDE and build system |
| **Windows 10/11** | Target platform |

---

## Page 3: How to Play

---

### Objective

Navigate through the 3D maze from the starting entrance at the bottom and reach the **golden pulsing cube** (the goal) located in the upper section of the maze. Collect **red cubes** along the way for bonus points!

---

### Controls

| Control | Action |
|---|---|
| **W** / **Up Arrow** | Move forward |
| **S** / **Down Arrow** | Move backward |
| **A** / **Left Arrow** | Strafe left |
| **D** / **Right Arrow** | Strafe right |
| **Mouse** | Look around (FPS-style camera) |
| **Space** | Fire a projectile |
| **Q** | Toggle wireframe / solid rendering |
| **C** | Toggle wall collision on/off |
| **L** | Toggle lighting on/off |
| **T** | Toggle texture mapping on/off |
| **ESC** | Exit the game |

---

### Tips for Playing

1. **Use the mouse to peek around corners** before committing to a direction — this helps avoid dead ends and find the fastest route through the maze.

2. **Look for the golden glow** — the goal cube pulses and emits a golden light, making it visible from a distance. Head towards it when you spot it through the corridors.

3. **Collect red cubes for points** — there are 5 red cubes scattered throughout the maze. They rotate to catch your attention. Walking over them automatically picks them up.

4. **Use toggle keys to explore** — try pressing L to see the maze without lighting, T to see it without textures, or Q for wireframe mode. These are great for understanding how the graphics pipeline works!

5. **Press C if you get stuck** — disabling collision lets you walk through walls, which is useful for finding the goal or getting unstuck.

---
