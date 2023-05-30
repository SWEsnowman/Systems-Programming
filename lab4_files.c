#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// This program creates the Fibonacci series with n terms
// It then writes it to a file in ASCII format and to a different file in binary format

// USAGE: lab4_files n output_ascii output_bin

int main (int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "Wrong number of command-line arguments\n");
		return -1;
	}
	
	int n = atoi(argv[1]);

	// TODO: complete the file opening code below
	FILE* out_ascii = fopen(argv[2], "w");
	if (out_ascii == NULL) {
		printf("%s", "The ASCII output file cannot be opened!");
		exit(-1);
	}

	FILE* out_bin = fopen(argv[3], "w");
	if (out_bin == NULL) {
		printf("%s", "The binary output file cannot be opened!");
		exit(-1);
	}
	// TODO: compute the first n terms of the Fibonacci series
	// no need to use recursion or store the entire sequence 
	int f1 =1, f2 =1;

	// TODO: write first two terms to the files
	// ASCII file: a human-readable 32-bit integer and a newline character per line
	// binary file: a 32-bit integer for each number in the series
	fprintf(out_ascii, "%d\n%d\n",f1, f2);
	fwrite(&f1,sizeof(int),1,out_bin);
	fwrite(&f2,sizeof(int),1,out_bin);
	int count = 2;
	for (int i = 2; i < n; i++)
	{
		int tmp = f1 + f2;
		f1 = f2;
		f2 = tmp;
		count++;
		// TODO: write new number to files
		fprintf(out_ascii, "%d\n",tmp);
		fwrite(&tmp,sizeof(int),1,out_bin);
	}

	// TODO: close files
	fclose(out_ascii);
	fclose(out_bin);
	// TODO: compute the size of the two files in bytes
	// print the sizes in stdout
	//printf("The size of the ascii file: %d", count);
	//printf("The size of the binary file: %d", count*sizeof(int));

	return 0;
}

