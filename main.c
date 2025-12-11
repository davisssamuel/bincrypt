#include <stdio.h>
#include <stdlib.h>

void print_binary(unsigned int n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

void encrypt_file(FILE *ifile, FILE *ofile, const int key, const int verbose) {
    unsigned char buf; // buf is a single character
    int i = 0;

    while (fread(&buf, sizeof(buf), 1, ifile) == 1) {
        int encrypted_char = buf + i + key;
        fwrite(&encrypted_char, sizeof(encrypted_char), 1, ofile);
        if (verbose == 1) { print_binary(encrypted_char); }
        i++;
    }

    fclose(ifile);
    fclose(ofile);
}

void decrypt_file(FILE *ifile, FILE *ofile, const int key, const int verbose) {
    int buf; // buf is 32 bits
    int i = 0;

    while (fread(&buf, sizeof(buf), 1, ifile) == 1) {
        buf -= i + key;
        fprintf(ofile, "%c", (char) buf);
        if (verbose == 1) { printf("%c", (char) buf); }
        i++;
    }

    fclose(ifile);
    fclose(ofile);
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
    ifile = fopen(input_arg, "r");
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
        encrypt_file(ifile, ofile, encrypt_flag, verbose_flag);
    } else if (decrypt_flag != 0) {
        decrypt_file(ifile, ofile, decrypt_flag, verbose_flag);
    } else {
        printf("Error: operation not specified\n");
        usage(argv[0]);
        return 1;
    }

    return 0;
}
