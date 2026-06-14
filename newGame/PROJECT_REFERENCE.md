# newGame Project Reference

## Overview

- Purpose: a simple top-down tank game built with C++, OpenGL and freeglut.
- Core gameplay: the player controls a tank, fires projectiles, and interacts with enemies and obstacles on a 2D scene.
- Main focus of the project: rendering, movement, projectile logic, collision handling, and basic game loop management.

## Build and Run

- Build command: `g++ src/*.cpp -Iinclude -o build/TankGame.exe -lfreeglut -lglu32 -lopengl32`
- Project root for this module: `newGame`
- Run command: `./build/TankGame.exe`

## File Summary

### `src/main.cpp`

- Entry point of the game.
- Creates the game instance and starts the main loop.

### `src/Game.cpp` / `include/Game.h`

- Controls the overall game flow.
- Handles initialization, update logic, rendering order, and object management.
- Coordinates the tank, enemies, projectiles, and obstacles.

### `src/Tank.cpp` / `include/Tank.h`

- Implements the player tank logic.
- Handles movement, aiming, firing, and tank state.

### `src/EnemyTank.cpp` / `include/EnemyTank.h`

- Implements enemy tank behavior.
- Contains basic AI logic for movement and attacking.

### `src/Projectile.cpp` / `include/Projectile.h`

- Represents bullets or shells fired by tanks.
- Updates projectile movement and checks for collisions.

### `src/Obstacle.cpp` / `include/Obstacle.h`

- Represents static obstacles in the game world.
- Used for blocking movement and supporting collision logic.

### `src/Vector2D.cpp` / `include/Vector2D.h`

- 2D vector utility class.
- Used for positions, directions, movement, and distance calculations.

## How the Code Fits Together

- `main.cpp` starts the application and hands control to `Game`.
- `Game` creates and manages the tank, enemies, obstacles, and projectiles.
- `Tank` and `EnemyTank` update their positions and actions each frame.
- `Projectile` objects move through the scene and are checked against other entities.
- `Obstacle` objects provide simple level structure and collision boundaries.
- `Vector2D` supports the math used across the project.

## Gameplay Flow

1. The game starts from `main.cpp`.
2. `Game` initializes the scene and game objects.
3. Each frame, the update step processes input, movement, AI, and collisions.
4. The draw step renders the tank, enemies, obstacles, and projectiles.
5. The loop continues until the player exits.

## Graphics Concepts and Implementations

This section matches the syllabus-style explanation used in the other project reference.

### Translation

- Implemented through world position changes in the update logic.
- `Tank::update()` changes the tank position using its velocity.
- `Projectile::update()` moves projectiles forward along their direction.
- In rendering, `Game::draw()` applies camera translation with `glTranslatef(-camX, -camY, 0)` so the world is drawn relative to the viewport.

### Rotation

- Implemented with angle values stored in the tank and enemy tank classes.
- `Game::update()` calculates the player tank angle from the mouse position using `atan2`.
- `EnemyTank::update()` calculates the enemy angle so it faces the player.
- `Tank::draw()` and `EnemyTank::draw()` apply that rotation with `glRotatef(angle, 0, 0, 1)` before drawing the body and turret.
- `Projectile` uses the firing angle to build its velocity vector with `cos` and `sin`.

### Scaling

- Not implemented as a separate transform in this project.
- The tank, turret, projectile, and obstacle sizes are drawn using fixed vertex coordinates instead of a scale matrix.
- If needed for an assignment explanation, the size of each shape can be treated as hard-coded geometry rather than dynamic scaling.

### Matrix Transformations

- Not used explicitly with custom matrix classes.
- The project relies on the OpenGL matrix stack and immediate-mode transforms instead:
	- `glTranslatef` for translation
	- `glRotatef` for rotation
- The camera effect in `Game::draw()` is also produced through a translation transform.

### Homogeneous Coordinates

- Not implemented explicitly in the source code.
- The project does not define custom 3x3 or 4x4 transform matrices for 2D homogeneous coordinate calculations.

### Viewport / Camera Transformation

- Implemented in the game loop using `camX` and `camY`.
- `Game::update()` centers the camera on the player tank and clamps it to the world bounds.
- `Game::draw()` offsets the scene by the negative camera position so only the visible portion of the world appears inside the window.

### Collision and Point-in-Shape Testing

- Collision is implemented with simple circle and square checks.
- `EnemyTank::checkHit()` uses distance-based circle collision against bullets.
- `Obstacle::checkCollision()` uses point-in-square bounds checking.
- There is no point-in-polygon implementation in this project.

### Polygon Clipping / Line Clipping

- Not implemented.
- The project draws only simple quads and relies on the camera offset instead of clipping algorithms such as Cohen-Sutherland or Liang-Barsky.

### Scan-Line Filling

- Not implemented as an explicit algorithm.
- Filled shapes are drawn directly using OpenGL immediate mode with `GL_QUADS`.

## Summary for Assignment Write-Up

- Translation: yes, through object movement and camera offset.
- Rotation: yes, through stored angles and `glRotatef`.
- Scaling: no explicit scale transform; sizes are hard-coded geometry.
- Matrix transformations: implicit through OpenGL transforms, not custom matrices.
- Homogeneous coordinates: not explicitly used.
- Viewport / camera transformation: yes, through `camX`, `camY`, and `glTranslatef`.
- Clipping, scan-line filling, point-in-polygon: not explicitly implemented.

## Notes

- The project is organized as a small self-contained OpenGL assignment.
- The existing source structure is already split into headers and implementation files, which makes the code easy to document and extend.
- If needed for submission, this file can be expanded with exact function-level notes or line references from each source file.
