//
//   Game Console class provides input/output in Seabattle game console
// 
///////////////////////////////////////////////////////////////////////////////


#include <cctype>
#include <iostream> 
#include <conio.h>
#include <string>

#include "console.h"
#include "screen.h"

using namespace seabattle_screen;
using std::cin;
using std::getline;


// Print message to game console.
void Console::ShowMessage(const char *message, MessageType status /* = MessageType::Normal */)
{
	Colors color = Colors::LightGrey;
	if (status == MessageType::Error)
	{
		color = Colors::LightRed;
	}
	else if (status == MessageType::System)
	{
		color = Colors::LightCyan;
	}
		
	PrintString(5, k_messagesStartRow + m_currentRow, message, color, Colors::Black);
	m_currentRow++;
	if (m_currentRow > k_messagesMaxRows)
	{
		Pause();
		ClearConsole();
	}
	SetColor(Colors::LightGrey, Colors::Black);
}


// Print empty string to game console.
void Console::ShowEmptyRow()
{
	m_currentRow++;
	if (m_currentRow > k_messagesMaxRows)
	{
		Pause();
		ClearConsole();
	}
}


// Print special messages about computer turn to game console.
void Console::ShowComputerTurn(const char *message, int x, int y)
{
	std::string tempString(message);
	char temp_prefix[7] = "(x:x) ";
	temp_prefix[3] = '0' + y;
	temp_prefix[1] = 'a' + x;
	tempString = temp_prefix + tempString;

	PrintString(5, k_messagesStartRow + m_currentRow, tempString.c_str(), Colors::LightGrey, Colors::Black);
	m_currentRow++;
	if (m_currentRow > k_messagesMaxRows)
	{
		Pause();
		ClearConsole();
	}
	SetColor(Colors::LightGrey, Colors::Black);
}


// Print error message to game console, with pausing and removing error message after further pressing any key.
void Console::ShowError(const char *message)
{
	ShowMessage(message, MessageType::Error);
	Pause();
	DeleteLastConsoleRow();
	DeleteLastConsoleRow();
}


//Get integer value from console.
int Console::AskForInt(const char *message)
{
	std::string tempString;
	while (true)
	{
		ShowMessage(message, MessageType::Normal);
		getline(cin, tempString);
		auto it = tempString.begin();
		while (it != tempString.end() && std::isdigit(*it))
		{
			it++;
		}
		if (!tempString.empty() && it == tempString.end())
		{
			break;
		}
		ShowError("Please, enter a valid number!");
	}
	return std::stoi(tempString);
}


//Get Yes or No value from game console. Return true, if Yes, else false
bool Console::AskForYesNo(const char *message)
{
	char temp_char;
	std::string tempString;
	while (true)
	{
		ShowMessage(message, MessageType::Normal);
		getline(cin, tempString);
		if (tempString.size() != 1)
		{
			ShowError("Please, enter 1 symbol Y/y or N/n!");
		}
		else
		{
			temp_char = tempString[0];
			if ((temp_char == 'N') || (temp_char == 'n'))
			{
				return false;
			}
			else if ((temp_char == 'Y') || (temp_char == 'y'))
			{
				return true;
			}
			else
			{
				ShowError("Please, enter one of: \"Y\", or \"y\", or \"N\", or \"n\"!");
			}
		}
	}
}


//Get coordinates of cell from game console. Return Coords structure (int x and y attributes)
Console::Coords Console::AskForCoords(const char *message)
{
	Coords result;
	std::string tempString;
	while (true)
	{
		ShowMessage(message, MessageType::Normal);
		getline(cin, tempString);

		if ((tempString.size() != 2) || (!isalpha(tempString[0])) || (!isdigit(tempString[1])))
		{
			ShowError("Please, enter a coordinates in format <letter><digit>, for example \"a8\"!");
		}
		else
		{
			result.y = tempString[1] - 48;
			if (isupper(tempString[0]))
			{
				result.x = tempString[0] - 65;
			}
			else
			{
				result.x = tempString[0] - 97;
			}
			if (result.x > 9)
			{
				ShowError("Use only letters from a-j set!");
			}
			else
			{
				break;
			}
				
		}
	}
	return result;
}


// Clear game console. 
void Console::ClearConsole()
{
	for (int q = 0; q < k_messagesMaxRows; q++)
	{
		PrintString(0, k_messagesStartRow + q, k_emptyWideString, Colors::Black, Colors::Black);
	}
	m_currentRow = 0;
}


// Clear message screen. If row defined, it means number of row must be cleared. By default clear all.
void Console::DeleteLastConsoleRow()
{
	m_currentRow--;
	PrintString(0, k_messagesStartRow + m_currentRow, k_emptyWideString, Colors::Black, Colors::Black);
}


// Pause.
void Console::Pause()
{
	PrintString(5, k_messagesStartRow + m_currentRow, "...press a key...", Colors::Grey, Colors::Black);
	m_currentRow++;
	_getch();
	DeleteLastConsoleRow();
}

const char * Console::k_emptyWideString = "                                                                                ";

