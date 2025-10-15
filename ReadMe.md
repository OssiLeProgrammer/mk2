# 🌌 The Space Simulator

A simple physics-based space simulation written in C++ and compiled to WebAssembly using **Emscripten**.

---

## 🧱 Prerequisites

If you want to **build the project yourself**:

* **Emscripten SDK** (provides `emcmake`, `emcc`, etc.)
  → [https://emscripten.org/docs/getting_started/downloads.html](https://emscripten.org/docs/getting_started/downloads.html)

* **CMake** (for project configuration and build)

* **GLFW** and **GLAD** libraries (for OpenGL context and function loading)

  * A version of **GLM** (math library) is already included under `src/`.

Once those are installed and configured, building the project only requires two commands:

```bash
emcmake cmake -S . -B build
cmake --build build
```

After building, the important files will appear in the `build/` directory:

* `MyProject.js`
* `MyProject.wasm`

These two files together form the compiled WebAssembly module.

> 💡 **Note:** If you only want to *use* the simulator and not build it yourself, you don’t need Emscripten.
> You can simply use the existing `MyProject.js` and `MyProject.wasm` from the `build` folder.

---

## 🚀 The Space Simulator

The project is split into several logical parts under the `src/` directory:

| File              | Description                                                                                                                     |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------- |
| **Geometry.h**    | Defines circular geometry using vertex functions.                                                                               |
| **Physics.h**     | Contains the `Planet` and `World` classes. The world holds a vector of planets and updates their states using Newtonian forces. |
| **Camera.h**      | Defines the camera system for navigation (movement only, no orientation for browser privacy reasons).                           |
| **Shader Loader** | Handles loading and compiling GLSL shader programs.                                                                             |
| **Main.cpp**      | Entry point that runs the simulation. Uses `#ifndef __EMSCRIPTEN__` guards to support both native and WebAssembly builds.       |

The simulation computes gravitational interactions between planets by summing force vectors, dividing by mass, and updating velocities over time.

---

## 🧩 The JavaScript Module

The JavaScript interface to the simulation is defined by the generated files:

* **`MyProject.js`** — Contains the Emscripten runtime loader and glue code.
* **`MyProject.wasm`** — Contains the compiled C++ code as WebAssembly.

You can use the module like this in your web app:

```html
<script src="MyProject.js"></script>
<script>
  MyProject().then((Module) => {
    // The C++ simulation is now loaded and ready to use.
    console.log("Space Simulator module loaded!");
  });
</script>
```

---

## 🧠 Notes

* The camera cannot access device orientation APIs in Chrome due to privacy restrictions, so movement is limited to keyboard or manual input.
* For local testing, serve your files through a web server:                                 (you should remove the hashtag from the CMake file)

  ```bash
  python -m http.server
  ```

  Then open [http://localhost:8000/build/MyProject.html](http://localhost:8000/build/MyProject.html)

---