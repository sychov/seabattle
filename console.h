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

	void					ShowMessage(const char *message, MessageType status = MessageType::Normal);
	void					ShowEmptyRow();
	void					ShowError(const char *message);
	void					ShowComputerTurn(const char *message, int x, int y);
	int						AskForInt(const char *message);
	Coords					AskForCoords(const char *message);
	bool					AskForYesNo(const char *message);
	void					Pause();
	void					ClearConsole();

	// ----------------------------------------------------

private:

	// -----METHODS----- 

	void					DeleteLastConsoleRow();

	// -----MEMBER VARS----- 

	int						m_currentRow = 0;

	// -----CONST----- 

	static const char		*k_emptyWideString;

};

