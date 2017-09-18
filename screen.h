//
//   Header: some functions for text-mode output with color and positioning 
// 
///////////////////////////////////////////////////////////////////////////////


#pragma once


namespace seabattle_screen {

	// -----ENUMS-----

	enum class Colors 
	{
		Black, Blue, Green, Cyan, Red, Magenta, Yellow, LightGrey,
		Grey, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, LightYellow, White
	};

	// -----FUNCTIONS-----

	void		PrintChar(int x, int y, char character, Colors fg, Colors bg);
	void		PrintString(int x, int y, const char *str, Colors fg, Colors bg);
	void		GotoXY(int x, int y);
	void		SetColor(Colors fg, Colors bg);

	// -----CONST-----

	const int	k_messagesStartRow = 27;
	const int	k_messagesMaxRows = 8;

}

