// Screen header

#include <string>


namespace seabattle_screen {

	enum Colors 
	{
		Black, Blue, Green, Cyan, Red, Magenta, Yellow, LightGrey,
		Grey, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, LightYellow, White
	};

	void outtext(int x, int y, char ch, Colors fg = White, Colors bg = Black);
	void outtext(int x, int y, char *string, Colors fg = White, Colors bg = Black);
	void outtext(int x, int y, std::string &string);
	void goto_xy(int x, int y);
	void setcolor(Colors fg, Colors bg);

	const int MESSAGES_ROW = 27;
	const int MAX_MESSAGES_ROWS_COUNT = 8;

}