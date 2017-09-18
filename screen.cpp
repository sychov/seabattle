//
//   Some functions for text-mode output with color and positioning 
// 
///////////////////////////////////////////////////////////////////////////////


#include "screen.h"
#include <windows.h>
#include <iostream>


static COORD coord;


namespace seabattle_screen {

	// Move text cursor to (x,y) position.
	void GotoXY(int x, int y)
	{
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}


	// Set foreground and background colors.
	void SetColor(Colors fg, Colors bg)
	{
		unsigned char sheme = static_cast <unsigned int> (fg) + (static_cast <unsigned int> (bg) << 4);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sheme);
	}


	// Print single character at specific (x,y) position with specific foreground and background colors.
	void PrintChar(int x, int y, char character, Colors fg, Colors bg)
	{
		GotoXY(x, y);
		SetColor(fg, bg);
		std::cout << character;
	}

	// Print C-string at specific (x,y) position with specific foreground and background colors.
	void PrintString(int x, int y, const char *str, Colors fg, Colors bg)
	{
		GotoXY(x, y);
		SetColor(fg, bg);
		std::cout << str;
	}

}