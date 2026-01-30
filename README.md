# Extending Conway’s Game of Life: A C++ Simulation Framework with Multi-Agent Rules and Performance Benchmarks
Final Year Project By Michael Franks

---
## About Project

This project extends Conway’s classic Game of Life into a modern C++ simulation framework that supports multi-agent 
rules, dynamic maps, and performance benchmarking. While the original Game of Life is built on a simple binary cell 
state (alive or dead), this framework introduces multiple cell types such as white, green, red, and blue each governed 
by its own customizable rule set. These agents can interact in diverse ways: protecting, hunting, hiding, or competing 
for survival, creating a richer ecosystem of emergent behaviors.

The system is designed with scalability and maintainability in mind. A modular architecture separates concerns into 
distinct components:

- Map System for chunk-based storage and boundary management
- Rule Engine for defining and applying dynamic cell behaviors
- Game Manager for orchestrating simulation flow
- UI Layer for real-time control and visualization
- Debugging Tools for inspecting maps, rules, and performance metrics

Beyond simulation, the framework integrates benchmarking utilities to measure efficiency across different rule sets 
and map sizes. Python scripts complement the C++ core by generating graphs and visual reports, enabling detailed 
analysis of performance and emergent dynamics.

Ultimately, the project aims to recreate the spirit of Conway’s Game of Life while pushing it into new territory:
a customizable, multi-agent ecosystem where diverse cell types coexist and compete. This opens the door to exploring 
complex adaptive systems, emergent behaviors, and performance trade-offs in large-scale simulations.

---
## Notes
- Update function | needs to generate new chunks for crossing boundaries **to fix**
- Create dynamic rule system
  - rules of white, green, red, blue
- Create dynamic Map system of viewing chunks
- create UI for controlling the game
- Implement benchmarking tools
- create Python scripts for generating grafts 

---
## Requirements
- Recreate Conway’s game of life rule set in C++ with accurate cell behaviour.
- Develop two distinct map systems for unlimited representing the simulation grid.
- Design and implement custom rule type such as Attackers, Defenders, and Workers.
- Enable export and import of user-created patterns and world states.
- Build a search system to detect and locate specific patterns within the simulation.
- Create an interactive map viewer for visualizing the world state.
- Integrate benchmarking tools to evaluate simulation performance.

# Installation & Build
This project uses CMake Presets to provide a consistent build experience across Linux and Windows.

todo