// Link Parrish
// Tim Butts
// Christian Grandy
// Trey Roberts

/*
* This program allows display and editing of text art (also called ASCII art).
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cctype>
#include <windows.h>
#include <conio.h>
#include "Definitions.h"
using namespace std;

int main()
{
	Node* current = NULL;
	Node* tempNode = NULL;
	current = newCanvas();

	List undoList;
	List redoList;
	List clips;

	bool exitFlag = true;
	char mainAction;
	char replaceBuffer[BUFFERSIZE];
	char mainBuffer[BUFFERSIZE];
	bool animate = false;
	char menuLine2[] = "<E>dit / <M>ove / <R>eplace / <D>raw / <C>lear / <U>ndo / <L>oad / <S>ave / <Q>uit: ";
	bool saveLoadCheck;
	char inputName[MAXINPUTSIZE];
	char saveLoadBuffer[BUFFERSIZE];
	char saveLoadOption;


	while (exitFlag)
	{

		// displays the canvas
		system("cls");
		displayCanvas(current->item);

		char animateChar = animate ? 'Y' : 'N';
		cout << "<A>nimate: " << animateChar << " / <U>ndo: " << undoList.count;
		if (redoList.count > 0) cout << " / red<O>: " << redoList.count;
		cout << " / Cl<I>p: " << clips.count;
		if (clips.count > 0) cout << " / <P>lay";
		cout << endl;


		// displays the main menu
		cout << menuLine2;
		cin.getline(mainBuffer, BUFFERSIZE);
		mainAction = mainBuffer[0];
		mainAction = toupper(mainAction);


		// switch statement to pick action
		switch (mainAction)
		{

			// animate case
		case 'A':
		{
			// if animate is currently true set it to false and vice-versa
			animate = !animate;
			animateChar = animate ? 'Y' : 'N';
			break;
		}

		// undo case
		case 'U':
		{
			if (undoList.count > 0)
				restore(undoList, redoList, current);
			break;
		}

		// redo case
		case 'O':
		{
			if (redoList.count > 0)
				restore(redoList, undoList, current);
			break;
		}

		// clip case
		case 'I':
		{
			tempNode = newCanvas(current);
			addNode(clips, tempNode);
			tempNode = NULL;
			break;
		}

		// play case
		case 'P':
		{
			if (clips.count > 1)
				play(clips);
			break;
		}

		// edit canvas case
		case 'E':
		{
			addUndoState(undoList, redoList, current);
			editCanvas(current->item);
			break;
		}


		// move canvas case
		case 'M':
		{
			int moveRows, moveCols;

			addUndoState(undoList, redoList, current);
			clearLine(MAXROWS + 2, sizeof(menuLine2) + 20);
			clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);

			// gets the number of colums to move
			cout << "enter the numbers of colums to move: ";
			cin >> moveCols;

			// loop to check for valid input
			while (cin.fail()) 
			{

				// resets cin and clears the input buffer
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

				clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);
				cout << "error: not an integer, enter the number of colums to move: ";
				cin >> moveCols;
			}

			// gets the number of rows to move;
			clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);
			cout << "enter the numbers of rows to move: ";
			cin >> moveRows;

			// loop to check for valid input
			while (cin.fail()) 
			{

				// resets cin and clears the input buffer
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

				clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);
				cout << "error: not an integer, enter the number of rows to move: ";
				cin >> moveRows;
			}

			// calls the actual move canvas function to move the canvas
			moveCanvas(current->item, moveRows, moveCols);
			break;
		}

		// replace character case
		case 'R':
		{
			char toReplace, replaceWith;

			addUndoState(undoList, redoList, current);

			// gets the character to replace
			clearLine(MAXROWS + 2, sizeof(menuLine2) + 20);
			clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);
			cout << "enter the character to replace: ";
			cin.getline(replaceBuffer, BUFFERSIZE);
			toReplace = replaceBuffer[0];

			// gets the character to replace the previous character with
			clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);
			cout << "enter the character to replace with: ";
			cin.getline(replaceBuffer, BUFFERSIZE);
			replaceWith = replaceBuffer[0];

			// calls the replace function to actually replace the characters in the canvas
			replace(current->item, toReplace, replaceWith);
			break;

		}	// draw case
		case 'D':
		{
			// clears line and calls the menuTwo function to print the second menu
			clearLine(MAXROWS + 2, sizeof(menuLine2) + 20);
			clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);
			menuTwo(current, undoList, redoList, clips, animate);

			break;

		}	// clear case
		case 'C':
		{
			addUndoState(undoList, redoList, current);
			initCanvas(current->item);
			break;

		}	// load file to canvas case
		case 'L':
		{

			// Clears menu
			clearLine(MAXROWS + 2, sizeof(menuLine2) + 20);
			clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);
			gotoxy(MAXROWS + 1, 0);

			// Gets load option from user
			cout << "<C>anvas or <A>nimation: ";
			cin.getline(saveLoadBuffer, BUFFERSIZE);

			//Takes option and changes to uppercase
			saveLoadOption = saveLoadBuffer[0];
			saveLoadOption = toupper(saveLoadOption);

			//As long as option is valid, recieves a filename from user 
			if (saveLoadOption == 'C' || saveLoadOption == 'A')
			{
				clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);

				cout << "Enter a file name: ";
				cin.getline(inputName, MAXINPUTSIZE);

				//Attaches pathway to filename 
				char fileNameFull[FILENAMESIZE] = "SavedFiles/";
				strncat(fileNameFull, inputName, MAXINPUTSIZE);

				clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);

				if (saveLoadOption == 'C')
				{
					// Loads single file
					addUndoState(undoList, redoList, current);
					saveLoadCheck = loadCanvas(current->item, fileNameFull);
				}
				else if (saveLoadOption == 'A')
				{
					//Loads clip
					saveLoadCheck = loadClips(clips, fileNameFull);
				}

				// If load is successful, gives confirmation
				if (saveLoadCheck)
				{
					if (saveLoadOption == 'C')
						cout << "File Loaded!\n";

					else
						cout << "Clips Loaded!\n";
				}

				// If either option fails gives error
				else
					cout << "File failed to load.\n";

				system("pause");
			}
			break;
		}

			// save canvas to file case
		case 'S':
		{
			clearLine(MAXROWS + 2, sizeof(menuLine2) + 20);
			clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);
			gotoxy(MAXROWS + 1, 0);

			// Gets save option from user
			cout << "<C>anvas or <A>nimation: ";
			cin.getline(saveLoadBuffer, BUFFERSIZE);

			//Takes option and changes to uppercase
			saveLoadOption = saveLoadBuffer[0];
			saveLoadOption = toupper(saveLoadOption);

			//As long as option is valid, recieves a filename from user 
			if (saveLoadOption == 'C' || saveLoadOption == 'A')
			{
				clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);

				cout << "Enter a file name: ";
				cin.getline(inputName, MAXINPUTSIZE);

				char fileNameFull[FILENAMESIZE] = "SavedFiles/";
				// Attaches input to file and adds .txt to its save
				strncat(fileNameFull, inputName, MAXINPUTSIZE);

				clearLine(MAXROWS + 1, sizeof(menuLine2) + 20);

				if (saveLoadOption == 'C')
				{
					//Saves the canvas 
					addUndoState(undoList, redoList, current);
					saveLoadCheck = saveCanvas(current->item, fileNameFull);
				}
				// saves clips for animations
				else if (saveLoadOption == 'A')
					saveLoadCheck = saveClips(clips, fileNameFull);

				if (saveLoadCheck)
				{
					if (saveLoadOption == 'C')
						cout << "File saved!\n";

					else
						cout << "Clips saved!\n";
				}

				else
					cout << "File could not be saved .\n";

				system("pause");
			}

				break;

			}
			// exit case
		case 'Q':
		{
			// this flag stops the loop
			exitFlag = false;
			break;
		}
		}
	}
	delete current;
	deleteList(undoList);
	deleteList(redoList);
	deleteList(clips);
	return 0;
}


void gotoxy(short row, short col)
{
	COORD pos = { col, row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


void clearLine(int lineNum, int numOfChars)
{
	// Move cursor to the beginning of the specified line on the console
	gotoxy(lineNum, 0);

	// Write a specified number of spaces to overwrite characters
	for (int x = 0; x < numOfChars; x++)
		cout << " ";

	// Move cursor back to the beginning of the line
	gotoxy(lineNum, 0);
}


void replace(char canvas[][MAXCOLS], char oldCh, char newCh)
{

	// loop for each row
	for (int i = 0; i < MAXROWS; i++)
	{
		//iterate all the characters in the row
		for (int j = 0; j < MAXCOLS; j++)
		{
			//if any character is equal to oldCh
			//change it to newCh
			if (canvas[i][j] == oldCh)
				canvas[i][j] = newCh;
		}
	}
}


void editCanvas(char canvas[][MAXCOLS])
{
	char input;
	int row = 0, col = 0;

	// displays message about how to exit
	clearLine(MAXROWS + 2, MAXCOLS + 40);
	clearLine(MAXROWS + 1, MAXCOLS + 40);
	cout << "Press ESC to Stop Editing";

	// Move cursor to row,col and then get
	// a single character from the keyboard

	gotoxy(row, col);
	input = _getch();


	while (input != ESC)
	{
		// If statement that moves the position if the user inserts an arrow key.
		if (input == SPECIAL)
		{
			input = _getch();

			// Switch Statement that uses the arrows to move throughout the canvas.
			switch (input)
			{
				// case for moving up
			case UPARROW:
				if (row > 0)
				{
					row--;
				}
				break;

				// case for moving down
			case DOWNARROW:
				if (row < MAXROWS - 1)
				{
					row++;
				}
				break;

				// case for moving right
			case RIGHTARROW:
				if (col < MAXCOLS - 1)
				{
					col++;
				}
				break;

				// case for moving left
			case LEFTARROW:
				if (col > 0)
				{
					col--;
				}
				break;
			}
		}

		// If the key is a printable ASCII Character between 32 and 126. Prints the letter where the cursor is
		else if (32 <= input && input <= 126)
		{
			canvas[row][col] = input;
			printf("%c", input);
		}

		// If the key used is special key. 
		else if (input == '\0')
		{
			input = _getch();
		}

		gotoxy(row, col);
		input = _getch();
	}
}


void moveCanvas(char canvas[][MAXCOLS], int rowValue, int colValue)
{
	// delcares our calcualtion variables and creates a copy of our canvas
	int rowIndex, colIndex;
	char oldCanvas[MAXROWS][MAXCOLS];
	copyCanvas(oldCanvas, canvas);

	// clears our canvas so we can copy the moved lines onto it
	initCanvas(canvas);

	// loops through each character
	for (int i = 0; i < MAXROWS; i++)
	{
		rowIndex = i + rowValue;

		for (int j = 0; j < MAXCOLS; j++)
		{
			colIndex = j + colValue;

			// checks if we are outside our array
			if (rowIndex >= 0 && rowIndex < MAXROWS && colIndex >= 0 && colIndex < MAXCOLS)
			{
				// writes the old canvas to the new canvas with the row and column values added
				canvas[rowIndex][colIndex] = oldCanvas[i][j];
			}
		}
	}
}


void initCanvas(char canvas[][MAXCOLS])
{
	// nested for loop to set each character to a space
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			canvas[i][j] = ' ';
		}
	}
}


void displayCanvas(char canvas[][MAXCOLS])
{
	gotoxy(0, 0);

	// nested for loop to print each character of the canvas
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			cout << canvas[i][j];
		}

		// adds the right border to each line
		cout << "|\n";
	}

	// adds the bottom border
	for (int i = 0; i < MAXCOLS; i++) cout << "-";
	cout << endl;
}


void copyCanvas(char to[][MAXCOLS], char from[][MAXCOLS])
{
	// loop for each row
	for (int x = 0; x < MAXROWS; x++)
	{
		// loop for each column
		for (int y = 0; y < MAXCOLS; y++)
		{
			// copy each char from the from canvas to the to canvas
			to[x][y] = from[x][y];
		}
	}
}

bool saveCanvas(char canvas[][MAXCOLS], char filename[])
{
	// Create input array and file name array
	// initialize file name array with first part of file path

	strncat(filename, ".txt", MAXINPUTSIZE);


	// Opens a file with the given name 
	ofstream outfile;
	outfile.open(filename);


	//if statement to check if file can be created
	if (!outfile)
		return false;

	//copies canvas to outfile
	for (int x = 0; x < MAXROWS; x++)
	{
		for (int y = 0; y < MAXCOLS; y++)
		{
			outfile << canvas[x][y];
		}
		outfile << endl;
	}

	// closes file
	outfile.close();

	return true;
	
}

bool loadCanvas(char canvas[][MAXCOLS],char filename[])
{
	// takes filename and appends '.txt'	
	strncat(filename, ".txt", MAXINPUTSIZE);

	// Opens a file with the given name 
	ifstream infile;
	infile.open(filename);

	//if statement to check if there is a file it can load 
	if (!infile) return false;

	else
	{
		// clears canvas before loading a new one 
		initCanvas(canvas);

		// gets first character
		char charCopy = infile.get();

		// creates a variable to check for new line char
		bool newLine = false;
		int counter = 0;

		// first loop to run through all of the rows
		for (int x = 0; x < MAXROWS; x++)
		{
			// will only start the next line if it recieves a newline character
			// once it starts it will reset counters and newline variable
			if (newLine == true)
			{
				charCopy = infile.get();
				counter = 0;
				newLine = false;
			}

			// second loop to run through columns
			for (int y = 0; y <= MAXCOLS; y++)
			{
				// once counter reaches 80 it will check for a newline character and if it doesn't see it,
				// it will get the next character and end the inner loop and pass it to the while loop at the end of the outer loop
				if (counter > MAXCOLS - 1 && charCopy != '\n')
				{
					charCopy = infile.get();
					continue;
				}

				// checks for a newline character before character 80 and if found will set newLine to true
				// if newline is true it will put spaces in the rest of the iterations of the inner loop ( the if statements)
				// if newline is false it will set the char in the file to its proper place in the canvas (the else statements)

				if (charCopy == '\n')
				{
					newLine = true;
				}

				if (newLine == true)
				{
					if (counter > MAXCOLS - 1)
					{
						continue;
					}
					canvas[x][counter] = ' ';
				}

				else canvas[x][counter] = charCopy;

				if (newLine == true) continue;

				else charCopy = infile.get();

				counter++;
			}

			// once the inner loop ends, and there are still characters past column 80, it will continue grabbing characters until it finds a newline or EOF
			while (charCopy != '\n' && !infile.eof())
			{
				charCopy = infile.get();
				if (charCopy == '\n') newLine = true;
			}
		}

		// closes file
		infile.close();

		return true;

	}
}