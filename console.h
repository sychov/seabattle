//
//   Header: Game Console class provides input/output in Seabattle game console
// 
/////////////////////////////////////////////////////////////////////////////////////


#pragma once


// =======================================  CONSOLE Class  ========================================= //

//Provides text input/output in Seabattle game console.
class Console
{
public:

	// -----STRUCT----- 

	struct Coords
	{
		int x, y;
	};

	//  -----ENUM----- 

	enum class MessageType	{ Normal, Error, System };

	//  -----METHODS----- 

	void					ShowMessage(char *message, MessageType status = MessageType::Normal);
	void					ShowEmptyRow();
	void					ShowError(char *message);
	void					ShowComputerTurn(char *message, int x, int y);
	int						AskForInt(char *message);
	Coords					AskForCoords(char *message);
	bool					AskForYesNo(char *message);
	void					Pause();
	void					ClearConsole();

	// ----------------------------------------------------

private:

	// -----METHODS----- 

	void					DeleteLastConsoleRow();

	// -----MEMBER VARS----- 

	int						m_currentRow = 0;
	std::string				m_tempString;

	// -----CONST----- 

	static const char		*k_emptyWideString;

};

