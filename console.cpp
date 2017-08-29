// Game console

#include <cctype>
#include <iostream> 
#include <conio.h>

#include "console.h"

using namespace seabattle_screen;
using std::cin;
using std::getline;


// Write game messages.
void Seabattle_Console::show_message(char *message, Message status)
{
	Colors color = LightGrey;
	if (status == MSG_Error)
		color = LightRed;
	else if (status == MSG_System)
		color = LightCyan;
	outtext(5, MESSAGES_ROW + current_row, message, color);
	current_row++;
	if (current_row > MAX_MESSAGES_ROWS_COUNT)
	{
		pause();
		cls();
	}
	setcolor(LightGrey, Black);
}


// Write game messages.
void Seabattle_Console::show_message()
{
	current_row++;
	if (current_row > MAX_MESSAGES_ROWS_COUNT)
	{
		pause();
		cls();
	}
}


// Write game messages about computer turn.
void Seabattle_Console::show_computer_turn(char *message, int x, int y)
{
	std::string temp_string(message);
	char temp_prefix[11] = "(x:x) ";

	temp_prefix[3] = '0' + y;
	temp_prefix[1] = 'a' + x;

	outtext(5, MESSAGES_ROW + current_row, temp_prefix + temp_string);
	current_row++;
	if (current_row > MAX_MESSAGES_ROWS_COUNT)
	{
		pause();
		cls();
	}
	setcolor(LightGrey, Black);
}


// Show error message, pause and clear two last lines.
void Seabattle_Console::show_error(char *message)
{
	show_message(message, MSG_Error);
	pause();
	clear_last_row();
	clear_last_row();
}


//Get integer value from console.
int Seabattle_Console::ask_for_int(char *message)
{
	std::string::const_iterator it;
	while (true)
	{
		show_message(message, MSG_Normal);
		getline(cin, temp_string);
		it = temp_string.begin();
		while (it != temp_string.end() && std::isdigit(*it))
			it++;
		if (!temp_string.empty() && it == temp_string.end())
			break;
		show_error("Please, enter a valid number!");
	}
	return atoi(temp_string.c_str());
}


//Get Yes or No value from console.
//Return true, if Yes, else false
bool Seabattle_Console::ask_for_yesno(char *message)
{
	char temp_char;
	while (true)
	{
		show_message(message, MSG_Normal);
		getline(cin, temp_string);
		if (temp_string.size() != 1) 
			show_error("Please, enter 1 symbol Y/y or N/n!");
		else
		{
			temp_char = temp_string[0];
			if ((temp_char != 'Y') && (temp_char != 'y') && (temp_char != 'N') && (temp_char != 'n'))
				show_error("Please, enter one of: \"Y\", or \"y\", or \"N\", or \"n\"!");
			else if ((temp_char == 'Y') || (temp_char == 'y'))
				return true;
			else
				return false;
		}
	}
}


//Get coordinates of cell from console.
//Return Coords structure (int x and y attributes)
Coords Seabattle_Console::ask_for_coords(char *message)
{
	Coords result;
	std::string::const_iterator it;
	while (true)
	{
		show_message(message, MSG_Normal);
		getline(cin, temp_string);

		if ((temp_string.size() != 2) || (!isalpha(temp_string[0])) || (!isdigit(temp_string[1])))
			show_error("Please, enter a coordinates in format <letter><digit>, for example \"a8\"!");

		else
		{
			result.y = temp_string[1] - 48;

			if (isupper(temp_string[0]))
				result.x = temp_string[0] - 65;
			else
				result.x = temp_string[0] - 97;

			if (result.x > 9)
				show_error("Use only letters from a-j set!");
			else
				break;
		}
	}
	return result;
}


// Clear message screen. 
void Seabattle_Console::cls()
{
	for (int q = 0; q < MAX_MESSAGES_ROWS_COUNT; q++)
		outtext(0, MESSAGES_ROW + q, "                                                                                ");
	current_row = 0;
}


// Clear message screen. If row defined, it means number of row must be cleared. By default clear all.
void Seabattle_Console::clear_last_row()
{
	current_row--;
	outtext(0, MESSAGES_ROW + current_row, "                                                                               ");
}


// Pause.
void Seabattle_Console::pause()
{
	outtext(5, MESSAGES_ROW + current_row, "...press a key...", Grey);
	current_row++;
	_getch();
	clear_last_row();
}