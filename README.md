# Photon Simulation & Pinky's Management System

This project was developed for the **Data Structures and Algorithms** course during the first semester of 2021 at the **Pontifical Catholic University of Chile**. The task focuses on applying core course concepts like **sorting**, **trees**, and **algorithmic strategies** to solve complex problems. The project is divided into two main parts, each with its own unique challenges and objectives.

## Project Overview

### 1. **Pinky's Management System**

In this part of the project, you help Pinky and DCCerebro by building a **Binary Search Tree (BST)** system to efficiently manage materials in a highly disorganized warehouse. Each material has several properties (e.g., index, conductiveness, hardness, and inatorness), and the system must allow Pinky to search, order, and destroy materials based on a given property.

Key operations include:

- **PATH**: Find the path to a specific material.
- **DEEP**: Determine the depth of a material in the tree.
- **ORDER**: Return all materials in sorted order.
- **DESTROY**: Destroy a sub-tree of materials.

### 2. **Photon Simulation**

This part involves simulating the behavior of photons in a house of mirrors. The photons interact with walls (segments) and change direction upon collision. The goal is to optimize this simulation using an efficient **Bounding Volume Hierarchy (BVH)** or **KD-Tree** structure to reduce the complexity of detecting collisions.

Key concepts:

- Use the BVH or KD-Tree to organize segments in 2D space.
- Simulate particle (photon) collisions with segments and track each interaction across frames.
  
## Objectives

- Implement efficient algorithms using **divide-and-conquer** and **recursive strategies**.
- Familiarize yourself with the importance of **computational complexity** in solving real-world problems.
- Design and manage data structures to achieve optimal performance.

## Requirements

To run the project, you need the following:

- **C compiler** (e.g., `gcc`)
- **Make utility** (for compilation)
- **GTK+3** for visualization (optional)

### Installation (for GTK+3)

Make sure GTK+3 is properly installed. You can refer to the official installation guide if you need assistance. The visualizer tool helps you debug and visualize the photon simulation.

## Compilation

Use the provided `Makefile` to compile the program. From the project directory, run:

```bash
make
```

This will generate three executables:

- `simulate`: For the Photon Simulation problem.
- `pinky`: For the Pinky's Management System problem.
- `visualizer`: To run the visualizer for the simulation

## Running the Program

### Pinky's Management System

To run the management system:

```bash
./pinky input.txt output.txt
```

- `input.txt`: A file containing material data and operations.
- `output.txt`: A file where the results of the operations will be stored.

### Photon Simulation

To run the photon simulation:

```bash
./simulate input.txt output.txt
```

- `input.txt`: A file containing the initial setup of photons and segments.
- `output.txt`: A file where the simulation results (collisions) will be reported.

## Input Format

### Pinky's Management System

The input consists of:

- A number indicating the criteria (0 for `index`, 1 for `conductiveness`, 2 for `hardness`, 3 for `inatorness`).
- A list of N materials, each defined by `index`, `conductiveness`, `hardness`, and `inatorness`.
- A list of S queries, such as `PATH`, `DEEP`, `DESTROY`, and `ORDER`.

#### Example

```sql
2
2
51 414 23 104
24 1 32 52
4
PATH 10
DEEP 104
DESTROY 23
ORDER
```

### Photon Simulation

The input specifies:

- The number of frames for the simulation.
- The initial positions, velocities of photons, and the segments representing walls in the house of mirrors.
- he input is processed by the `simulate_init_from_file(char* input_file, bool visualize)` function.

## Output Format

### Pinky's Management System

The output will contain the results of each query in the input file. For example, the output of a `PATH` query might look like this:

```sql
3 6 4
```

Or if a node is not found, it may include an `X`:

```sql
3 6 10 7 X
```

### Photon Simulation

The output will log each collision detected per frame, based on photon interactions with walls.

## Additional Considerations

- Time Complexity: The photon simulation should optimize collision detection with a complexity of `O(F * P * log(S))`, where `F` is the number of frames, `P` is the number of particles, and `S` is the number of segments.
- Visualization: Use the visualizer executable for debugging and visually inspecting the simulation's progress.
