#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void image_save(const char *path, int width, int height, unsigned char *data);
void generate_gaussian_kernel(float *kernel, int k_size, float sigma);

int main() {
    FILE *img = fopen("img.ppm", "rb");
    if (img == NULL) return 1;

    char version[3]; // var to store version 
    int width ,height , max_val; // vars to store the other header values 

    // read and store all the header values 
    // the reason why we dothis is to make the cursor past the header
    fscanf(img, "%s %d %d %d", version, &width, &height, &max_val);
    fgetc(img); // consome the  last remining whitespace just before the first pixel red value

    //alloate memory to store the pixels
    int size = width * height * 3;
    unsigned char *data = (unsigned char *)malloc(size);

    //read the pixels into array
    fread(data, sizeof(unsigned char), size , img);

    int choice, brightness,v,index,blurV;
    unsigned char *temp_data = (unsigned char *)malloc(size);
    
    while(true){
        /*
        .ppm files are stored usually as [R1,G1,B1,R2,G2,B2,R3,....,Bn]
        */
        printf("1. Brightness\n2. Sharpen (Kernel)\n3. Blur (Kernel)\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1){
            printf("brightnes value(0-255): ");
            scanf("%d", &brightness);
            for (int i = 0 ; i < size; i += 3){
                // red
                int res = data[i] + brightness;
                if (res > 255) res = 255;
                else if (res < 0) res = 0;
    
                temp_data[i] = (unsigned char)res;

                //green
                res = data[i + 1] + brightness;
                if (res > 255) res = 255;
                else if (res < 0) res = 0;
    
                temp_data[i + 1] = (unsigned char)res;
                
                //blue
                res = data[i + 2] + brightness;
                if (res > 255) res = 255;
                else if (res < 0) res = 0;
    
                temp_data[i + 2] = (unsigned char)res;
                
            }

            image_save("output.ppm", width, height, temp_data);

        }else if(choice == 2){
            
            printf("sharpeness value: ");
            scanf("%d", &v);
            // declaring the kernal for sharpness
            float sharp_ker[3][3] = {{0,-v,0},{-v,(1+4*v),-v},{0,-v,0}};

            for (int y = 1; y < height - 1; y++) {
                for (int x = 1; x < width - 1; x++) {
                    // pixel (x, y)
                    // every pixel is 3 values R,G,B
                    for (int c = 0; c < 3; c++) {
                        
                        int index = (y * width + x) * 3 + c;
                        
                        float sum = 0.0;

                        for (int i = 0; i < 3; i++) {      // kernel Row 
                            for (int j = 0; j < 3; j++) {  // Kernel Column 

                                int ky = i - 1; 
                                int kx = j - 1;

                                int neighbor_idx = ((y + ky) * width + (x + kx)) * 3 + c;

                                // Multiply by kernel weight
                                sum += (float)data[neighbor_idx] * sharp_ker[i][j];
                            }
                        }
                        if (sum > 255) sum = 255;
                        if (sum < 0) sum = 0;

                        temp_data[index] = (unsigned char)sum;
                    }
                }
            }
            
            image_save("output.ppm", width, height, temp_data);

        }else if(choice == 3){
            int v; 
            float sigma = 1.0; 

            printf("Blur level (1: 3x3, 2: 5x5, 3: 7x7, 4: 9x9): ");
            scanf("%d", &blurV);

            switch(blurV) {
                case 1: v = 3; break;
                case 2: v = 5; break;
                case 3: v = 7; break;
                case 4: v = 9; break;
                default: v = 3; break;
            }

            int radius = v / 2;
            float *dynamic_ker = malloc(v * v * sizeof(float));

           
            generate_gaussian_kernel(dynamic_ker, v, sigma);

         
            for (int y = radius; y < height - radius; y++) {
                for (int x = radius; x < width - radius; x++) {
                    for (int c = 0; c < 3; c++) {
                        int index = (y * width + x) * 3 + c;
                        float pixel_sum = 0.0;

                        for (int i = 0; i < v; i++) {
                            for (int j = 0; j < v; j++) {
                                int ky = i - radius;
                                int kx = j - radius;
                                int neighbor_idx = ((y + ky) * width + (x + kx)) * 3 + c;
                                
                               
                                pixel_sum += (float)data[neighbor_idx] * dynamic_ker[i * v + j];
                            }
                        }

                        
                        if (pixel_sum > 255.0f) pixel_sum = 255.0f;
                        if (pixel_sum < 0.0f) pixel_sum = 0.0f;
                        
                        temp_data[index] = (unsigned char)pixel_sum;
                    }
                }
            }

            free(dynamic_ker);

            image_save("output.ppm", width, height, temp_data);

        }else{
            printf("invalid value;\n");
            break;
        }
    }
    


    free(data);
    free(temp_data);
    fclose(img);
}



void image_save(const char *path, int width, int height, unsigned char *temp_data) {
    int size = width * height * 3;
    
    // Use the 'path' variable passed into the function, not a hardcoded name
    FILE *out = fopen(path, "wb");

    if (out == NULL) {
        perror("Could not create output file");
        return; // Functions returning 'void' use 'return;' without a number
    }

    // Write the PPM Header
    fprintf(out, "P6\n%d %d\n255\n", width, height);

    // Write the Array
    fwrite(temp_data, 1, size, out);

    fclose(out);
    printf("Image saved successfully as %s\n", path);
}

void generate_gaussian_kernel(float *kernel, int k_size, float sigma) {
    int r = k_size / 2;
    float sum = 0.0;
    float s = 2.0 * sigma * sigma;

    
    for (int i = 0; i < k_size; i++) {
        for (int j = 0; j < k_size; j++) {
            int x = j - r; 
            int y = i - r; 
            
            float val = exp(-(x * x + y * y) / s) / (M_PI * s);
            kernel[i * k_size + j] = val;
            sum += val;
        }
    }

    for (int i = 0; i < k_size * k_size; i++) {
        kernel[i] /= sum;
    }
}