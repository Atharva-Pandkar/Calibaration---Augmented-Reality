# Calibration & Augmented Reality

Camera calibration and AR using OpenCV: detect a chessboard, calibrate the camera, and overlay virtual objects in the scene.

## Overview

This project implements:

- **Chessboard detection** — Find and refine corners in the camera feed.
- **Camera calibration** — Use multiple views of the chessboard (minimum 5) to compute intrinsic parameters and distortion.
- **Pose estimation** — Get camera position/orientation relative to the board.
- **Augmented reality** — Draw projected corners, 3D axes, and a virtual cube when the board is visible.

![image](https://github.com/Atharva-Pandkar/Calibaration---Augmented-Reality/assets/62322017/7ebcdbc0-79db-43c3-8918-fc562d043bf8)
![image](https://github.com/Atharva-Pandkar/Calibaration---Augmented-Reality/assets/62322017/2fc3ba24-98b8-4001-8a94-986fb65a6d15)

## Project Structure

| File | Description |
|------|-------------|
| `project4_main.cpp` | Tasks 1–3: chessboard detection, capture calibration images, run calibration, save `intrinsics.xml` |
| `Project4_part2.cpp` | Tasks 4–6: load intrinsics, solve PnP, project corners/axes, draw virtual cube |
| `Project4_part3.cpp` | Task 7: Harris corner detection demo |

**Note:** Each C++ file defines a different entry point (`mainasd`, `mainasda`, `main`). To run a file, set its function as `main` (or use a single `main` that dispatches).

## Requirements

- **OS:** Windows (tested on Windows 11); paths may need adjustment on Linux/macOS.
- **IDE:** e.g. Visual Studio Code.
- **OpenCV** with modules: `core`, `imgproc`, `highgui`, `calib3d`, `imgcodecs`.
- **C++ compiler** with C++11 or later.

## Build & Run

1. Install OpenCV and configure your build (include/lib paths).
2. Build the desired source file (only one “main” at a time).
3. **Calibration (part 1):** Run the main-program binary with an identifier:  
   `./program myid`  
   - `1` — save frame to `../data/<id>.<nnn>.png`  
   - `2` — add current frame to calibration set (need chessboard)  
   - `3` — run calibration and write `intrinsics.xml` (need ≥5 calibration images)  
   - `q` — quit
4. **AR (part 2):** Run the part2 binary. It reads `intrinsics.xml` and shows the chessboard with overlaid corners and cube. Press `q` to quit.
5. **Harris (part 3):** Run the part3 binary for the corner-detection demo.


## License

See [LICENSE](LICENSE).

## Author

Atharva Pandkar — *Calibration & Augmented Reality (PRCV Assignment 4)*
