# Setup Guide — 3D Maze Game (Windows + Visual Studio)

This guide explains how to set up, build, and run the 3D Maze Game project using **Visual Studio 2022** (or 2019) on Windows.

The project uses **standard GLUT** (`glut.h` + `glut32.lib`) — the original OpenGL Utility Toolkit.

---

## Prerequisites

- **Visual Studio 2022** (or 2019) with the **"Desktop development with C++"** workload installed.
- **Windows 10 or 11**
- **Standard GLUT** installed on the system (see Step 1)

---

## Step 1: Verify GLUT is Installed

Standard GLUT should already be on your machine if you've used it in university coursework. Verify that the following files exist:

### Header File
```
C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\<version>\include\GL\glut.h
```

### Library File
```
C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\<version>\lib\x86\glut32.lib
```

### DLL File (one of these locations)
```
C:\Windows\SysWOW64\glut32.dll    (for 32-bit apps on 64-bit Windows)
C:\Windows\System32\glut32.dll    (for 32-bit Windows)
```

> **Note:** Replace `<version>` with your MSVC version number (e.g., `14.35.32215`). The exact path may vary depending on your Visual Studio edition (Community, Professional, Enterprise).

### If GLUT is NOT installed

1. Download GLUT binaries from: http://www.xmission.com/~nate/glut/glut-3.7.6-bin.zip
2. Extract the ZIP file
3. Copy the files to the appropriate locations:
   - `glut.h` → into the `GL\` folder inside your MSVC `include` directory (create the `GL` folder if needed)
   - `glut32.lib` → into your MSVC `lib\x86\` directory
   - `glut32.dll` → into `C:\Windows\SysWOW64\` (for 64-bit Windows) or `C:\Windows\System32\` (for 32-bit Windows)

---

## Step 2: Download stb_image.h

The project uses `stb_image.h` for texture loading. It should already be included in the project folder.

If it's missing:
1. Go to: https://github.com/nothings/stb/blob/master/stb_image.h
2. Click **"Raw"** to view the raw file
3. Right-click → **"Save As..."** → save as `stb_image.h`
4. Place `stb_image.h` in the project folder (same folder as `main.cpp`)

---

## Step 3: Add Texture Images

Create an `assets/` folder in the project directory and add three texture images:

```
3DMazeGame/
├── assets/
│   ├── brick.jpg     (Wall texture — any brick/stone image)
│   ├── floor.jpg     (Floor texture — stone, tile, or wood)
│   └── ceiling.jpg   (Ceiling texture — wood planks or plaster)
```

**Requirements:**
- Images should be square (e.g., 256×256, 512×512, or 1024×1024)
- JPG or PNG format
- You can find free textures at:
  - https://www.textures.com/
  - https://polyhaven.com/textures
  - https://ambientcg.com/

> **Note:** If no texture images are found, the game will still run — surfaces will use solid colors instead.

---

## Step 4: Configure the Visual Studio Project

The project file (`.vcxproj`) should already have the correct settings. To verify:

1. Open `3DMazeGame.sln` in Visual Studio
2. Right-click the project → **Properties**
3. **IMPORTANT:** Set **Platform** to **Win32** (not x64) — standard GLUT is 32-bit

### Linker → Input
- **Additional Dependencies** should contain:
  ```
  glut32.lib;opengl32.lib;glu32.lib;%(AdditionalDependencies)
  ```

### General
- **Platform Toolset:** `Visual Studio 2022 (v143)` for VS 2022, or `Visual Studio 2019 (v142)` for VS 2019
- If you get a retarget prompt when opening, accept it.

---

## Step 5: Set Working Directory (Important for Textures)

The game loads textures from `assets/` relative to the working directory.

1. Right-click the project → **Properties**
2. Go to **Debugging**
3. Set **Working Directory** to: `$(ProjectDir)`

This ensures the game looks for `assets/brick.jpg` etc. in the correct location.

---

## Step 6: Build and Run

1. Set the configuration to **Debug | Win32** in the toolbar
2. Press **F7** to build (Build → Build Solution)
3. If there are errors, check:
   - Is `stb_image.h` in the project folder?
   - Is `glut.h` in the GL include directory?
   - Is `glut32.lib` in the lib directory?
   - Is the platform set to **Win32** (not x64)?
4. Press **F5** to run (Debug → Start Debugging)
5. If you get a "glut32.dll not found" error, make sure `glut32.dll` is in `C:\Windows\SysWOW64\` or in the same folder as the built `.exe`

---

## Step 7: Play!

Use WASD to move, mouse to look around, and navigate to the golden cube to win!

---

## Troubleshooting

| Problem | Solution |
|---|---|
| `Cannot open include file: 'GL/glut.h'` | `glut.h` not found. Check Step 1 — copy it to your MSVC include/GL directory. |
| `LINK : fatal error LNK1104: cannot open file 'glut32.lib'` | `glut32.lib` not found. Copy it to your MSVC lib directory. Check Step 1. |
| `unresolved external symbol` with glut functions | `glut32.lib` not linked. Check Step 4 (Linker → Input). |
| `glut32.dll not found` | Copy `glut32.dll` to `C:\Windows\SysWOW64\` or to the exe output folder (`bin\Debug\`). |
| `Failed to load texture 'assets/brick.jpg'` | Create the `assets/` folder and add images. See Step 3. Or set Working Directory (Step 5). |
| Build error: `stb_image.h: No such file` | Download `stb_image.h` and place it in the project folder. See Step 2. |
| Platform Toolset error | Right-click project → Retarget solution to your installed version. |
| Linker errors on x64 platform | Standard GLUT (`glut32.lib`) is 32-bit only. Switch to **Win32** platform in Configuration Manager. |

---

## For Visual Studio 2019

The project uses Platform Toolset `v143` (VS 2022). To use VS 2019:
1. Open the solution in VS 2019
2. When prompted to retarget, select `v142` and click OK
3. Everything else remains the same
