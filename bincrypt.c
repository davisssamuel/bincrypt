#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int  ENCRYPT;
int  KEY     = 0; 
char *INPUT  = NULL;
char *OUTPUT = NULL;
int  VERBOSE = 0;

void usage(char *program_name);

int to_binary(int decimal_num);

int main(int argc, char **argv) {
    
  int opt;
  while ((opt = getopt(argc, argv, "e:d:i:o:")) != -1) {
    switch (opt) {
      case 'v':
        VERBOSE = 1;
      case 'e':
        ENCRYPT = 1;
        KEY = atoi(optarg);
        break;
      case 'd':
        ENCRYPT = 0;
        KEY = atoi(optarg);
        break;
      case 'i':
        INPUT = optarg;
        break;
      case 'o':
        OUTPUT = optarg;
        break;
      default:
        usage(argv[0]);
        return 1;
    }
  }
  
  if (ENCRYPT == 1) {
    // file handling
    FILE *infile;
    infile = fopen(INPUT, "r");
    if (infile == NULL) {
      printf("Error: file %s not found\n", INPUT);
      return 1;
    }
    FILE *outfile;
    outfile = fopen(OUTPUT, "wb");
    if (outfile == NULL) {
      printf("Error: unable to write to file %s\n", OUTPUT);
      return 1;
    }

    // encryption
    char c[1];
    int i = 0;
    while (!feof(infile)) {
      fread(c, sizeof(c), 1, infile);
      int binary_num[32];
      int ascii_code = (int)*c + KEY + i;
      // fwrite(&c, sizeof(c),1 , outfile);
        for (int j = 31; j >= 0; j--) {
          if (ascii_code == 0) {
            binary_num[j] = 0;
          } 
          else {
            binary_num[j] = ascii_code % 2;
            ascii_code /= 2;
          }
        }
        for (int j = 0; j < 32; j++) {
          // if (binary_num[j] == '-') {
          //   return 1;
          // }
          fwrite(&binary_num[j], sizeof(binary_num[j]),1 , outfile);
          fprintf(outfile, "%d", binary_num[j]);
          printf("%d", binary_num[j]);
        }
      i++;
    }
    printf("\n");
    fclose(outfile);
    fclose(infile);
  }    
  else if (ENCRYPT == 0) { 
    // file handling
    FILE *infile;
    infile = fopen(INPUT, "r");
    if (infile == NULL) {
      printf("Error: file %s not found\n", INPUT);
      return 1;
    }
    FILE *outfile;
    outfile = fopen(OUTPUT, "w");
    if (outfile == NULL) {
      printf("Error: unable to write to file %s\n", OUTPUT);
    }    

    // decryption
    char in_char[1];
    int ascii_code = 0;
    int i = 0;
    while (!feof(infile)) {
      fread(in_char, sizeof(in_char), 1, infile);
      int int_num = *in_char - '0';
      ascii_code += int_num * pow(2, (31 - (i % 32)));
      if (i % 32 == 31) {
        char out_char = (char)(ascii_code - (KEY + i));
        fprintf(outfile, "%c", out_char);
        // fwrite(&out_char, sizeof(out_char),1 , outfile);
        printf("%c", out_char);
        ascii_code = 0;
      }
      i++;
    }
    printf("\n");
    fclose(outfile);
    fclose(infile);
  } 
  else {
    printf("Error: operation not specified\n");
    usage(argv[0]);
    return 1;
  }
  return 0; 
}

void usage(char *program_name) {
  printf("Usage: %s [-e] [key] [-i] [input_file] [-o] [output_file]\n", program_name);
}


    // encryption
    // int binary_num[16];
    // for (int i = 0; i < strlen(INPUT); i++) {
    //   int int_value = KEY + 1 + (int)INPUT[i];
    //   for (int j = 15; j >= 0; j--) {
    //     if (int_value == 0) {
    //       binary_num[j] = 0;
    //     } 
    //     else {
    //       binary_num[j] = int_value % 2;
    //       int_value /= 2;
    //     }
    //   }
    //   for (int j = 0; j < 16; j++) {
    //     printf("%i", binary_num[j]);
    //   }
    // }
    // printf("\n"); 
    // else {
    // int ascii_code = 0;
    // for (int i = 0; i < strlen(INPUT); i++) {
    //   int int_num = INPUT[i] - '0';
    //   ascii_code += int_num * pow(2, (15 - (i % 16)));
    //   if (i % 16 == 15) {
    //     printf("%c", (char)(ascii_code - (KEY + 1)));
    //     ascii_code = 0;
    //   }
    // }
    // printf("\n"); 
    // }
