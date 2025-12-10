#include <stdio.h>
#include <stdlib.h>

int encrypt_file(FILE *ifile, FILE *ofile, const int key, const int verbose) {
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

            for (int j = 0; j < 32; j++) {
                printf("%d", binary_num[j]);
            }
        }
        i++;
    }

    fclose(ifile);
    fclose(ofile);

    return 0;
}

int decrypt_file(FILE *ifile, FILE *ofile, const int key, const int verbose) {
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
    printf("Usage: %s {-e <key> | -d <key>} [-v] <input_file> <output_file>\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        usage(argv[0]);
        return 1;
    }

    int encrypt_flag = 0;
    int decrypt_flag = 0;
    int verbose_flag = 0;

    char *input_arg = NULL;
    char *output_arg = NULL;
    int file_count = 0;

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'e':
                encrypt_flag = atoi(argv[++i]);
                break;
            case 'd':
                decrypt_flag = atoi(argv[++i]);
                break;
            case 'v':
                verbose_flag = 1;
                break;
            }
        } else {
            if (file_count == 0) {
                input_arg = argv[i];
                file_count++;
            } else {
                output_arg = argv[i];
            }
        }
    }

    FILE *ifile;
    ifile = fopen(input_arg, "rb");
    if (ifile == NULL) {
        printf("Error: file %s not found\n", input_arg);
        return 1;
    }

    FILE *ofile;
    ofile = fopen(output_arg, "w");
    if (ofile == NULL) {
        printf("Error: unable to write to file %s\n", output_arg);
        return 1;
    }

    if (encrypt_flag != 0) {
        return encrypt_file(ifile, ofile, encrypt_flag, verbose_flag);
    } else if (decrypt_flag != 0) {
        return decrypt_file(ifile, ofile, decrypt_flag, verbose_flag);
    } else {
        printf("Error: operation not specified\n");
        usage(argv[0]);
        return 1;
    }
}
