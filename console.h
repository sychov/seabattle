// Game console module header

#include "screen.h"


struct Coords
{
	int x;
	int y;
};


class Seabattle_Console
{
private:
		
	// ATTRIBUTES

	int current_row = 0;
	std::string temp_string;
	
	// METHODS

	void clear_last_row();

public:

	// ENUM

	enum Message { MSG_Normal, MSG_Error, MSG_System };

	// METHODS

	void	show_message(char *message, Message status = MSG_Normal);
	void	show_message();
	void	show_error(char *message);
	void	show_computer_turn(char *message, int x, int y);
	int		ask_for_int(char * message);
	Coords	ask_for_coords(char * message);
	bool	ask_for_yesno(char *message);
	void	pause();
	void	cls();
};

