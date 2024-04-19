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
using namespace std;

const int BUFFERSIZE = 20;
const int FILENAMESIZE = 255;
const int MAXINPUTSIZE = 240;
const int MAXROWS = 22;
const int MAXCOLS = 80;

// ASCII codes for special keys; for editing
const char ESC = 27;
const char LEFTARROW = 75;
const char UPARROW = 72;
const char RIGHTARROW = 77;
const char DOWNARROW = 80;
const char SPECIAL = (char)224;

/*
* Gets a filename from the user. If file can be opened for reading,
* this function loads the file's contents into canvas.
* File is a TXT file located in the SavedFiles folder.
* If file cannot be opened, error message is displayed and
* canvas is left unchanged.
*/
void loadCanvas(char canvas[][MAXCOLS]);

/*
* Gets a filename from the user. If file can be opened for writing,
* this function writes the canvas contents into the file.
* File is a TXT file located in the SavedFiles folder.
* If file cannot be opened, error message is displayed.
*/
void saveCanvas(char canvas[][MAXCOLS]);

/*
* Initializes canvas to contain all spaces.
*/
void initCanvas(char canvas[][MAXCOLS]);

/*
* Displays canvas contents on the screen, with a border
* around the right and bottom edges.
*/
void displayCanvas(char canvas[][MAXCOLS]);

/*
* Allows user to edit the canvas by moving the cursor around and
* entering characters. Editing continues until the ESC key is pressed.
*/
void editCanvas(char canvas[][MAXCOLS]);

/*
* Copies contents of the "from" canvas into the "to" canvas.
*/
void copyCanvas(char to[][MAXCOLS], char from[][MAXCOLS]);

/*
* Replaces all instances of a character in the canvas.
* oldCh is the character to be replaced.
* newCh character is the character to replace with.
*/
void replace(char canvas[][MAXCOLS], char oldCh, char newCh);

/*
* Shifts contents of the canvas by a specified number of rows and columns.
* rowValue is the number of rows by which to shift
*    positive numbers shift downward; negative numbers shift upward
* colValue is the number of rows by which to shift
*    positive numbers shift right; negative numbers shift left
*/
void moveCanvas(char canvas[][MAXCOLS], int rowValue, int colValue);

/*
* Clears a line on the output screen, then resets the cursor back to the
* beginning of this line.
* lineNum is the line number on the output screen to clear
* numOfChars is the number of characters to clear on this line
*/
void clearLine(int lineNum, int numOfChars);

/*
* Moves the cursor in the output window to a specified row and column.
* The next output produced by the program will begin at this position.
*/
void gotoxy(short row, short col);


