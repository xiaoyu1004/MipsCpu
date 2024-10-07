
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    int rv = 0;
    FILE* in;
    FILE* out;
    int idx;
    int size;
    unsigned char rb;
    int firstValue;

    if (argc < 3) {
        printf("Bad args\nUSAGE: bin2coe input_file.bin output_file.coe\n\n");
        return 1;
    }

    in = fopen(argv[1], "rb");
    if (!in) {
        printf("ERROR: can't open input file: \"%s\"\n", argv[1]);
        return 1;
    }

    out = fopen(argv[2], "w");
    if (!out) {
        fclose(in);
        printf("ERROR: can't create output file: \"%s\"\n", argv[2]);
        return 1;
    }

    idx = 0;
    fseek(in, 0L, SEEK_END);
    size = ftell(in);
    fseek(in, 0L, SEEK_SET);

    fprintf(out, "memory_initialization_radix=16;\nmemory_initialization_vector=");

    firstValue = 1;

    while (!feof(in)) {
        rb = fgetc(in);

        if (feof(in)) break;

        if (firstValue) {
            firstValue = 0;
        } else {
            fprintf(out, ", ");
        }

        fprintf(out, "%02x", rb);
    }

    fprintf(out, ";\n");

    fclose(in);
    fclose(out);

    // printf("Conversion done. Saved %d bytes\n", size);

    return rv;
}