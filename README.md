# Drone Delivery Game

## Description

The Drone Delivery Game is an interactive 3D simulation where players control a drone to pick up and deliver packages to specified destinations. The game environment features dynamic obstacles, moving clouds, and a minimap to assist players in navigation. The objective is to safely transport packages without colliding with obstacles, and to score points by reaching the destination with the package.

The game uses OpenGL for rendering, and the camera is controllable to provide an immersive flying experience. The player can navigate the drone using the keyboard, and the camera's view adjusts dynamically based on drone movement and player inputs.

## Features

- **3D Graphics**: The game is rendered in a 3D environment using OpenGL, featuring terrain, clouds, obstacles, and drones.
- **Camera Controls**: Two cameras are available: one for controlling the drone and another for viewing the minimap.
- **Dynamic Obstacles**: Various objects like trees and buildings are scattered throughout the environment, posing challenges for drone navigation.
- **Package Delivery**: Players pick up packages from platforms and deliver them to designated destinations.
- **Score and Lives**: Players have a limited number of lives and earn points by successfully delivering packages.

## Game Controls

- **Movement**:
  - `W` - Move forward
  - `S` - Move backward
  - `A` - Move left
  - `D` - Move right
  - `Q` - Move down
  - `E` - Move up

- **Camera Controls**:
  - Right-click and drag to rotate the camera.

## Game Mechanics

1. **Drone Movement**: The drone can be controlled using the keyboard, and its position is updated every frame. The player can move the drone forward, backward, left, and right, as well as up and down.
2. **Collision Detection**: The drone must avoid obstacles such as trees, buildings, and other environmental objects. Colliding with an obstacle reduces the player's lives.
3. **Package Pickup**: When the drone is within a certain range of a package, it will automatically pick it up.
4. **Destination**: The drone must fly to a target destination marked on the ground and deliver the package to complete the task.
5. **Scoring**: Points are awarded for successfully delivering packages to the destination. The game ends when all lives are lost.

## Project Structure

- `tema2.h` & `tema2.cpp`: These files define the core functionality of the game, including drone movement, obstacle collision, package delivery, and camera control.
- `camera.h`: Contains the implementation for the camera, including functionality for setting the camera position and updating the view.
- `VertexShader.glsl` & `FragmentShader.glsl`: Shaders for rendering the game objects.
- `utils`: Contains utility files for math and transformations (e.g., `glm_utils.h`, `math_utils.h`).

## Future Enhancements

- **Multiplayer**: Add a feature where multiple players can control different drones and compete to deliver packages.
- **Improved AI**: Introduce AI-controlled drones or moving obstacles for added challenge.
- **Power-ups**: Implement power-ups that can temporarily increase the drone's speed or shield it from obstacles.
