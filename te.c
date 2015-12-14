#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

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
	file * f = malloc(sizeof(file));
	f->head = NULL;
	f->tail = NULL;
	f->lc = 0;

	strcpy(f->filename, filename);

	return f;
}

void add_line (file * f, const char * lt)
{
	line * l = malloc(sizeof(line));

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

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open %s\n", filename);
		return NULL;
	}

	while (fgets(buffer, MAXBUFFSIZE, fp)) {
		add_line(f, buffer);
	}

	fclose(fp);

	return f;
}

void print_file (file * f, int start, int n)
{
	int c = 1;
	line * curr;

	if (f == NULL) {
	       fprintf(stderr, "Can't print null file\n");	       
	       return;
	}

	curr = f->head;

	while (c < start) {
		curr = curr->next;
		c++;
	}

	while (curr != NULL && start + n >= c) {
		printw("%i\t%s", c++, curr->text);
		curr = curr->next;
	}
}

void display_file (file * f)
{
	int top = 0;
	int q = 0;

	char c;

	initscr();
	cbreak();
	print_file(f, top, LINES);
	refresh();

	while (!q) {
		c = getch();
		switch (c) {
		case 'q':
			q = 1;
		case 'j':
			clear();
			refresh();
			print_file(f, ++top, LINES);
			refresh();
		case 'k':
			clear();
			refresh();
			print_file(f, --top, LINES);
			refresh();
		case 'c':
			clear();
		}
	}
	endwin();
}

static void print_usage (void)
{
	printf("te: Text editor\n");
	printf("Usage:\n");
	printf("\tte filename\n");
}

int main(int argc, char *argv[])
{
	file * f;
	
	if (argc != 2) {
		print_usage();
		exit(EXIT_SUCCESS);
	}
	
	f = read_file(argv[1]);

	display_file(f);

	exit(EXIT_SUCCESS);
}
