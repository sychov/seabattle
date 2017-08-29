// Screen output module

#include "screen.h"
#include <windows.h>
#include <iostream>


static COORD coord;


namespace seabattle_screen {

	void goto_xy(int x, int y)
	{
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	void setcolor(Colors fg, Colors bg)
	{
		unsigned char sheme = fg + (bg << 4);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sheme);
	}

	void outtext(int x, int y, char ch, Colors fg, Colors bg)
	{
		goto_xy(x, y);
		setcolor(fg, bg);
		std::cout << ch;
	}

	void outtext(int x, int y, char *string, Colors fg, Colors bg)
	{
		goto_xy(x, y);
		setcolor(fg, bg);
		std::cout << string;
	}
	
	void outtext(int x, int y, std::string &string)
	{
		goto_xy(x, y);
		setcolor(LightGrey, Black);
		std::cout << string;
	}

}