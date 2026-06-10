# 2D Top-Down Tank Game - Team Task Breakdown

This TODO assigns tasks that are strictly for the already-built game. Each member should work within the existing source files (annotate, refactor small sections, add debug toggles) rather than creating large new systems. Aim to demonstrate graphics syllabus topics (translation, rotation, scaling, matrix transforms, homogeneous coordinates, clipping, scan-line filling, point-in-polygon, viewport/camera) using the current codebase.

Build order dependency: Member 1 → Member 2 → Members 3,4,5 (3,4,5 can work in parallel after 1 & 2 are done).

---

## Member 1: Core Linear Algebra & Shader Transforms

Responsibility: Implement 2D homogeneous transforms and expose them to the renderer (CPU and shader).

Target files: `src/MathAddon.h/cpp`, `src/Vector2D.h/cpp`, `src/RendererGL.cpp`, `include/RendererGL.h`

Note: Work only within these existing files — prefer small refactors, annotations, and debug toggles. Do not add large new modules.

Tasks:
- [ ] Add `Matrix3x3` (or Matrix3) type: multiplication, multiply by Point2D (x,y,1), and utilities.
- [ ] Implement `getTranslationMatrix(tx,ty)`, `getScalingMatrix(sx,sy)`, `getRotationMatrix(theta)` using radians.
- [ ] Add matrix composition helpers: `compose(trans, rot, scale)` and function `applyMatrixToPoint(matrix, point)`.
- [ ] Update RendererGL to accept a mat3 uniform for object transform (or mat4 if easier). Replace manual cos/sin per-vertex with matrix multiply in vertex shader. Provide fallback CPU path.
- [ ] Add unit tests or small demo scene that draws a single sprite using the new matrix path.

Mapping to syllabus: homogeneous coordinates, matrix transforms, application of translation/rotation/scaling via matrices.

What you'll see in-game after Member 1's work:
- Demo sprite driven by a model matrix: translation, rotation, and scaling respond to debug keys.
- Renderer toggle (old math vs matrix-uniform shader) with visually matching output.
- Small transform overlay showing mat3 values and smoother GPU-driven rotations.

---

## Member 2: Applied Sprite Transforms, World->Screen & Camera

Responsibility: Hook Matrix math into Sprite drawing and implement camera (viewport) transforms, zoom and parallax.

Target files: `src/Sprite.cpp`, `src/Sprite.h`, `src/Game.cpp`, `src/RendererGL.cpp`

Note: Modify existing functions in these files (Sprite::drawBody, Game::updateCamera). Avoid creating new high-level systems; add debug keys or small helper functions as needed.

Tasks:
- [ ] Replace current manual pixel rect computation with building a model matrix for each sprite: M = T_camera^-1 * T_object * R * S (or equivalent). Use Matrix3x3 utilities from Member 1.
- [ ] Implement camera zoom and pan: expose camera position and scale in Game and pass view matrix to RendererGL as uniform.
- [ ] Ensure world-to-screen uses homogeneous matrix multiplication; update Sprite::drawBody to use composed matrix.
- [ ] Demonstrate translation (move), scaling (zoom & sprite scale), rotation (turret/body) via toggles or debug keys.

Mapping to syllabus: viewport/camera transforms, translation, scaling applied to sprites, matrix composition.

What you'll see in-game after Member 2's work:
- Camera pan and zoom controls that scale and translate the world; sprites and UI update uniformly.
- Sprites use composed model-view matrices for consistent movement, rotation, and scaling.
- Debug toggle to display the view matrix and world→screen coordinate mapping.

---

## Member 3: Polygon/Line Clipping, Point-in-Polygon, and Selection

Responsibility: Implement clipping and selection algorithms required by the syllabus and integrate them for debug/demo overlays.

Target files: `src/Clipping.cpp` (prefer adding small file if necessary), `src/Game.cpp` (use existing debug hooks), `src/Sprite.cpp`

Note: Prefer implementing algorithms as small utilities and integrate them into existing debug code. If a new file (e.g., Clipping.cpp) is created, keep it minimal and justified.

Tasks:
- [ ] Implement Sutherland–Hodgman polygon clipping against the window/view rectangle for debug polygons and overlays.
- [ ] Implement Cohen–Sutherland or Liang–Barsky line clipping for debug lines (e.g., weapon traces, laser beams).
- [ ] Implement `isPointInPolygon(point, polygon)` (even-odd rule) for selection/hit testing demonstration.
- [ ] Add a debug mode where a polygon (or arbitrary polyline) can be drawn and then clipped to the viewport, showing before/after.

Mapping to syllabus: polygon clipping, line clipping, point-in-polygon hit testing.