int main()
{
	char canvas[MAXROWS][MAXCOLS];
	char canvasBackup[MAXROWS][MAXCOLS];
	bool exitFlag = true;
	char mainAction;
	char replaceBuffer[BUFFERSIZE];
	char mainBuffer[BUFFERSIZE];

	// initlize the canvas
	initCanvas(canvas);

	while (exitFlag)
	{

		// displays the canvas, and creates a backup for the undo function
		displayCanvas(canvas);

		// displays the main menu
		cout << "<E>dit / <M>ove / <R>eplace / <U>ndo / <L>oad / <S>ave / <Q>uit: ";
		cin.getline(mainBuffer, BUFFERSIZE);
		mainAction = mainBuffer[0];


		// switch statement to pick action
		switch (mainAction)
		{

			// edit canvas case
		case 'e':
		case 'E':
			copyCanvas(canvasBackup, canvas);
			editCanvas(canvas);
			break;

			// move canvas case
		case 'm':
		case 'M':
			int moveRows, moveCols;

			copyCanvas(canvasBackup, canvas);
			clearLine(MAXROWS + 1, 100);

			// gets the number of colums to move
			cout << "enter the numbers of colums to move: ";
			cin >> moveCols;

			// loop to check for valid input
			while (cin.fail()) {

				// resets cin and clears the input buffer
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

				clearLine(MAXROWS + 1, 100);
				cout << "error: not an integer, enter the number of colums to move: ";
				cin >> moveCols;
			}

			// gets the number of rows to move;
			clearLine(MAXROWS + 1, 80);
			cout << "enter the numbers of rows to move: ";
			cin >> moveRows;

			// loop to check for valid input
			while (cin.fail()) {

				// resets cin and clears the input buffer
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

				clearLine(MAXROWS + 1, 100);
				cout << "error: not an integer, enter the number of rows to move: ";
				cin >> moveRows;
			}

			// calls the actual move canvas function to move the canvas
			moveCanvas(canvas, moveRows, moveCols);
			break;

			// replace character case
		case 'r':
		case 'R':

			char toReplace, replaceWith;

			copyCanvas(canvasBackup, canvas);

			// gets the character to replace
			clearLine(MAXROWS + 1, 100);
			cout << "enter the character to replace: ";
			cin.getline(replaceBuffer, BUFFERSIZE);
			toReplace = replaceBuffer[0];

			// gets the character to replace the previous character with
			clearLine(MAXROWS + 1, 80);
			cout << "enter the character to replace with: ";
			cin.getline(replaceBuffer, BUFFERSIZE);
			replaceWith = replaceBuffer[0];

			// calls the replace function to actually replace the characters in the canvas
			replace(canvas, toReplace, replaceWith);
			break;

			// undo case
		case 'u':
		case 'U':

			// calls the copyCanvas function to replace the main canvas with the backup taken at the start of the loop
			copyCanvas(canvas, canvasBackup);
			break;

			// load file to canvas case
		case 'l':
		case 'L':
			copyCanvas(canvasBackup, canvas);
			loadCanvas(canvas);
			break;

			// save canvas to file case
		case 's':
		case 'S':
			saveCanvas(canvas);
			break;

			// exit case
		case 'q':
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
	for(int i = 0; i < MAXROWS; i++)
	{
		//iterate all the characters in the row
		for(int j = 0; j < MAXCOLS; j++)
		{
			//if any character is equal to oldCh
			//change it to newCh
			if(canvas[i][j] == oldCh)
				canvas[i][j] = newCh;
		}
	}
}


void editCanvas(char canvas[][MAXCOLS])
{
	char input;
	int row = 0, col = 0;

	// displays message about how to exit
	clearLine(MAXROWS + 1, 80);
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
	char oldCanvas[MAXROWS][MAXCOLS];
	copyCanvas(oldCanvas, canvas);

	// moving rows down 
	if (rowValue > 0)
	{

		// loop for how many rows to move
		for (int x = 0; x < rowValue; x++)
		{
			// loop to move each row, MAXROWS-1 is used because of how indexes work 
			for (int i = MAXROWS - 1; i > 0; i--)
			{
				// loop to move each character down a row
				for (int j = 0; j < MAXCOLS; j++)
				{
					canvas[i][j] = oldCanvas[i - 1][j];
				}
			}

			// replace the top row with spaces
			for (int i = 0; i < MAXCOLS; i++) canvas[x][i] = ' ';

			// creates a new copy to base the next move after
			copyCanvas(oldCanvas, canvas);
		}
	}

	// moving rows up
	else if (rowValue < 0)
	{
		// loop for how many rows to move
		for (int x = 0; x > rowValue; x--)
		{
			// loop to move each row, MAXROWS -1 is used because we access i + 1
			for (int i = 0; i < MAXROWS - 1; i++)
			{
				// loop to move each character up in a row
				for (int j = 0; j < MAXCOLS; j++)
				{
					canvas[i][j] = oldCanvas[i + 1][j];
				}
			}

			// replaces the bottom row with spaces
			for (int i = 0; i < MAXCOLS; i++) canvas[MAXROWS - 1 + x][i] = ' ';

			// creates a new copy to base the next move after
			copyCanvas(oldCanvas, canvas);
		}
	}

	// moving colums right
	if (colValue > 0)
	{
		// loop for how many colums to move
		for (int x = 0; x < colValue; x++)
		{
			// loop for each row
			for (int i = 0; i < MAXROWS; i++)
			{
				// loop to move each character in a row one to the right
				// MAXCOLS - 1 is used because of how indexs work
				for (int j = MAXCOLS - 1; j > 1; j--)
				{
					canvas[i][j] = oldCanvas[i][j - 1];
				}

				// replace the far right character with a space
				canvas[i][x] = ' ';
			}

			// creates a new copy to base the next move after 
			copyCanvas(oldCanvas, canvas);
		}
	}

	// moving colums left
	else if (colValue < 0)
	{
		// loop for how many colums to move
		for (int x = 0; x > colValue; x--)
		{
			// loop for each row
			for (int i = 0; i < MAXROWS; i++)
			{
				// loop to move each character in a row one to the right
				// MAXCOLS - 1 is used because we access j + 1
				for (int j = 0; j < MAXCOLS - 1; j++)
				{
					canvas[i][j] = oldCanvas[i][j + 1];
				}

				// replaces the far right character with a space
				canvas[i][MAXCOLS - 1 + x] = ' ';
			}

			// creates a new copy to base the next move after
			copyCanvas(oldCanvas, canvas);
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
	for (int i = 0; i < 80; i++) cout << "-";
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
				if (counter > MAXCOLS-1 && charCopy != '\n')
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
			while (charCopy != '\n' && !infile.eof() )
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