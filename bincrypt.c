#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int  ENCRYPT = -1;
int  KEY     = 0;
char *INPUT  = NULL;
char *OUTPUT = NULL;
int  VERBOSE = 0;

void usage(char *program_name);

int main(int argc, char **argv) {
  
  int opt;
  while ((opt = getopt(argc, argv, "e:d:i:o:v")) != -1) {
    switch (opt) {
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
      case 'v':
        VERBOSE = 1;
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
    char buf[1]; // buf is each character
    int i = 0;
    while (!feof(infile)) {
      fread(buf, sizeof(buf), 1, infile);
      int ascii_code = (int)*buf + KEY + i;
      fwrite(&ascii_code, sizeof(ascii_code), 1 , outfile);

      // verbose output for encryption
      if (VERBOSE == 1) {
        int binary_num[32];
        for (int j = 31; j >= 0; j--) {
          if (ascii_code != 0) {
            binary_num[j] = ascii_code % 2;
            ascii_code /= 2;
          }
          else {
            binary_num[j] = 0;
          }
        }
        for (int j = 0; j < 32; j++) {
          printf("%d", binary_num[j]);
        }
      }
      i++;
    }
    if (VERBOSE == 1) {
      printf("\n");
    }    
    fclose(outfile);
    fclose(infile);
  }
  else if (ENCRYPT == 0) { 

    // file handling
    FILE *infile;
    infile = fopen(INPUT, "rb");
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
    int i = 0;
    int buf; // buf is each integer (32 bits)
    while (!feof(infile)) {
      fread(&buf, sizeof(buf), 1, infile);
      buf -= KEY + i;
      if (buf > 8) { // exclude ascii values 0-8
        fprintf(outfile, "%c", (char)buf);

        // verbose output for decryption
        if (VERBOSE == 1) {
          printf("%c", (char)buf);
        }
      }
      i++;
    }
    fclose(outfile);
    fclose(infile);
  } 
  else {
    printf("Error: cryptographic operation not specified\n");
    usage(argv[0]);
    return 1;
  }
  return 0; 
}

void usage(char *program_name) {
  printf("Usage: %s [-ed] [key] [-i] [input_file] [-o] [output_file] [-v]\n", program_name);
}
