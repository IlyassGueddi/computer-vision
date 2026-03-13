int v; 
printf("Blur level (1-4): ");
scanf("%d", &blurV);

// the len of the kernal depending in the user input
switch(blurV) {
    case 1: v = 3; break;
    case 2: v = 5; break;
    case 3: v = 7; break;
    case 4: v = 9; break;
    default: v = 3; break;
}

int radius = v / 2; // the  middle of the kernel

// generating the kernel
float *dynamic_ker = malloc(v * v * sizeof(float));
generate_gaussian_kernel(dynamic_ker, v, 1.0); 

// convolution logic
for (int y = radius; y < height - radius; y++) {
    for (int x = radius; x < width - radius; x++) {
        for (int c = 0; c < 3; c++) {
            
            int index = (y * width + x) * 3 + c;
            float pixel_sum = 0.0;

            for (int i = 0; i < v; i++) {      // kernel rows
                for (int j = 0; j < v; j++) {  // kernel columns

                    int ky = i - radius; 
                    int kx = j - radius;

                    int neighbor_idx = ((y + ky) * width + (x + kx)) * 3 + c;
                    
                    pixel_sum += (float)data[neighbor_idx] * dynamic_ker[i * v + j];
                }
            }

            if (pixel_sum > 255.0f) pixel_sum = 255.0f;
            if (pixel_sum < 0.0f)   pixel_sum = 0.0f;
            temp_data[index] = (unsigned char)pixel_sum;
        }
    }
}

free(dynamic_ker);
image_save("output.ppm", width, height, temp_data);