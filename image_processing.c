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

    int choice, brightness;
    unsigned char *temp_data = (unsigned char *)malloc(size);

    while(true){

        /*
        .ppm files are stored usually as [R1,G1,B1,R2,G2,B2,R3,....,Bn]
        */
        printf("1. Brightness\n2. Sharpen (Kernel)\n3. Blur (Kernel)\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1){
            printf("brightnes value: ");
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

            // 1. Open a new file for Writing in Binary mode ("wb")
            FILE *out = fopen("output.ppm", "wb");

            if (out == NULL) {
                perror("Could not create output file");
                return 1;
            }

            // 2. Write the PPM Header
            // P6 = Binary RGB, then Width, Height, and Max Color Value
            fprintf(out, "P6\n%d %d\n255\n", width, height);

            // 3. Write the Array
            // '1' is the size of each element (char), 'size' is the total bytes
            fwrite(temp_data, 1, size, out);

            // 4. Close the file to save it to disk
            fclose(out);

            printf("Image saved successfully as output.ppm\n");

        }else if(choice == 2){

        }else if(choice == 3){
            
        }else{
            printf("invalid value;\n");
            break;
        }
    }
    


    free(data);
    fclose(img);
}