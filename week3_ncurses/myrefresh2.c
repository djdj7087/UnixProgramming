#include <unistd.h>
#include <ncurses.h>

int main(void)
{
	int c = 0;

	initscr();

	// Hide cursor
	curs_set(0);
	while (c < 1000) {
		mvprintw(0, 0, "%d", c++);
		// refresh();
		sleep(1);
	}

	endwin();
	return 0;
}
