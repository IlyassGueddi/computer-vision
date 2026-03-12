#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

            //Open a new file for Writing in Binary mode 
            FILE *out = fopen("output.ppm", "wb");

            if (out == NULL) {
                perror("Could not create output file");
                return 1;
            }

            // Write the PPM Header
            // P6 = Binary RGB, then Width, Height, and Max Color Value
            fprintf(out, "P6\n%d %d\n255\n", width, height);

            // Write the Array
            // '1' is the size of each element (char), 'size' is the total bytes
            fwrite(temp_data, 1, size, out);

            // Close the file to save it to disk
            fclose(out);

            printf("Image saved successfully as output.ppm\n");

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
            //Open a new file for Writing in Binary mode 
            FILE *out = fopen("output.ppm", "wb");

            if (out == NULL) {
                perror("Could not create output file");
                return 1;
            }

            // Write the PPM Header
            // P6 = Binary RGB, then Width, Height, and Max Color Value
            fprintf(out, "P6\n%d %d\n255\n", width, height);

            // Write the Array
            // '1' is the size of each element (char), 'size' is the total bytes
            fwrite(temp_data, 1, size, out);

            // Close the file to save it to disk
            fclose(out);

            printf("Image saved successfully as output.ppm\n");

        }else if(choice == 3){
            printf("Blur value(1-3): ");
            scanf("%d", &blurV);

            switch(blurV) {
                case 1:
                    // declaring the kernal for blur
                    float blur1_ker[3][3] = {{0.0625,0.125,0.0625},{0.125,0.25,0.125},{0.0625,0.125,0.0625}};

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

                                        int neighbor_idx2 = ((y + ky) * width + (x + kx)) * 3 + c;

                                        // Multiply by kernel weight
                                        sum += (float)data[neighbor_idx2] * blur1_ker[i][j];
                                    }
                                }
                                if (sum > 255) sum = 255;
                                if (sum < 0) sum = 0;

                                temp_data[index] = (unsigned char)sum;
                            }
                        }
                    }
                    break;
                    
                case 2:
                    
                    // declaring the kernal for blur
                    float blur2_ker[5][5] = {
                        {1.0/256, 4.0/256,  6.0/256,  4.0/256,  1.0/256},
                        {4.0/256, 16.0/256, 24.0/256, 16.0/256, 4.0/256},
                        {6.0/256, 24.0/256, 36.0/256, 24.0/256, 6.0/256},
                        {4.0/256, 16.0/256, 24.0/256, 16.0/256, 4.0/256},
                        {1.0/256, 4.0/256,  6.0/256,  4.0/256,  1.0/256}
                    };

                    for (int y = 2; y < height - 2; y++) {
                        for (int x = 2; x < width - 2; x++) {
                            // pixel (x, y)
                            // every pixel is 3 values R,G,B
                            for (int c = 0; c < 3; c++) {
                                
                                int index = (y * width + x) * 3 + c;
                                
                                float sum = 0.0;

                                for (int i = 0; i < 5; i++) {      // kernel Row 
                                    for (int j = 0; j < 5; j++) {  // Kernel Column 

                                        int ky = i - 2; 
                                        int kx = j - 2;

                                        int neighbor_idx2 = ((y + ky) * width + (x + kx)) * 3 + c;

                                        // Multiply by kernel weight
                                        sum += (float)data[neighbor_idx2] * blur2_ker[i][j];
                                    }
                                }
                                if (sum > 255.0f) sum = 255.0f;
                                if (sum < 0.0f) sum = 0.0f;

                                temp_data[index] = (unsigned char)sum;
                            }
                        }
                    }

                    break;
                case 3:
                    // declaring the kernel for blur
                    float blur3_ker[7][7] = {
                        {1,  6,  15,  20,  15,  6,  1},
                        {6,  36, 90,  120, 90,  36, 6},
                        {15, 90, 225, 300, 225, 90, 15},
                        {20, 120,300, 400, 300, 120,20},
                        {15, 90, 225, 300, 225, 90, 15},
                        {6,  36, 90,  120, 90,  36, 6},
                        {1,  6,  15,  20,  15,  6,  1}
                    };

                    for (int y = 3; y < height - 3; y++) {
                        for (int x = 3; x < width - 3; x++) {
                            // pixel (x, y)
                            // every pixel is 3 values R,G,B
                            for (int c = 0; c < 3; c++) {
                                
                                int index = (y * width + x) * 3 + c;
                                
                                float sum = 0.0;

                                for (int i = 0; i < 7; i++) {      // kernel Row 
                                    for (int j = 0; j < 7; j++) {  // Kernel Column 

                                        int ky = i - 3; 
                                        int kx = j - 3;

                                        int neighbor_idx2 = ((y + ky) * width + (x + kx)) * 3 + c;

                                        // Multiply by kernel weight
                                        sum += (float)data[neighbor_idx2] * (blur3_ker[i][j] / 4096.0f);
                                    }
                                }
                                if (sum > 255.0f) sum = 255.0f;
                                if (sum < 0.0f) sum = 0.0f;

                                temp_data[index] = (unsigned char)sum;
                            }
                        }
                    }
                    break;

            }
            
            //Open a new file for Writing in Binary mode 
            FILE *out = fopen("output.ppm", "wb");

            if (out == NULL) {
                perror("Could not create output file");
                return 1;
            }

            // Write the PPM Header
            // P6 = Binary RGB, then Width, Height, and Max Color Value
            fprintf(out, "P6\n%d %d\n255\n", width, height);

            // Write the Array
            // '1' is the size of each element (char), 'size' is the total bytes
            fwrite(temp_data, 1, size, out);

            // Close the file to save it to disk
            fclose(out);

            printf("Image saved successfully as output.ppm\n");

        }else{
            printf("invalid value;\n");
            break;
        }
    }
    


    free(data);
    free(temp_data);
    fclose(img);
}