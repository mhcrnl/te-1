#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define BUFFSIZE 509

typedef struct ln {
	char text[BUFFSIZE];
	struct ln * next;
	struct ln * prev;
} line;

typedef struct f {
	line * head;
	line * tail;
	int lc;
	char filename[BUFFSIZE];
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
		l->prev = f->tail;
		f->tail->next = l;
		f->tail = l;
	}

	(f->lc)++;
}

file * read_file (const char * filename)
{
	FILE *fp;
	char buffer[BUFFSIZE];
	file * f = create_file(filename);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open %s\n", filename);
		return NULL;
	}

	while (fgets(buffer, BUFFSIZE, fp)) {
		add_line(f, buffer);
	}

	fclose(fp);

	return f;
}

void print_lines (WINDOW * lwin, line * head, int n)
{
	int i = 1;
	line * curr = head;

	while (curr != NULL && n >= ++i) {
		wprintw(lwin, "%s", curr->text);
		curr = curr->next;
	}

	wrefresh(lwin);
	printf("%i\n", n);
}

/*
void display_file (file * f)
{
	int rows, cols;
	char c;
	line * top = f->head;

	initscr();
	noecho();

	getmaxyx(stdscr, rows, cols);

	print_lines(top, rows);

	while ((c = getch()) != EOF && c != 'q') {
		if (c == 'k' && top->prev != NULL)
			top = top->prev;
		else if (c == 'j' && top->next != NULL)
			top = top->next;
		else if (top->prev == NULL || top->next == NULL)
			printf("\a\n");

		clear();
		print_lines(top, rows);
	}
	endwin();
}
*/

WINDOW * init_disp (file * f)
{
	WINDOW * win;
	int rows, cols;

	initscr();
	noecho();
	refresh();

	getmaxyx(stdscr, rows, cols);
	win = newwin(rows, cols, 0, 0);

	print_lines(win, f->head, rows);

	return win;
}

void no_scroll (void)
{
	printf("\a\n");
}

line * scrollw (WINDOW * win, line * top, int up)
{
	int rows, cols;
	if (up && top->prev != NULL)
		top = top->prev;
	else if (!up && top->next != NULL)
		top = top->next;
	else
		no_scroll();

	getmaxyx(win, rows, cols);
	wclear(win);
	print_lines(win, top, rows);

	return top;
}

void main_loop (WINDOW * win, file * f)
{
	int q = 0;
	line * top = f->head;
	while (!q) {
		switch (getch()) {
		case EOF:
			q = 1;
			break;
		case 'q':
			q = 1;
			break;
		case 'k':
			top = scrollw(win, top, 1);
			break;
		case 'j':
			top = scrollw(win, top, 0);
			break;
		}
	}
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
	WINDOW * win;
	int rows, cols;
	
	if (argc != 2) {
		print_usage();
		exit(EXIT_SUCCESS);
	}
	
	f = read_file(argv[1]);
	
	win = init_disp(f);
	getmaxyx(win, rows, cols);
	printf("%i\n", rows);

	main_loop(win, f);

	endwin();

	exit(EXIT_SUCCESS);
}
