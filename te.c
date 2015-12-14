#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFSIZE 128

int main(int argc, char *argv[])
{
	FILE *fp;
	char buffer[MAXBUFFSIZE];

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	while (fgets(buffer, MAXBUFFSIZE, fp)) {
		printf("%s", buffer);
	}

	exit(EXIT_SUCCESS);
}
