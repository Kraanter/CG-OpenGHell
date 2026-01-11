# CG-OpenGHell

## Overzicht
Dit project is een OpenGL-applicatie met scenes en een eenvoudige spel-flow:
1) Selecteer een auto in de car scene.
2) Verken de kleine wereld in de track scene.
3) Stap in de auto en rijd over een spline‑track in de spline scene.

Het gebruikt een gedeeld scene-framework (objectScene) en gespecialiseerde scenes voor autokeuze, verkenning en rijden. Rendering gebeurt via één shader‑pipeline, en scenes leveren hun eigen objecten, HUD‑tekst en optionele lichtlogica.

## Functies
- Scene‑systeem met autokeuze, track‑verkenning en spline‑rijden.
- Skybox met PNG‑ondersteuning en een standaard effen kleur als fallback.
- Dynamische car‑loading in de car‑scene (auto’s worden ingelezen uit de assetmap).
- HUD‑tekst per scene.
- Mouse‑look met cursor lock en pauze‑modus.
- Toon‑stijl autoshading (effen vulling met outline).
- Spline‑gebaseerde track‑generatie en auto‑beweging in spline scene.
- Car‑keuze blijft behouden tussen scenes.
- Globale reset die alle scenes opnieuw opbouwt.

## Rubric‑items (waar te vinden)
Deze lijst koppelt rubric‑onderdelen aan concrete plekken in het project.

### Modellen & meshes
- **Unieke modellen**: `Project1/objects/Asseto corsa/**/3d.obj` en `Project1/objects/Eigen/Exports/*.obj`.
- **Primitive meshes**:
  - Ground plane: `objectScene::addGroundPlane` in `Project1/objectScene.cpp`.
  - Skybox cube: `Project1/skybox.cpp`.
  - Spline track mesh (samengevoegd): `splineScene::compileTrack` in `Project1/scenes/splineScene.cpp`.
- **Ontworpen OBJ’s**: custom exports in `Project1/objects/Eigen/Exports/` (bijv. `track_curb.obj`, `osso.obj`, `puntboom.obj`).

### Textures
- **Unieke textures**: `Project1/textures/*` en per‑car textures in `Project1/objects/Asseto corsa/**/wire.bmp` en `.png`.
- **Skybox texture**: `Project1/textures/Skybox.png`.

### Animaties
- **Spline‑rijden (auto beweegt over pad)**: `Project1/scenes/splineScene.cpp`.
- **Scene‑animaties/updates**: per‑frame updates via `preRenderCallback` in scenes.

### Fragment shading
- **Standaard lighting + texture**: `Project1/fragmentshader.frag`.
- **Toon‑shading voor auto**: `Project1/fragmentshader.frag` + `use_toon` in scenes.
- **Skybox/texture‑only path**: `Project1/fragmentshader.frag` + `use_texture_only`.

### Controls & interaction
- **Keybinds**: zie sectie “Keybinds” in deze README.
- **Scene‑flow (track → car → spline)**: `Project1/main.cpp`.

### Viewing / drone mode
- **Track scene drone/walk mode**: `Project1/scenes/trackScene.cpp` (toggle met `V`).

### Technisch ontwerp
- **Klassenstructuur**: `Project1/objectScene.h`, `Project1/sceneManager.h`, scene‑klassen in `Project1/scenes/`.
- **Documentatie**: `README.md` en HUD‑tekst per scene (`getHudLines`).

### Grafisch ontwerp
- **Scene‑opbouw**: `Project1/scenes/trackScene.cpp` en `Project1/scenes/splineScene.cpp`.

## Keybinds

### Algemeen
- `Esc`: Pauze aan/uit (cursor zichtbaar + pauze‑banner).
- `r`: Reset alle scenes.
- `n`: Volgende scene (debug/override).

### Track Scene (standaard start)
- `W/A/S/D`: Bewegen.
- Muis bewegen: Rondkijken.
- `V`: Wissel drone/walk mode.
- `Q/E`: Omhoog/omlaag in drone mode.
- `Space`: Springen in walk mode.
- `F`: Stap in auto (naar spline scene).
- `M`: Auto wisselen (naar car scene).

### Car Scene (selectie)
- `,` / `.`: Vorige/volgende auto.
- `C`: Wissel autokleur.
- `W/A/S/D`: Bewegen.
- `Q/E`: Omhoog/omlaag.
- `Enter`: Terug naar track scene.

### Spline Scene (rijden)
- `P`: Wissel camera‑view (first/third person).
- Muis bewegen: Rondkijken.

## Architectuur

### Scene‑systeem
- `sceneManager` beheert de lijst met scenes en regelt rendering, VBO‑binding en scene‑wissels.
- `objectScene` is de basis‑scene. Deze beheert camera, objecten en render flow, en biedt hooks voor:
  - `resetAndInit()` om de scene opnieuw op te bouwen.
  - `preRenderCallback()` voor animatie/updates per frame.
  - `getHudLines()` voor HUD‑tekst.
  - `getLightPosition()` om per scene de lichtpositie te overriden.

### Rendering‑pipeline
- Eén shader‑paar (`vertexshader.vert` / `fragmentshader.frag`).
- `object::render` bouwt MV‑matrices en zet materiaal/licht‑uniforms.
- Licht wordt per frame in view‑space gezet.
  - Standaard scenes gebruiken een directioneel licht.
  - Spline scene gebruikt een licht dat de auto volgt.

### Objecten en Materialen
- `objectData` bewaart mesh‑buffers, texture IDs en VBO‑binding.
- `Material` bevat licht‑parameters en render‑flags:
  - `use_texture_only` voor skybox en effen fills.
  - `use_toon` voor auto‑shading (effen fill + outline).

### Track en Spline
- `trackScene` is een kleine wereld met een statische track en objecten.
- `splineScene` genereert een spline‑track met `CatmullRom` en laat de auto over het pad rijden.
- In spline scene wordt de track als één object opgebouwd voor stabiliteit/prestatie.

## Notities
- PNG, BMP en DDS‑textures worden ondersteund. PNG gebruikt stb_image.
- Skybox laadt standaard `textures/Skybox.png`.
- Autokeuze en kleur zitten in `ApplicationData` en worden gedeeld tussen scenes.
