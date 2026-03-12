# Image-Processor (Low-Level Image Processing)

A lightweight image processing tool written in **C** from scratch. This project focuses on understanding how image data is structured in memory and applying mathematical kernels without external libraries (like OpenCV).

## 🚀 Overview
The goal of this project is to implement fundamental Image Processing algorithms by directly manipulating the pixel buffer of `.ppm` (P6) files. 

### Features implemented:
* **Brightness Adjustment:** Linear manipulation of RGB channels.
* **Sharpening Filter:** Implementing 2D Convolution using a 3x3 kernel.
* **Direct Memory Management:** Manual allocation and handling of pixel data in the Heap.

## 🛠️ How it Works
1. **PPM Parsing:** The program reads the binary header of a PPM file to extract dimensions.
2. **Buffer Allocation:** Pixels are stored in a 1D array as `[R1, G1, B1, R2, G2, B2...]`.
3. **Convolution Logic:** For filters like Sharpen, the program maps the 1D buffer to 2D coordinates `(x, y)` to calculate neighbor pixel weights.



## 🏗️ How to Run
```bash
gcc -o image_processing main.c
./image_processing