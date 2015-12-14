#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUFFSIZE 128

typedef struct ln {
	char text[MAXBUFFSIZE];
	struct ln * next;
} line;

typedef struct f {
	line * head;
	line * tail;
	int lc;
	char filename[128];
} file;

file * create_file (const char * filename)
{
	file * f = malloc(sizeof(file *));
	f->head = NULL;
	f->tail = NULL;
	f->lc = 0;

	strcpy(f->filename, filename);

	return f;
}

void add_line (file * f, const char * lt)
{
	line * l = (line *) malloc(sizeof(line));

	strcpy(l->text, lt);
	l->next = NULL;

	if (f->head == NULL) {
		f->head = l;
		f->tail = l;
	} else {
		f->tail->next = l;
		f->tail = l;
	}

	(f->lc)++;
}

file * read_file (const char * filename)
{
	FILE *fp;
	char buffer[MAXBUFFSIZE];
	file * f = create_file(filename);

	puts("File created");

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open %s\n", filename);
		return NULL;
	}

	puts("File opened");

	while (fgets(buffer, MAXBUFFSIZE, fp)) {
		add_line(f, buffer);
	}

	return f;
}

void print_file (file * f)
{
	int c = 1;
	line * curr;

	if (f == NULL) {
	       fprintf(stderr, "Can't print null file\n");	       
	       return;
	}

	curr = f->head;
	while (curr != NULL) {
		printf("%i\t%s", c++, curr->text);
		curr = curr->next;
	}
}

int main(int argc, char *argv[])
{
	file * f = read_file(argv[1]);
	puts("File read");

	printf("%i\n", f->lc);
	print_file(f);


	exit(EXIT_SUCCESS);
}
