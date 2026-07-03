# Fragments of Growth 🌟

An interactive, story-driven 2D narrative experience built using **C++** and **OpenGL (GLUT/GLEW)**. 

The game follows a character through a desaturated, grayscale landscape representing a world that has lost all color and emotion. As the player progresses through different environments and recovers symbolic "Fragments of Growth," vibrancy and full color are dynamically restored to the world.

---

## 👑 My Role: Team Leader & Technical Contributor

This project was a collaborative effort developed by a group of six student engineers. As the **Team Leader, Project Organizer, and Technical Documentation Lead**, I managed the overall project roadmap, oversaw the structural code integration across modules, and directed the final delivery assets. 

Alongside my leadership responsibilities, I took complete technical ownership of the following code modules, gameplay systems, and mechanics:

### 🛠️ Core Technical Implementations
* **Scene 6: The Interactive Library (Purpose):** I designed and built the project's most technically complex user interaction layer. Using mouse coordinate mapping converted from 2D screen space into OpenGL world space via `glutMouseFunc` and `glutMotionFunc`, I implemented a real-time mouse-picking and drag-and-drop sorting puzzle. Players must click and manually reorder scattered book objects into the proper alphabetical sequence (A through F) to unlock the scene.
* **Texture Mapping Engine (Cover & End Scenes):** I implemented the raw BMP texture loader using `GL_TEXTURE_2D`, `glPixelStorei`, and `glTexImage2D`. I configured the atmospheric rendering layer using `GL_BLEND` and `glBlendFunc` to ensure clean alpha-channel transparency, allowing high-fidelity pixel art backgrounds to overlay flawlessly with primitive geometric shapes.
* **Cover Scene Adjustments & Help UI Layer:** I edited and refined the shared Cover Scene to align with professional design standards. I built the interactive Help overlay system from scratch, designing a custom text alignment buffer using `glutBitmapCharacter` to display a clean, toggleable game-control instruction manual for the user.

### 📄 Architecture & Documentation Ownership
As the Documentation Lead, I personally managed and authored all secondary project delivery assets:
* **Project Technical Report:** Authored the comprehensive engineering report, detailing our abstract overview, architectural state-machine methodology, geometric transformation protocols (`glPushMatrix`/`glPopMatrix`), and system implementation results.
* **User Manual:** Designed and wrote the step-by-step layout guide, runtime setup parameters, dependencies reference, and complete interactive control map.
* **Project Presentation Materials:** Designed and organized the entire technical slide deck mapping out the code distribution framework, vertex primitive modeling logic, and the core graphic flow.

---

## 📁 Repository Structure

* `/src/FinalProject_2.cpp` — Main application source code.
* `/assets/` — Texture resources, including `cover_2.jpg` and `end_2.jpg`.
* `/docs/` — Full documentation suite (Technical Report, User Manual, and Presentation slides).

---

## 🎮 Full Game Flow & Environment Overview

To provide context for the global project, the narrative moves sequentially through the following thematic environments handled across our team's pipelines:
1. **Scene 1: Cover Screen** — Main entryway featuring an animated, rotating title fragment.
2. **Scene 2: Forest Path (Identity)** — Exploration puzzle requiring the user to isolate a true fragment via coordinate-based collision checking.
3. **Scene 3: The Desert (Courage)** — A sandstorm scene featuring branching path choice flags and rotation-based fall animations.
4. **Scene 4: The Beach (Clarity)** — Invisible trigger box boundary checking to unlock a hidden blue crystal matrix.
5. **Scene 5: The City (Connection)** — A sequential graph puzzle where touching urban nodes activates semi-transparent ambient lighting loops.
6. **Scene 6: The Library (Purpose)** — *[My Module]* Mouse-driven drag-and-drop book sorting logic.
7. **Scene 7: The Garden (Acceptance)** — Multi-object collection counter which dynamically resets the canvas buffer (`glClearColor`) to reveal blooming flower arrays.
8. **Scene 8: Final Light (Ending)** — *[My Module]* Final texture-mapped screen rendering a philosophical message on personal adaptability and growth.

---

## 🛠️ Setup & Dependencies

* **Development Environment:** Visual Studio 2022 (C++ Desktop Development Workload)
* **Graphics Framework:** OpenGL, linked via Core GLEW and FreeGLUT binaries.
* **Execution Note:** Ensure that the local relative paths for image assets are correctly pointed to the `/assets/` folder within your local environment setup.

---
*Collaborative project developed for the CS2206 Computer Graphics course at Umm Al-Qura University.*
