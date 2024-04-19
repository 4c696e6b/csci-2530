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

const int MAXINPUTSIZE = 240;

int main()
{
	char canvas[MAXROWS][MAXCOLS];
	char canvasBackup[MAXROWS][MAXCOLS];
	bool exitFlag = true;
	char mainAction;
	char replaceBuffer[BUFFERSIZE];
	char mainBuffer[BUFFERSIZE];
	bool animate = false;
	char menu[] = "<E>dit / <M>ove / <R>eplace / <D>raw / <C>lear / <U>ndo / <L>oad / <S>ave / <Q>uit: ";

	// initlize the canvas
	initCanvas(canvas);

	while (exitFlag)
	{

		// displays the canvas, and creates a backup for the undo function
		displayCanvas(canvas);

		// displays the main menu
		cout << menu;
		cin.getline(mainBuffer, BUFFERSIZE);
		mainAction = mainBuffer[0];
		mainAction = toupper(mainAction);


		// switch statement to pick action
		switch (mainAction)
		{

			// edit canvas case
		case 'E':
			copyCanvas(canvasBackup, canvas);
			editCanvas(canvas);
			break;

			// move canvas case
		case 'M':
			int moveRows, moveCols;

			copyCanvas(canvasBackup, canvas);
			clearLine(MAXROWS + 1, sizeof(menu) + 20);

			// gets the number of colums to move
			cout << "enter the numbers of colums to move: ";
			cin >> moveCols;

			// loop to check for valid input
			while (cin.fail()) {

				// resets cin and clears the input buffer
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

				clearLine(MAXROWS + 1, sizeof(menu) + 20);
				cout << "error: not an integer, enter the number of colums to move: ";
				cin >> moveCols;
			}

			// gets the number of rows to move;
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "enter the numbers of rows to move: ";
			cin >> moveRows;

			// loop to check for valid input
			while (cin.fail()) {

				// resets cin and clears the input buffer
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

				clearLine(MAXROWS + 1, sizeof(menu) + 20);
				cout << "error: not an integer, enter the number of rows to move: ";
				cin >> moveRows;
			}

			// calls the actual move canvas function to move the canvas
			moveCanvas(canvas, moveRows, moveCols);
			break;

			// replace character case
		case 'R':

			char toReplace, replaceWith;

			copyCanvas(canvasBackup, canvas);

			// gets the character to replace
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "enter the character to replace: ";
			cin.getline(replaceBuffer, BUFFERSIZE);
			toReplace = replaceBuffer[0];

			// gets the character to replace the previous character with
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "enter the character to replace with: ";
			cin.getline(replaceBuffer, BUFFERSIZE);
			replaceWith = replaceBuffer[0];

			// calls the replace function to actually replace the characters in the canvas
			replace(canvas, toReplace, replaceWith);
			break;

			// draw case
		case 'D':

			// clears line and calls the menuTwo function to print the second menu
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			menuTwo(canvas, canvasBackup, animate);

			break;

			// clear case
		case 'C':

			copyCanvas(canvasBackup, canvas);
			initCanvas(canvas);
			break;


			// undo case
		case 'U':
			// calls the copyCanvas function to replace the main canvas with the backup taken at the start of the loop
			copyCanvas(canvas, canvasBackup);
			break;

			// load file to canvas case
		case 'L':

			copyCanvas(canvasBackup, canvas);
			loadCanvas(canvas);
			break;

			// save canvas to file case
		case 'S':

			saveCanvas(canvas);
			break;

			// exit case
		case 'Q':

			// this flag stops the loop
			exitFlag = false;
			break;
		}
	}
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
	clearLine(MAXROWS + 1, MAXCOLS + 40);
	cout << "Press ESC to Stop Editing";

	// Move cursor to row,col and then get
	// a single character from the keyboard

	gotoxy(row, col);
	input = _getch();


	while (input != ESC)
	{
		// If statemet that moves the position if the user inserts an arrow key.
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
	// Clear the screen
	system("cls");

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

void saveCanvas(char canvas[][MAXCOLS])
{
	// Create input array and file name array
	// initialize file name array with first part of file path
	char inputName[MAXINPUTSIZE];
	char fileName[FILENAMESIZE] = "SavedFiles/";
	clearLine(MAXROWS + 1, 120);
	cout << "Enter a name for the file: ";
	cin.getline(inputName, MAXINPUTSIZE);

	// Attach input to file name and add .txt 
	strncat(fileName, inputName, MAXINPUTSIZE);
	strncat(fileName, ".txt", MAXINPUTSIZE);


	// Opens a file with the given name 
	ofstream outfile;
	outfile.open(fileName);


	//if statement to check if there is a filend if it can be create 
	if (!outfile)
	{
		cerr << "ERROR! Output file could not be created." << endl;
		system("pause");
	}
	else
	{

		//copies canvas line by line to file 
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

		// gives confirmation 
		cout << "File Saved!\n";

		system("pause");
	}
}

void loadCanvas(char canvas[][MAXCOLS])
{
	// Create input array and file name array
	// initialize file name array with first part of file path
	char inputName[MAXINPUTSIZE];
	char fileName[FILENAMESIZE] = "SavedFiles/";
	clearLine(MAXROWS + 1, 120);
	cout << "Enter a name for the file: ";
	cin.getline(inputName, MAXINPUTSIZE);

	// Attach input to file name and add .txt 
	strncat(fileName, inputName, MAXINPUTSIZE);
	strncat(fileName, ".txt", MAXINPUTSIZE);


	// Opens a file with the given name 
	ifstream infile;
	infile.open(fileName);


	//if statement to check if there is a file it can load 
	if (!infile)
	{
		cerr << "ERROR! Input file could not be found" << endl;
		system("pause");
	}
	else
	{
		// clears canvas before loading a new one 
		initCanvas(canvas);

		//copies file char by char to canvas

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
				else
				{
					canvas[x][counter] = charCopy;
				}
				if (newLine == true)
				{
					continue;
				}
				else
				{
					charCopy = infile.get();
				}
				counter++;
			}

			// once the inner loop ends, and there are still characters past column 80, it will continue grabbing characters until it finds a newline or EOF
			while (charCopy != '\n' && !infile.eof())
			{
				charCopy = infile.get();
				if (charCopy == '\n')
				{
					newLine = true;
				}
			}
		}

		// closes file
		infile.close();

		// gives confirmation 
		cout << "File Loaded!" << endl;
		system("pause");
	}
}