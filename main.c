#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int encrypt_file(const char *input_file, const char *output_file, const int key,
                 const int verbose) {
    // File handling
    FILE *ifile;
    ifile = fopen(input_file, "r");

    if (ifile == NULL) {
        printf("Error: file %s not found\n", input_file);
        return 1;
    }

    FILE *ofile;
    ofile = fopen(output_file, "wb");

    if (ofile == NULL) {
        printf("Error: unable to write to file %s\n", output_file);
        return 1;
    }

    // Encryption
    char buf[1]; // buf is each character
    int i = 0;

    while (!feof(ifile)) {
        fread(buf, sizeof(buf), 1, ifile);
        int ascii_code = (int) *buf + key + i;
        fwrite(&ascii_code, sizeof(ascii_code), 1, ofile);

        // Print encryption output to the console
        if (verbose == 1) {
            int binary_num[32];
            for (int j = 31; j >= 0; j--) {
                if (ascii_code != 0) {
                    binary_num[j] = ascii_code % 2;
                    ascii_code /= 2;
                } else {
                    binary_num[j] = 0;
                }
            }
            for (int j = 0; j < 32; j++) { printf("%d", binary_num[j]); }
        }
        i++;
    }

    if (verbose == 1) { printf("\n"); }

    fclose(ifile);
    fclose(ofile);

    return 0;
}

int decrypt_file(const char *input_file, const char *output_file, const int key,
                 const int verbose) {
    // File handling
    FILE *ifile;
    ifile = fopen(input_file, "rb");

    if (ifile == NULL) {
        printf("Error: file %s not found\n", input_file);
        return 1;
    }

    FILE *ofile;
    ofile = fopen(output_file, "w");

    if (ofile == NULL) {
        printf("Error: unable to write to file %s\n", output_file);
        return 1;
    }

    // Decryption
    int i = 0;
    int buf; // buf is each integer (32 bits)

    while (!feof(ifile)) {
        fread(&buf, sizeof(buf), 1, ifile);
        buf -= key + i;
        if (buf > 8) { // exclude ascii values 0-8
            fprintf(ofile, "%c", (char) buf);

            // Print decryption output to the console
            if (verbose == 1) { printf("%c", (char) buf); }
        }
        i++;
    }

    fclose(ifile);
    fclose(ofile);

    return 0;
}

void usage(char *program_name) {
    printf("Usage: %s [-ed] [key] [-i] [input_file] [-o] [output_file] [-v]\n", program_name);
}

int main(int argc, char **argv) {
    int encrypt_arg = 0;
    int decrypt_arg = 0;
    char *input_arg = NULL;
    char *output_arg = NULL;
    int verbose_flag = 0;

    int opt;
    while ((opt = getopt(argc, argv, "e:d:i:o:v")) != -1) {
        switch (opt) {
        case 'e':
            encrypt_arg = atoi(optarg);
            break;
        case 'd':
            decrypt_arg = atoi(optarg);
            break;
        case 'i':
            input_arg = optarg;
            break;
        case 'o':
            output_arg = optarg;
            break;
        case 'v':
            verbose_flag = 1;
            break;
        default:
            usage(argv[0]);
            return 1;
        }
    }

    if (encrypt_arg != 0) {
        return encrypt_file(input_arg, output_arg, encrypt_arg, verbose_flag);
    } else if (decrypt_arg != 0) {
        return decrypt_file(input_arg, output_arg, decrypt_arg, verbose_flag);
    } else {
        printf("Error: operation not specified\n");
        usage(argv[0]);
        return 1;
    }
}
