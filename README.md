# OpenGL Base

This project is used to aleviate the pain of setting up the boiler plate application code every time one wants to create something with OpenGL.
It includes the bare basics; an entry point, Window class and Application class. It also contains GLFW and "Glad 2.0" ( supporting OpenGL 4.6 ) which includes Vulkan headers for the adventurous.

Notes:
- The context version is hardcoded to version 4.5 in the window class, rather than 3.3 for its compatibility
- The application comes with incredibly simple logging functionality and log all glErrors will be printed due to the current setup
- The application has no rendering functionality; it was only created to handle context and window creation, and runtime management
