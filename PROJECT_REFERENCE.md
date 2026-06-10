Project reference for CG_assigment game

Overview

- Purpose: top-down tank shooter. Player controls a tank (blue) that moves and shoots; enemies (red) and destructible obstacles (orange squares) populate the level. Simple physics, circle collision, camera centering, and OpenGL-based shape rendering.
- Controls: WASD to move, mouse to aim, left-click to shoot, ESC to quit.

File summary (what each file does)

src/
- main.cpp
  - Initializes SDL/OpenGL, GLEW, enables alpha blending, creates RendererGL, starts Game.

- RendererGL.cpp / include/RendererGL.h
  - Implements OpenGL rendering via a small shader pair. Vertex shader applies rotation, scaling and pixel-to-clip conversion; fragment shader draws filled rectangles or ovals. Exposes fillRect/fillOval and setDrawColor. Base for all drawing.

- Sprite.cpp / include/Sprite.h
  - Base drawable entity. Stores pos, angle, radius and provides drawBody() which converts game coordinates + camera to pixel rects and calls RendererGL to draw shapes. Provides circle-overlap collision helpers.

- Vector2D.cpp / include/Vector2D.h
  - 2D vector utilities (normalise, magnitude, angle(), dot/cross, angleBetween). Used for movement, rotation math, and projectile directions.

- MathAddon.cpp / include/MathAddon.h
  - Small math helpers (PI, deg/rad conversion, random float/angle).

- Timer.cpp / include/Timer.h
  - Simple countdown/countup timer used for weapon cooldowns.

- Unit.cpp / include/Unit.h
  - Generic unit logic: turret drawing, shooting helper, weapon cooldown, health.

- UnitPlayer.cpp / include/UnitPlayer.h
  - Player-specific behavior: movement (WASD), aiming at mouse (posViewTarget), level/experience, drawing body/turret and shadow, calls Unit::shootProjectile.

- UnitEnemy.cpp / include/UnitEnemy.h
  - Enemy AI: rotates toward player, shoots when within range (updateAngle and update), draws body/turret and shadow. Health/experience value.

- Projectile.cpp / include/Projectile.h
  - Projectile position update, collision checks (against enemies, destructibles, or player), draw and shadow.

- Destructible.cpp / include/Destructible.h
  - Static obstacles that have health and give experience when destroyed; draw and shadow.

- Game.cpp / include/Game.h
  - Game loop: event processing, update loop, camera update (center on player), spawn setup (addRandomUnitEnemies/addRandomDestructibles), drawing order (shadows then shapes), management of lists (enemies, projectiles, destructibles). Collision/respawn checks: computeRandomPositionForSpawn, checkIfPositionOkForSpawn.

Assets/
- contains DLLs/exe and other resources (not source). The game uses SDL, GLEW, and OpenGL.

How files connect (flow)

- main.cpp -> creates RendererGL -> creates Game(window, renderer).
- Game constructs player UnitPlayer and populates vectors of UnitEnemy and Destructible.
- Game loop calls processEvents (input) -> update (unit updates, projectiles, remove dead ones) -> draw (camera offset, draw shadows then shapes via Sprite::drawBody -> RendererGL).
- Unit, Projectile and Destructible inherit from Sprite and rely on RendererGL via Sprite::drawBody.
- Vector2D and MathAddon provide math used across movement, aiming, and rotation.

Mapping to classic graphics syllabus

- Translation: Covered. Implemented by changing sprite positions (UnitPlayer::update moves pos; Projectile::update moves pos). Conversion from world to screen uses posCamera in Sprite::drawBody (Sprite.cpp drawBody).

- Rotation: Covered. Rotation stored as angle for sprites and applied in RendererGL vertex shader (RendererGL.cpp vertex shader: computes posRot using cos/sin). Unit rotation updated in UnitPlayer (angle set to (posViewTarget - pos).angle()) and UnitEnemy::updateAngle.

- Scaling: Covered. World-to-screen scaling uses tileSize multiplied in Sprite::drawBody (size calculation) and vertex shader uses rectPixels.zw and windowWHPixels to scale to clip space.

- Matrix transformations: Partial. The renderer performs translation/rotation/scaling math manually in the vertex shader (using cos/sin and arithmetic) rather than explicit 3x3 or 4x4 matrices or GLM matrices. The shader logic in RendererGL.cpp acts like combined transforms.

- Homogeneous coordinates: Not explicitly used. The shader maps pixel coordinates to clip space without using explicit homogeneous 3/4 component transform matrices.

- Polygon clipping / Line clipping: Not implemented. Rendering uses simple rectangle/oval draws; there is no Cohen–Sutherland or Liang–Barsky clipping logic.

- Scan-line filling: Not implemented; fragment shader fills shapes per-fragment (procedural), but no CPU-side scan-line polygon filling.

- Point-in-polygon testing: Not implemented. Collision uses circle-overlap (Sprite::checkCircleOverlap) rather than point-in-polygon.

- Viewport / Camera transformations: Covered. Game::updateCamera computes posCamera to center player; Sprite::drawBody uses posCamera to convert world positions to window pixels; RendererGL maps pixel rect to normalized clip coordinates.

Summary of covered items: Translation, Rotation, Scaling, Viewport/Camera transformations. Partial: Matrix transformations (manual math). Not covered: Homogeneous coordinates (explicit), Polygon clipping, Line clipping, Scan-line filling, Point-in-polygon testing.

Code sections responsible for game features

- Barriers (obstacles): Destructible class (Destructible.cpp/h). Spawned by Game::addRandomDestructibles and stored in Game::listDestructibles. They have health and are drawn as orange squares.

- Shadows of obstacles, enemies, tank, projectiles:
  - drawShadow methods: Destructible::drawShadow, UnitPlayer::drawShadow, UnitEnemy::drawShadow, Projectile::drawShadow. Shadows drawn before main shapes in Game::draw to appear beneath.

- Enemies and tank:
  - Player: UnitPlayer (UnitPlayer.cpp/h). Controls, movement, shooting.
  - Enemy: UnitEnemy (UnitEnemy.cpp/h). AI for aiming and shooting, health and experience.

- Enemy logic & spawning:
  - Spawning: Game::addRandomUnitEnemies uses computeRandomPositionForSpawn which attempts random positions and uses checkIfPositionOkForSpawn to avoid player/enemy/destructible overlap.
  - AI: UnitEnemy::update checks distance to player (weaponRange) and calls updateAngle; updateAngle computes angleBetween and rotates toward player (speedAngular) and triggers Unit::shootProjectile when angle aligned.

Other useful code behavior

- Collision detection: Sprite::checkCircleOverlap and Projectile::checkCollisions handle collisions between circles (player, enemies, destructibles, projectiles).
- Drawing order: Game::draw renders shadows first (so they appear beneath shapes), then shapes, then overlay on death.
- Camera: Game::updateCamera centers camera on player using window size and tileSize.

Notes & suggestions (for assignment write-up)

- For graphics syllabus mapping, reference lines:
  - Rotation math in shader: src/RendererGL.cpp (vertexShaderSource: posRot computation).
  - World->screen & camera: src/Sprite.cpp drawBody (conversion using tileSize and posCamera) and src/Game.cpp updateCamera.
  - Movement/translation: src/UnitPlayer.cpp update and src/Projectile.cpp update.
- If required to demonstrate matrix/homogeneous coordinates explicitly, add a small Matrix2x2/3x3 utility and rewrite shader or CPU transform to use matrix multiplication.

---

If you want, can produce a shorter README suitable for your assignment submission or annotate specific functions with exact line numbers. Tell me which format (short report, annotated code, or presentation slide notes) you prefer.