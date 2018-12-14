# Terrain Sketching
Procedural Terrain Generation using Sketch for my Computer Graphics Course (CSE 528)

This is a project that is a work in progress. This will allow users to sketch the terrain in space, as if using Paint, and then generate a procedural terrain within those constraints. The noise that is defined in the silhouette terrain line will then be propagated throughout the landform to create a realistic landform.

If you are just interested in try out the application, download `Sketch_Terrain.exe` from the `Release` folder.

## Interface
The interface is divided into 2 modes which can be switched using **M**. These are:

##### Height Mode
Here the user can define the spine curves of the terrain. Any part of the curve below the base plane is drawn in red while anything above is drawn in green.

##### View Mode
The user sees the generated terrains.

All the modes provide the user to move around. For faster movement, use the arrow and the page up/down keys. For a more controlled movement use the w, a, s, d keys. To strafe up and down use the r and f keys.


## Controls
1. BASIC CONTROLS
- G: Toggle grids
- M: Toggle mode
- O: Increase water level
- L: Decrease water level
- P: Pop/Delete last terrain curve
2. MOVEMENT CONTROLS (FAST/SLOW)
- UP/W: Move ahead
- DOWN/S: Move back
- LEFT/A: Turn left
- RIGHT/D: Turn right
- PG UP/R: Climb up
- PG DN/F: Climb down
