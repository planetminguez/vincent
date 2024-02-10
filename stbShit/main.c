#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int main(int argc, char** argv){
	
	if (argc < 2){
		assert(argc >=1);
		fprintf(stderr, "USAGE: %s stb_image_chk <input>\n", argv[0]);
		fprintf(stderr, "ERROR: No input file provided\n");
		exit(1);
	}
	
	const char* input_filepath = argv[1];
	int x,y,n;
	unsigned char* pixels = stbi_load(input_filepath, &x, &y, &n, 0);
	
	printf("Filepath:  %s\n", input_filepath );
	printf("Width:     %d\n", x);
	printf("Height:    %d\n", y);
	printf("Comps:     %d\n", n);


	return 0;
}