# Image Filter Application Using C Language

## Overview
This program reads a BMP image, applies **Mean** and **Median** filtering techniques for noise reduction, and generates filtered output images. It supports three types of outputs:

1. **Mean Filtered Image**
2. **Median Filtered Image**
3. **Combination of Median followed by Mean Filtered Image**

The program demonstrates basic image processing by manipulating raw BMP pixel data.

---

## Features
- Reads a BMP file and extracts the header, info header, and pixel data.
- Applies two noise-reduction filters:
  - **Mean Filter**: Averages the surrounding pixels in a 3x3 kernel.
  - **Median Filter**: Finds the median of the surrounding pixels in a 3x3 kernel.
- Creates new BMP files with the processed pixel data.

---

## Prerequisites
- A BMP image file with the following requirements:
  - Uncompressed 24-bit BMP format.
- GCC or another C compiler.
- Basic understanding of C programming.

---

## Cloning the Repository
To clone this repository, use the following command:
```bash
git clone https://github.com/your-repo/image-filter](https://github.com/mathan0946/Image-Filter-Application-using-C-language -c
cd image-filter-c
```

---

## Requirements
This program does not require any external libraries. However, ensure that your environment has:
- GCC or an equivalent C compiler.
- A valid BMP image for processing.

---

## Compilation and Execution

### Compile the Program
```bash
gcc -o image_filter image_filter.c
```

### Execute the Program
```bash
./image_filter
```
Ensure that the input BMP file is present at the specified path in the code.

---

## Code Structure

### File: `image_filter.c`
The program consists of the following components:

#### Structures
- **BMPHeader**: Contains metadata for the BMP file, such as file size and data offset.
- **BMPInfoHeader**: Contains image properties like dimensions, bit depth, and compression.

#### Functions
1. **readBMPHeader**
   - Reads the BMP header from the file.
2. **readBMPInfoHeader**
   - Reads the BMP info header from the file.
3. **readPixelData**
   - Reads the pixel data starting from the header’s data offset.
4. **applyMeanFilter**
   - Applies a 3x3 mean filter to smooth the image.
5. **applyMedianFilter**
   - Applies a 3x3 median filter to remove noise.
6. **createNewBMP**
   - Writes a new BMP file with the modified pixel data.

#### Main Logic
1. Reads the BMP file from the specified path.
2. Allocates memory for pixel data and filtered data.
3. Applies Mean and Median filtering sequentially.
4. Saves three output BMP files:
   - **Mean Filtered Image**
   - **Median Filtered Image**
   - **Combination of Median and Mean Filtered Image**
5. Frees allocated memory and exits.

---

## Inputs and Outputs

### Input
- A BMP file (e.g., `charlie.bmp`) located at the specified path.

### Outputs
- **meanoutput.bmp**: Result after applying the Mean Filter.
- **medianoutput.bmp**: Result after applying the Median Filter.
- **meanANDmedian.bmp**: Result after applying the Median followed by the Mean Filter.

---

## How to Change Input and Output Paths
1. Modify the input file path in the `fopen` function in the `main` function:
   ```c
   FILE *file = fopen("your_path/image_name.bmp", "rb");
   ```
2. Modify the output file paths in the `createNewBMP` function calls:
   ```c
   createNewBMP("your_path/image_output_name.bmp", ...);
   ```

---

## Error Handling
- Checks if the input file is accessible:
  ```c
  if (!file) {
      printf("Unable to open file.\n");
      return 1;
  }
  ```
- Checks if the output file can be created:
  ```c
  if (!outputFile) {
      printf("Unable to create file %s.\n", outputFilename);
      exit(1);
  }
  ```

---

## Memory Management
- Dynamically allocates memory for pixel data and filtered outputs using `malloc`.
- Frees allocated memory at the end of the program to prevent memory leaks:
  ```c
  free(pixelData);
  free(meanFilteredData);
  free(medianFilteredData);
  free(meanANDmedianFilteredData);
  ```

---

## Limitations
- Only supports uncompressed 24-bit BMP files.
- Hardcoded paths for input and output files need manual changes.
- Fixed kernel size (3x3) for filtering operations.

---

## Future Enhancements
- Add support for other BMP formats (e.g., compressed, grayscale).
- Implement dynamic kernel sizes.
- Include command-line arguments for file paths and filter selection.
- Optimize median filter implementation for better performance.

---

## References
- [BMP File Format](https://en.wikipedia.org/wiki/BMP_file_format)
- C Programming Language Documentation

