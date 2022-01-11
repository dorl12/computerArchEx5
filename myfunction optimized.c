// 313547085 Dor Levy
#include <stdbool.h>
#include <string.h>

typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

/*
 * myfunction - The main function of the program.
 * Inline optimization - all the calls to the functions inside the function myfunction
 * were replaced by their logic embedded into the function.
 */
void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName,
                char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/
	int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	int sharpKernel[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

    // Defining a veriable for the size of the image in order to multiply m and n only once.
    unsigned long imageSize = m * n;

    // Calculating the area of the image in order to allocate memory in the heap for the image.
    int sizeOfImage = imageSize * sizeof(pixel);

    // Allocating memory for the pixelsImg and backupOrg only once in myfunction in order to save place in the heap.
    pixel* pixelsImg = malloc(sizeOfImage);
    pixel* backupOrg = malloc(sizeOfImage);

    /*
     *  Using the saved word "register" before variables that are reused few times in order to tell the compiler
     *  to save those variables in a register.
     */

    // Declaring i and j in order to use them in the for loops during the function.
    register int i, j;

    // Declaring int variables in order to sum the pixels instead of using the struct pixel_sum.
    register int sumOfRed;
    register int sumOfGreen;
    register int sumOfBlue;

    // Declaring a pixel which is used to iterate through pixels of the image during the function.
    register pixel currentPixel;

    // Declaring an index which is used to save multiplications during calculating the index of the array.
    register int index = m;

	if (flag == '1') {

        // blur image - Inline implementation of the call doConvolution(image, 3, blurKernel, 9, false).

        /*
         * Using the function memcpy in order to copy image->data to backupOrg
         * instead of using the function charsToPixels.
         */
        memcpy(backupOrg, image->data, sizeOfImage);

        // Copying the frame of backupOrg to pixelsImg because it's not going to change, instead of using copyPixels.
        // Copying the first row.
        for (i = 0; i < m; i ++) {
            pixelsImg[i] = backupOrg[i];
        }

        // Copying the left col.
        for (i = 0; i < imageSize; i += m) {
            pixelsImg[i] = backupOrg[i];
        }

        // Copying the last row.
        for (i = imageSize - m; i < imageSize; i++) {
            pixelsImg[i] = backupOrg[i];
        }

        // Copying the right col.
        for (i = m - 1; i < imageSize; i += m){
            pixelsImg[i] = backupOrg[i];
        }

        // Inline implementation of the call smooth(m, backupOrg, pixelsImg, 3, blurKernel, 9, false).
        // kernelSize / 2 is equal to 1 because kernelSize = 3.
        for (i = 1 ; i < m - 1; i++) {
            for (j =  1 ; j < m - 1 ; j++) {

                // Inline implementation of the call to applyKernel.
                // Using int variables in order to sum the pixels instead of using the struct pixel_sum.
                sumOfRed = sumOfGreen = sumOfBlue = 0;

                // Calculating 9 iterations instead of using a nested loop.
                // Inline optimization - Calculating the sum of the pixels instead of using sum_pixels_by_weight.
                // sum_pixels_by_weight - Multiply 1 because all the blurKernel matrix is 1.
                currentPixel = backupOrg[index + j - m - 1];
                sumOfRed = currentPixel.red;
                sumOfGreen = currentPixel.green;
                sumOfBlue = currentPixel.blue;

                currentPixel = backupOrg[index + j - m];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;

                currentPixel = backupOrg[index+ j - m + 1];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;

                currentPixel = backupOrg[index + j - 1];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;

                currentPixel = backupOrg[index + j];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;

                currentPixel = backupOrg[index + j + 1];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;

                currentPixel = backupOrg[index + j + m - 1];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;

                currentPixel = backupOrg[index + j + m];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;

                currentPixel = backupOrg[index + j + m + 1];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;

                // assign kernel's result to pixel at [i,j]
                // Inline optimization of the function assign_sum_to_pixel(&current_pixel, sum, kernelScale).

                // divide by kernel's weight
                sumOfRed = sumOfRed / 9;
                sumOfGreen = sumOfGreen / 9;
                sumOfBlue = sumOfBlue / 9;

                // truncate each pixel's color values to match the range [0,255]
                // Inline optimization of the functions min, max
                currentPixel.red = (unsigned char) (((sumOfRed > 0 ? sumOfRed : 0)
                        < 255 ? (sumOfRed > 0 ? sumOfRed : 0) : 255));
                currentPixel.green = (unsigned char) (((sumOfGreen > 0 ? sumOfGreen : 0)
                        < 255 ? (sumOfGreen > 0 ? sumOfGreen : 0) : 255));
                currentPixel.blue = (unsigned char) (((sumOfBlue > 0 ? sumOfBlue : 0)
                        < 255 ? (sumOfBlue > 0 ? sumOfBlue : 0) : 255));

                /*
                 * implementation of the line "dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize,
                 * kernel, kernelScale, filter)" in smooth function.
                 */
                pixelsImg[index + j] =  currentPixel;
            }
            index += m;
        }

        /*
         * Using the function memcpy in order to copy pixelsImg to image->data
         * instead of using the function pixelsToChars.
         */
        memcpy(image->data, pixelsImg, sizeOfImage);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);

        /*
         * sharpen the resulting image - Inline implementation of the call
         * doConvolution(image, 3, sharpKernel, 1, false).
         */

        /*
         * Using the function memcpy in order to copy image->data to backupOrg
         * instead of using the function charsToPixels.
         */
        memcpy(backupOrg, image->data, sizeOfImage);

        // Copying the frame of backupOrg to pixelsImg because it's not going to change, instead of using copyPixels.
        // copying the first row.
        for (i = 0; i < m; i ++) {
            pixelsImg[i] = backupOrg[i];
        }

        // copying the left col.
        for (i = 0; i < imageSize; i += m) {
            pixelsImg[i] = backupOrg[i];
        }

        // copying the last row.
        for (i = imageSize - m; i < imageSize; i++) {
            pixelsImg[i] = backupOrg[i];
        }

        // copying the right col.
        for (i = m - 1; i < imageSize; i += m){
            pixelsImg[i] = backupOrg[i];
        }

        // Inline implementation of the call smooth(m, backupOrg, pixelsImg, 3, sharpKernel, 1, false).
        index = m;

        // kernelSize / 2 is equal to 1 because kernelSize = 3.
        for (i = 1 ; i < m - 1; i++) {
            for (j =  1 ; j < m - 1 ; j++) {

                // Inline implementation of the call to applyKernel.
                // Using int variables in order to sum the pixels instead of using the struct pixel_sum.
                sumOfRed = sumOfGreen = sumOfBlue = 0;

                // Calculating 9 iterations instead of using a nested loop.
                // Inline optimization - Calculating the sum of the pixels instead of using sum_pixels_by_weight.
                // sum_pixels_by_weight - Multiply -1 because all the sharpKernel matrix is -1 except from the middle.
                currentPixel = backupOrg[index + j - m - 1];
                sumOfRed = -currentPixel.red;
                sumOfGreen = -currentPixel.green;
                sumOfBlue = -currentPixel.blue;

                currentPixel = backupOrg[index + j - m];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[index + j - m + 1];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[index + j - 1];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                // Multiply 9 because the middle of the sharpKernel matrix is 9.
                currentPixel = backupOrg[index + j];
                sumOfRed += currentPixel.red * 9;
                sumOfGreen += currentPixel.green * 9;
                sumOfBlue += currentPixel.blue * 9;

                currentPixel = backupOrg[index + j + 1];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[index + j + m - 1];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[index + j + m];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[index + j + m + 1];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                // assign kernel's result to pixel at [i,j]
                // Inline optimization of the function assign_sum_to_pixel(&current_pixel, sum, kernelScale).
                // Don't need to divide because kernel's weight is 1

                // truncate each pixel's color values to match the range [0,255]
                // Inline optimization of the functions min, max
                currentPixel.red = (unsigned char) (((sumOfRed > 0 ? sumOfRed : 0)
                        < 255 ? (sumOfRed > 0 ? sumOfRed : 0) : 255));
                currentPixel.green = (unsigned char) (((sumOfGreen > 0 ? sumOfGreen : 0)
                        < 255 ? (sumOfGreen > 0 ? sumOfGreen : 0) : 255));
                currentPixel.blue = (unsigned char) (((sumOfBlue > 0 ? sumOfBlue : 0)
                        < 255 ? (sumOfBlue > 0 ? sumOfBlue : 0) : 255));

                /*
                 * implementation of the line "dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize,
                 * kernel, kernelScale, filter)" in smooth function.
                 */
                pixelsImg[index + j] =  currentPixel;
            }
            index += m;
        }

        /*
         * Using the function memcpy in order to copy pixelsImg to image->data
         * instead of using the function pixelsToChars.
         */
        memcpy(image->data, pixelsImg, sizeOfImage);

		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);
	} else {

        /*
         * apply extermum filtered kernel to blur image - Inline implementation of the call
         * doConvolution(image, 3, blurKernel, 7, true).
         */

        /*
         * Using the function memcpy in order to copy image->data to backupOrg
         * instead of using the function charsToPixels.
         */
        memcpy(backupOrg, image->data, sizeOfImage);

        // Copying the frame of backupOrg to pixelsImg because it's not going to change, instead of using copyPixels.
        // copying the first row.
        for (i = 0; i < m; i ++) {
            pixelsImg[i] = backupOrg[i];
        }

        // copying the left col.
        for (i = 0; i < imageSize; i += m) {
            pixelsImg[i] = backupOrg[i];
        }

        // copying the last row.
        for (i = imageSize - m; i < imageSize; i++) {
            pixelsImg[i] = backupOrg[i];
        }

        // copying the right col.
        for (i = m - 1; i < imageSize; i += m){
            pixelsImg[i] = backupOrg[i];
        }

        // Inline implementation of the call smooth(m, backupOrg, pixelsImg, 3, blurKernel, 7, true).
        index = m;

        /*
         *  Using the saved word "register" before variables that are reused few times in order to tell the compiler
         *  to save those variables in a register.
         */

        // Defining an int variable in order to sum the pixels.
        register int sumOfLoopPixel;
        register int min_row, min_col, max_row, max_col;

        // kernelSize / 2 is equal to 1 because kernelSize = 3.
        for (i = 1 ; i < m - 1; i++) {
            for (j =  1 ; j < m - 1 ; j++) {
                int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
                int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0

                // Inline implementation of the call to applyKernel.
                // Using int variables in order to sum the pixels instead of using the struct pixel_sum.
                sumOfRed = sumOfGreen = sumOfBlue = 0;

                // Calculating 9 iterations instead of using a nested loop.
                // Inline optimization - Calculating the sum of the pixels instead of using sum_pixels_by_weight.
                // sum_pixels_by_weight - Multiply 1 because all the blurKernel matrix is 1.

                /*
                 * Getting the if conditions of the filter condition into the iterations
                 * of the nested for loop in order to save memory access.
                 */

                currentPixel = backupOrg[index + j - m - 1];
                sumOfRed = currentPixel.red;
                sumOfGreen = currentPixel.green;
                sumOfBlue = currentPixel.blue;
                sumOfLoopPixel = sumOfRed + sumOfGreen + sumOfBlue;

                if (sumOfLoopPixel <= min_intensity) {
                    min_intensity = sumOfLoopPixel;
                    min_row = i - 1;
                    min_col = j - 1;
                }
                if (sumOfLoopPixel > max_intensity) {
                    max_intensity = sumOfLoopPixel;
                    max_row = i - 1;
                    max_col = j - 1;
                }

                currentPixel = backupOrg[index + j - m];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;
                sumOfLoopPixel = currentPixel.red + currentPixel.green + currentPixel.blue;

                if (sumOfLoopPixel <= min_intensity) {
                    min_intensity = sumOfLoopPixel;
                    min_row = i - 1;
                    min_col = j;
                }
                if (sumOfLoopPixel > max_intensity) {
                    max_intensity = sumOfLoopPixel;
                    max_row = i - 1;
                    max_col = j;
                }

                currentPixel = backupOrg[index + j - m + 1];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;
                sumOfLoopPixel = currentPixel.red + currentPixel.green + currentPixel.blue;

                if (sumOfLoopPixel <= min_intensity) {
                    min_intensity = sumOfLoopPixel;
                    min_row = i - 1;
                    min_col = j + 1;
                }
                if (sumOfLoopPixel > max_intensity) {
                    max_intensity = sumOfLoopPixel;
                    max_row = i - 1;
                    max_col = j + 1;
                }

                currentPixel = backupOrg[index + j - 1];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;
                sumOfLoopPixel = currentPixel.red + currentPixel.green + currentPixel.blue;

                if (sumOfLoopPixel <= min_intensity) {
                    min_intensity = sumOfLoopPixel;
                    min_row = i;
                    min_col = j - 1;
                }
                if (sumOfLoopPixel > max_intensity) {
                    max_intensity = sumOfLoopPixel;
                    max_row = i;
                    max_col = j - 1;
                }

                currentPixel = backupOrg[index + j];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;
                sumOfLoopPixel = currentPixel.red + currentPixel.green + currentPixel.blue;

                if (sumOfLoopPixel <= min_intensity) {
                    min_intensity = sumOfLoopPixel;
                    min_row = i;
                    min_col = j;
                }
                if (sumOfLoopPixel > max_intensity) {
                    max_intensity = sumOfLoopPixel;
                    max_row = i;
                    max_col = j;
                }

                currentPixel = backupOrg[index + j + 1];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;
                sumOfLoopPixel = currentPixel.red + currentPixel.green + currentPixel.blue;

                if (sumOfLoopPixel <= min_intensity) {
                    min_intensity = sumOfLoopPixel;
                    min_row = i;
                    min_col = j + 1;
                }
                if (sumOfLoopPixel > max_intensity) {
                    max_intensity = sumOfLoopPixel;
                    max_row = i;
                    max_col = j + 1;
                }

                currentPixel = backupOrg[index + j + m - 1];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;
                sumOfLoopPixel = currentPixel.red + currentPixel.green + currentPixel.blue;

                if (sumOfLoopPixel <= min_intensity) {
                    min_intensity = sumOfLoopPixel;
                    min_row = i + 1;
                    min_col = j - 1;
                }
                if (sumOfLoopPixel > max_intensity) {
                    max_intensity = sumOfLoopPixel;
                    max_row = i + 1;
                    max_col = j - 1;
                }

                currentPixel = backupOrg[index + j + m];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;
                sumOfLoopPixel = currentPixel.red + currentPixel.green + currentPixel.blue;

                if (sumOfLoopPixel <= min_intensity) {
                    min_intensity = sumOfLoopPixel;
                    min_row = i + 1;
                    min_col = j;
                }
                if (sumOfLoopPixel > max_intensity) {
                    max_intensity = sumOfLoopPixel;
                    max_row = i + 1;
                    max_col = j;
                }

                currentPixel = backupOrg[index + j + m + 1];
                sumOfRed += currentPixel.red;
                sumOfGreen += currentPixel.green;
                sumOfBlue += currentPixel.blue;
                sumOfLoopPixel = currentPixel.red + currentPixel.green + currentPixel.blue;

                if (sumOfLoopPixel <= min_intensity) {
                    min_intensity = sumOfLoopPixel;
                    min_row = i + 1;
                    min_col = j + 1;
                }
                if (sumOfLoopPixel > max_intensity) {
                    max_intensity = sumOfLoopPixel;
                    max_row = i + 1;
                    max_col = j + 1;
                }

                // filter out min and max
                // Inline optimization - Calculating the sum of the pixels instead of using sum_pixels_by_weight.
                currentPixel = backupOrg[((min_row)*(m)+(min_col))];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[((max_row)*(m)+(max_col))];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;


                // assign kernel's result to pixel at [i,j]
                // Inline optimization of the function assign_sum_to_pixel(&current_pixel, sum, kernelScale).

                // divide by kernel's weight
                sumOfRed = sumOfRed / 7;
                sumOfGreen = sumOfGreen / 7;
                sumOfBlue = sumOfBlue / 7;

                // truncate each pixel's color values to match the range [0,255]
                // Inline optimization of the functions min, max
                currentPixel.red = (unsigned char) (((sumOfRed > 0 ? sumOfRed : 0)
                        < 255 ? (sumOfRed > 0 ? sumOfRed : 0) : 255));
                currentPixel.green = (unsigned char) (((sumOfGreen > 0 ? sumOfGreen : 0)
                        < 255 ? (sumOfGreen > 0 ? sumOfGreen : 0) : 255));
                currentPixel.blue = (unsigned char) (((sumOfBlue > 0 ? sumOfBlue : 0)
                        < 255 ? (sumOfBlue > 0 ? sumOfBlue : 0) : 255));

                /*
                 * implementation of the line "dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize,
                 * kernel, kernelScale, filter)" in smooth function.
                 */
                pixelsImg[index + j] =  currentPixel;
            }
            index += m;
        }

        /*
         * Using the function memcpy in order to copy pixelsImg to image->data
         * instead of using the function pixelsToChars.
         */
        memcpy(image->data, pixelsImg, sizeOfImage);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

        /*
         * sharpen the resulting image - Inline implementation of the call
         * doConvolution(image, 3, sharpKernel, 1, false).
         */

        /*
         * Using the function memcpy in order to copy image->data to backupOrg
         * instead of using the function charsToPixels.
         */
        memcpy(backupOrg, image->data, sizeOfImage);

        // Copying the frame of backupOrg to pixelsImg because it's not going to change, instead of using copyPixels.
        // copying the first row.
        for (i = 0; i < m; i ++) {
            pixelsImg[i] = backupOrg[i];
        }

        // copying the left col.
        for (i = 0; i < imageSize; i += m) {
            pixelsImg[i] = backupOrg[i];
        }

        // copying the last row.
        for (i = imageSize - m; i < imageSize; i++) {
            pixelsImg[i] = backupOrg[i];
        }

        // copying the right col.
        for (i = m - 1; i < imageSize; i += m){
            pixelsImg[i] = backupOrg[i];
        }

        // Inline implementation of the call smooth(m, backupOrg, pixelsImg, 3, sharpKernel, 1, false).
        index = m;

        // kernelSize / 2 is equal to 1 because kernelSize = 3.
        for (i = 1 ; i < m - 1; i++) {
            for (j =  1 ; j < m - 1 ; j++) {

                // Inline implementation of the call to applyKernel.
                // Using int variables in order to sum the pixels instead of using the struct pixel_sum.
                sumOfRed = sumOfGreen = sumOfBlue = 0;

                // Calculating 9 iterations instead of using a nested loop.
                // Inline optimization - Calculating the sum of the pixels instead of using sum_pixels_by_weight.
                // sum_pixels_by_weight - Multiply -1 because all the sharpKernel matrix is -1 except from the middle.
                currentPixel = backupOrg[index + j - m - 1];
                sumOfRed = -currentPixel.red;
                sumOfGreen = -currentPixel.green;
                sumOfBlue = -currentPixel.blue;

                currentPixel = backupOrg[index + j - m];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[index + j - m + 1];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[index + j - 1];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                // Multiply 9 because the middle of the sharpKernel matrix is 9.
                currentPixel = backupOrg[index + j];
                sumOfRed += currentPixel.red * 9;
                sumOfGreen += currentPixel.green * 9;
                sumOfBlue += currentPixel.blue * 9;

                currentPixel = backupOrg[index + j + 1];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[index + j + m - 1];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[index + j + m];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                currentPixel = backupOrg[index + j + m + 1];
                sumOfRed -= currentPixel.red;
                sumOfGreen -= currentPixel.green;
                sumOfBlue -= currentPixel.blue;

                // assign kernel's result to pixel at [i,j]
                // Inline optimization of the function assign_sum_to_pixel(&current_pixel, sum, kernelScale).
                // Don't need to divide because kernel's weight is 1

                // truncate each pixel's color values to match the range [0,255]
                // Inline optimization of the functions min, max
                currentPixel.red = (unsigned char) (((sumOfRed > 0 ? sumOfRed : 0)
                        < 255 ? (sumOfRed > 0 ? sumOfRed : 0) : 255));
                currentPixel.green = (unsigned char) (((sumOfGreen > 0 ? sumOfGreen : 0)
                        < 255 ? (sumOfGreen > 0 ? sumOfGreen : 0) : 255));
                currentPixel.blue = (unsigned char) (((sumOfBlue > 0 ? sumOfBlue : 0)
                        < 255 ? (sumOfBlue > 0 ? sumOfBlue : 0) : 255));

                /*
                 * implementation of the line "dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize,
                 * kernel, kernelScale, filter)" in smooth function.
                 */
                pixelsImg[index + j] =  currentPixel;
            }
            index += m;
        }

        /*
         * Using the function memcpy in order to copy pixelsImg to image->data
         * instead of using the function pixelsToChars.
         */
        memcpy(image->data, pixelsImg, sizeOfImage);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}

    // Releasing memory allocated for the pixelsImg and backupOrg.
    free(pixelsImg);
    free(backupOrg);
}