What you'll see in-game after Member 3's work:
- Debug mode showing original vs clipped polygons/lines at the viewport edges.
- Click-to-select highlights polygons using point-in-polygon testing.
- Weapon traces or laser previews visibly truncated at the window boundaries using the clipping algorithm.

---

## Member 4: Scan-line Fill (Demo Layer), Shaders & Rendering Polishing

Responsibility: Provide a CPU scan-line fill demo and update shaders to use transform uniforms from Member 1/2.

Target files: `src/RendererGL.cpp`, `src/DebugFill.cpp` (only if adding a small helper), `shaders/*` (vertex/fragment)

Note: Implement CPU scan-line fill as a small debug helper. Prefer integrating into existing renderer/debug UI rather than large refactors.

Tasks:
- [ ] Add a debug polygon fill implementation (scan-line) that can run in CPU and render the filled polygon as a debug overlay (use fragment shader to color the overlay).
- [ ] Modify vertex shader to accept a mat3/mat4 transform uniform (object-to-clip) and update fragment shader inputs accordingly.
- [ ] Ensure alpha blending and ordering remain correct (shadows drawn first, filled polygons overlayed as needed).
- [ ] Add a demo key to toggle CPU scan-line fill vs. shader fill for pedagogical comparison.

Mapping to syllabus: scan-line polygon filling, shader-based rasterization vs CPU filling.

What you'll see in-game after Member 4's work:
- Overlay comparing CPU scan-line fill and shader-based fill for the same polygon.
- A key to toggle CPU vs shader fill so performance and visual differences are observable.
- Debug fills respect shadow ordering and alpha blending, appearing as a pedagogical overlay.

---

## Member 5: Collision, Clipping Integration, and Demonstrations

Responsibility: Tie clipping, transforms and collision into gameplay features and demos.

Target files: `src/Projectile.cpp`, `src/Destructible.cpp`, `src/UnitEnemy.cpp`, `src/Game.cpp`, `src/Sprite.cpp`

Note: Work within existing gameplay files to add demo toggles or small visualizations (polygon outlines, clipped traces). Avoid replacing collision systems unless to add an optional demo mode.

Tasks:
- [ ] Keep circle-overlap collision but add optional polygon collision demo: compute AABB/polygon collision for a destructible obstacle and show demo when toggled.
- [ ] Integrate line clipping for projectile traces (visually clip traces to the viewport) using Member 3 implementation.
- [ ] Add a demo level or keyboard-driven scenario that highlights translation (moving units), scaling (zoom camera), rotation (turret), clipping (drawn polygons clipped to viewport), and scanline fill (filled destructible polygon overlay).
- [ ] Update spawn checks to respect new transforms (camera/scaling) when computing on-screen positions for debug UI.

Mapping to syllabus: collision (comparing circle vs polygon), practical usage of clipping and transforms in gameplay.

What you'll see in-game after Member 5's work:
- Optional polygon-collision demo for destructibles: obstacles show polygon outlines and polygon-vs-projectile hits when enabled.
- Projectile traces visually clipped to the viewport and aligned with camera/zoom transforms.
- A demo scenario or toggle that highlights translation, scaling, rotation, clipping, and scan-line fill together.

---

## Shared Tasks & Acceptance Criteria

- Add `// TODO:` comments at the top of each modified existing file showing which member is assigned. Do not create new files unless approved by the team lead; small helper files are allowed if justified.
- Each member should provide a short demo keybinding list and a brief README snippet in repo root `game/TODO.md` describing how to run their demo.
- Acceptance tests (manual):
  - Translation: Move player and enemies; positions visibly change.
  - Rotation: Turret rotates smoothly to aim at mouse; enemy rotation behavior remains.
  - Scaling: Camera zoom works and sprites scale visually.
  - Matrix transforms: Switch renderer to matrix-uniform path and verify same visual output.
  - Clipping: Drawn polygon/line overlays are clipped to viewport and visible differences shown.
  - Scan-line fill: Toggle shows CPU filled polygon in debug overlay.

---

## Notes & Hints

- Useful files: `src/RendererGL.cpp` (vertex shader shows current per-vertex math), `src/Sprite.cpp` (drawBody does world->screen conversion), `src/Game.cpp` (camera update & spawn logic).
- If implementing matrix uniforms in shader, prefer `mat3` + vec3 homogeneous coordinates for 2D; `mat4` is acceptable.
- Keep backward-compatible fallback: if matrix uniform not set, continue using existing pixel math.
- Use existing Vector2D utilities where possible and extend MathAddon for constants and helper conversions.

---

If this division looks good, confirm and each member can acknowledge their assigned tasks by adding their name and marking the first checkbox in their section.
