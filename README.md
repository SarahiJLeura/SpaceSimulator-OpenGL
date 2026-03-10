# Space-Simulator-OpenGL

Final project for Computer Graphics — the goal of this project is to understand and implement the general pipeline of computer graphics, which includes **modeling**, **animation (manipulation)**, **rendering**, and **rasterization** of 2D and 3D objects using OpenGL and FreeGLUT.
> This project was built using **Visual Studio** and **FreeGLUT 3.0.0.v140**

## Features
- All objects are rendered **without textures**, using only **RGB colors**
- Animate at least one object along a **Bezier curve** (parabolic motion)
- Display **Frames Per Second (FPS)** directly on the main window (not the console), calculated manually (without built-in timing functions)
- This OpenGL simulation allows real-time interaction through keyboard input:
  - Toggle lighting on/off
  - Rotate objects around their local axes
  - Apply different shading models:
    - Flat shading
    - Gouraud shading
    - No shading
  - Move the camera using a custom `LookAt` implementation
- Object transformations:
  - Translation
  - Rotation


## Project Structure

`Space-Simulator-OpenGL/`
`│`
`├── proyectoGraf_Final/ # All class files and simulation resources`
`│ ├── BezierCurve.{h,cpp} # Bezier curve implementation (WIP)`
`│ ├── Camera.{h,cpp} # Camera movement and LookAt logic`
`│ ├── Face.{h,cpp} # Object faces (triangular faces)`
`│ ├── LightManagement.{h,cpp} # Light control and toggling`
`│ ├── Matrix4x4.{h,cpp} # Matrix operations (transformations, inverses, transposes)`
`│ ├── Model.{h,cpp} # Loads and parses .obj files`
`│ ├── Vertex.{h,cpp} # Vertex/vector operations (normalize, dot, cross)`
`│ ├── MathUtils.{h,cpp} # Helper functions (centroid, normal calculations, etc.)`
`│ ├── simulation.obj # Collection of 3D models used in the simulation`
`│ └── main.cpp # Main application loop and rendering logic`
`│`
`├── proyectoGraf_Final.sln # Visual Studio solution file`
`└── README.md # Project documentation`

---

## 🛠 Requirements
- C++
- OpenGL
- [FreeGLUT 3.0.0](http://freeglut.sourceforge.net/)
- Visual Studio (tested on 2019+)
Make sure FreeGLUT is properly installed and linked in your Visual Studio environment.

---

## Simulation Preview

[🔗 [Link to Simulation Video](https://youtu.be/sh1x_f232Y8)](#)

---
## Known Limitations
- **BezierCurve class**: The class structure exists, but movement along the Bezier curve is currently hardcoded using provided translation points.
- **Perspective matrix**: Needs to be adjusted dynamically based on window resizing — currently leads to distortion when resizing the window.

---
## Future Improvements
- Apply **textures** to 3D models
- Fully implement the `BezierCurve` class to compute interpolated points dynamically
- Make the **perspective matrix adaptive** to window dimensions to maintain aspect ratio

---
## Author
**Patricia Sarahi Jimenez-Leura**  
December 2, 2024

## License

MIT License

This project is for academic and educational purposes.  

If you plan to use it commercially or extend it significantly, please provide credit.
