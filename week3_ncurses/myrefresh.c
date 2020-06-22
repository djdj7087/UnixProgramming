#include <unistd.h>
#include <ncurses.h>

int main(void)
{

	initscr();

	// Hide cursor
	curs_set(FALSE);
	mvprintw(0, 0, "Hello UNIX Programming World!!");
	refresh();
	sleep(1);

	endwin();
	return 0;
}
