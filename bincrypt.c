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
    char buf[1];
    int i = 0;
    while (!feof(infile)) {
      fread(buf, sizeof(buf), 1, infile);
      int ascii_code = (int)*buf + KEY + i;
      fwrite(&ascii_code, sizeof(ascii_code),1 , outfile);
      i++;
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
    int buf;
    while (!feof(infile)) {
      fread(&buf, sizeof(buf), 1, infile);
      buf -= KEY + i;
      if (buf > 9) {
        printf("%c", (char)buf);
        fprintf(outfile, "%c", (char)buf);
      }
      i++;
    }

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
