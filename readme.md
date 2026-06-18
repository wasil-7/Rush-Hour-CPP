# Rush Hour - Object-Oriented C++ Simulation

An interactive, real-time simulation game built with C++ and the CImg library. It demonstrates core Object-Oriented Programming (OOP) principles, dynamic memory management, and efficient rendering pipelines.

### Features

**1. Object-Oriented Architecture**
* **Encapsulation:** Strict separation of core simulation state from rendering and utility wrappers.
* **Modularity:** Clean class and function design for maintainability and scalability.

**2. Real-Time Rendering Pipeline**
* **Graphics Wrapper:** Utilizes raw graphics handling via the `CImg` library.
* **Execution Loop:** Continuously renders state changes with minimal latency.

**3. State Management**
* **Spatial Tracking:** Manages complex, real-time spatial data and entity updates.
* **Collision Logic:** Highly optimized computational loop to handle entity interactions and boundaries.

**4. Environment Automation**
* **Dependency Resolution:** Custom shell scripting to automate library installations for reproducible builds.

### Tech Stack

* **Core Logic:** C++ (OOP Architecture)
* **Graphics/Rendering:** CImg / OpenGL / X11
* **Environment Config:** Bash / Shell Scripting

### Setup Instructions (Linux / Ubuntu VM)

**Install Dependencies:**
Make the setup script executable and run it to install required X11 and OpenGL libraries:
```bash
chmod +x install-libraries.sh
./install-libraries.sh

--Structure--

rush-hour-cpp/
├── CImg.h                # Core graphics library wrapper
├── game.cpp              # Core simulation state and main execution loop
├── util.cpp              # Rendering and utility implementations
├── util.h                # Declarations for utilities and drawing functions
└── install-libraries.sh  # Ubuntu/Debian dependency setup script
