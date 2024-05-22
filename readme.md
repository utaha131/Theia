# Theia
A CPU Physically Based Renderer. The goal of this project is to learn to about all the fundamental theories and implementation of a physically based production level ray tracer as well as designing the program to be extensible for future purposes.

# Key Features
- Spectral Rendering
- Physically Based Rendering
- Path Tracing
- Extendable Interface
# Implementation Status
A list of features or parts of the program in progress of to be implemented.
Some parts use PBRT-V4 source code test cases to validate the parts of the program.
| Feature               | Description | Status       |
| :---                  |    :---:    |          :---: |
| Math Library          | Vector Math, Random Numbers, Spherical Geomtery, Interval, etc. | In Progress |
| Radiometry Library    | Spectra, Color Spaces, etc. | In Progress  |
| Shape Interface       |             | In Progress  |
| Integrator Interface  | Rendering Algorithms (Path Tracing, Bidirectional Path Tracing, etc.)            | In Progress  |
| Sampling Interface    |             | In Progress  |
| Camera Interface      |  Various Camera Models and Film.           | In Progress  |
| Material Interface    |             | Not Started  |
| Light Interface    |             | Not Started  |
| USD Scene Loader      |             | Not Started  |

# References
- [Physically Based Rendering:From Theory To Implementation](https://pbr-book.org/)
- [RenderMan: An Advanced Path Tracing Architecture for Movie Rendering](https://graphics.pixar.com/library/RendermanTog2018/paper.pdf)