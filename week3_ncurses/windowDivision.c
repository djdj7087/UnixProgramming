#include <ncurses.h>
#include <unistd.h>

int main()
{
	WINDOW *GameWindow, *ChatWindow, *InfoWindow;

	initscr();
	
	// Create on the Standard screen
	// GameWindow = newwin(15, 60, 0, 0);

	// Create on the other WINDOW screen
	GameWindow = subwin(stdscr, 15, 60, 0, 0);
	ChatWindow = subwin(stdscr, 9, 60, 15, 0);
	InfoWindow = subwin(stdscr, 24, 20, 0, 60);

	// box(GameWindow, ':', '-');
	box(GameWindow, ACS_VLINE, ACS_HLINE);
	box(ChatWindow, ACS_VLINE, ACS_HLINE);
	box(InfoWindow, ACS_VLINE, ACS_HLINE);

	wrefresh(GameWindow);
	wrefresh(ChatWindow);
	wrefresh(InfoWindow);

	// do nothing for a while or insert your game, chatting, and other codes
	// as here!!

	sleep(10);

	// When you want to exit, we need to
	delwin(GameWindow);
	delwin(ChatWindow);
	delwin(InfoWindow);
	endwin();

	return 0;
}
