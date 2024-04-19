// Link Parrish
// Tim Butts
// Christian Grandy
// Trey Roberts

#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Definitions.h"
using namespace std;


Point::Point(DrawPoint p)
{
	row = (int)round(p.row);
	col = (int)round(p.col);
}

// https://math.stackexchange.com/questions/39390/determining-end-coordinates-of-line-with-the-specified-length-and-angle
DrawPoint findEndPoint(DrawPoint start, int len, int angle)
{
	DrawPoint end;
	end.col = start.col + len * cos(degree2radian(angle));
	end.row = start.row + len * sin(degree2radian(angle));
	return end;
}


// Use this to draw characters into the canvas, with the option of performing animation
void drawHelper(char canvas[][MAXCOLS], Point p, char ch, bool animate)
{
	// Pause time between steps (in milliseconds)
	const int TIME = 50;

	// Make sure point is within bounds
	if (p.row >= 0 && p.row < MAXROWS && p.col >= 0 && p.col < MAXCOLS)
	{
		// Draw character into the canvas
		canvas[p.row][p.col] = ch;

		// If animation is enabled, draw to screen at same time
		if (animate)
		{
			gotoxy(p.row, p.col);
			printf("%c", ch);
			Sleep(TIME);
		}
	}
}


// Fills gaps in a row caused by mismatch between match calculations and screen coordinates
// (i.e. the resolution of our 'canvas' isn't very good)
void drawLineFillRow(char canvas[][MAXCOLS], int col, int startRow, int endRow, char ch, bool animate)
{
	// determine if we're counting up or down
	if (startRow <= endRow)
		for (int r = startRow; r <= endRow; r++)
		{
			Point point(r, col);
			drawHelper(canvas, point, ch, animate);
		}
	else
		for (int r = startRow; r >= endRow; r--)
		{
			Point point(r, col);
			drawHelper(canvas, point, ch, animate);
		}
}


// Draw a single line from start point to end point
void drawLine(char canvas[][MAXCOLS], DrawPoint start, DrawPoint end, bool animate)
{
	char ch;

	Point scrStart(start);
	Point scrEnd(end);

	// vertical line
	if (scrStart.col == scrEnd.col)
	{
		ch = '|';

		drawLineFillRow(canvas, scrStart.col, scrStart.row, scrEnd.row, ch, animate);
	}
	// non-vertical line
	else
	{
		int row = -1, prevRow;

		// determine the slope of the line
		double slope = (start.row - end.row) / (start.col - end.col);

		// choose appropriate characters based on 'steepness' and direction of slope
		if (slope > 1.8)  ch = '|';
		else if (slope > 0.08)  ch = '`';
		else if (slope > -0.08)  ch = '-';
		else if (slope > -1.8) ch = '\'';
		else ch = '|';

		// determine if columns are counting up or down
		if (scrStart.col <= scrEnd.col)
		{
			// for each column from start to end, calculate row values
			for (int col = scrStart.col; col <= scrEnd.col; col++)
			{
				prevRow = row;
				row = (int)round(slope * (col - start.col) + start.row);

				// draw from previous row to current row (to fill in row gaps)
				if (prevRow > -1)
				{
					drawLineFillRow(canvas, col, prevRow, row, ch, animate);
				}
			}
		}
		{
			// for each column from start to end, calculate row values
			for (int col = scrStart.col; col >= scrEnd.col; col--)
			{
				prevRow = row;
				row = (int)round(slope * (col - start.col) + start.row);

				// draw from previous row to current row (to fill in row gaps)
				if (prevRow > -1)
				{
					drawLineFillRow(canvas, col, prevRow, row, ch, animate);
				}
			}
		}
	}
}


// Draws a single box around a center point
void drawBox(char canvas[][MAXCOLS], Point center, int height, bool animate)
{
	int sizeHalf = height / 2;
	int ratio = (int)round(MAXCOLS / (double)MAXROWS * sizeHalf);

	// Calculate where the four corners of the box should be
	DrawPoint points[4];
	points[0] = DrawPoint(center.row - sizeHalf, center.col - ratio);
	points[1] = DrawPoint(center.row - sizeHalf, center.col + ratio);
	points[2] = DrawPoint(center.row + sizeHalf, center.col + ratio);
	points[3] = DrawPoint(center.row + sizeHalf, center.col - ratio);

	// Draw the four lines of the box
	for (int x = 0; x < 3; x++)
	{
		drawLine(canvas, points[x], points[x + 1], animate);
	}
	drawLine(canvas, points[3], points[0], animate);

	// Replace the corners with a better looking character
	for (int x = 0; x < 4; x++)
	{
		drawHelper(canvas, points[x], '+', animate);
	}
}


// Menu for the drawing tools
void menuTwo(char canvas[][MAXCOLS], char backupCanvas[][MAXCOLS], bool& animate)
{
	bool exitFlag = true;
	char drawAction;
	char menu[] = "<A>nimate: N / <F>ill / <L>ine / <B>ox / <N>ested Boxes / <T>ree / <M>ain Menu: ";
	char drawBuffer[BUFFERSIZE];
	char animateChar = animate ? 'Y' : 'N';

	Point selPoint1;
	Point selPoint2;
	char pointChar1, pointChar2;
	int height, branchAngle;

	while (exitFlag)
	{

		displayCanvas(canvas);
		menu[11] = animateChar;
		cout << menu;

		cin.getline(drawBuffer, BUFFERSIZE);
		drawAction = drawBuffer[0];
		drawAction = toupper(drawAction);

		switch (drawAction)
		{
			// animate case
		case 'A':
		{
			// if animate is currently true set it to false and vice-versa
			animate = !animate;
			animateChar = animate ? 'Y' : 'N';
			break;
		}

		// fill case
		case 'F':
		{
			

			// get the point to start filling from
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "Enter character to fill with from current location / <ESC> to cancel";
			pointChar1 = getPoint(selPoint1);

			// cancel if ESC is pressed
			if (pointChar1 == ESC) break;
			copyCanvas(backupCanvas, canvas);

			fillRecursive(canvas, selPoint1.row, selPoint1.col, canvas[selPoint1.row][selPoint1.col], pointChar1, animate);
			break;
		}

		// line case
		case 'L':
		{
			

			// get the first line point
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "type any character to choose start point / <ESC> to cancel";
			pointChar1 = getPoint(selPoint1);

			// cancel if ESC is pressed
			if (pointChar1 == ESC) break;

			// temporarily print the starting line point to the screen
			gotoxy(selPoint1.row, selPoint1.col);
			cout << pointChar1;

			// get the second line point
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "type any character to choose end point / <ESC> to cancel";
			pointChar2 = getPoint(selPoint2);

			// cancel if ESC is pressed
			if (pointChar2 == ESC) break;

			copyCanvas(backupCanvas, canvas);

			// display canvas to remove the temporary first line point  
			displayCanvas(canvas);

			drawLine(canvas, selPoint1, selPoint2, animate);
			break;
		}

		// box case
		case 'B':
		{

			// get the height of the box
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "Enter Size: ";
			cin >> height;

			// get the center point of the box
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "Type any letter to choose box center, or <C> for screen center / ESC to cancel ";
			pointChar1 = getPoint(selPoint1);
			pointChar1 = toupper(pointChar1);

			// cancel if ESC is pressed
			if (pointChar1 == ESC) break;

			// if C is entered use the center of the screen instead
			else if (pointChar1 == 'C')
			{
				selPoint1.row = MAXROWS / 2;
				selPoint1.col = MAXCOLS / 2;
			}

			copyCanvas(backupCanvas, canvas);
			drawBox(canvas, selPoint1, height, animate);
			break;
		}

		// nested boxes
		case 'N':
		{
			
			// get the height of the largest nested box
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "Enter size of largest box: ";
			cin >> height;

			// get the center point of the boxes
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "Type any letter to choose box center / <ESC> to cancel ";
			pointChar1 = getPoint(selPoint1);
			pointChar1 = toupper(pointChar1);

			// cancel if ESC is pressed
			if (pointChar1 == ESC) break;


			// if C is entered use the center of the screen instead
			else if (pointChar1 == 'C')
			{
				selPoint1.row = MAXROWS / 2;
				selPoint1.col = MAXCOLS / 2;
			}

			copyCanvas(backupCanvas, canvas);
			drawBoxesRecursive(canvas, selPoint1, height, animate);
			break;
		}

		// tree case
		case 'T':
		{

			// get the height of the tree
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "Enter approximate tree height: ";
			cin >> height;

			// get the branch angle
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "Enter branch angle: ";
			cin >> branchAngle;

			// get the bottom point for the tree
			clearLine(MAXROWS + 1, sizeof(menu) + 20);
			cout << "Type any letter to choose start point, or <C> for bottom center / <ESC> to cancel  ";
			pointChar1 = getPoint(selPoint1);
			pointChar1 = toupper(pointChar1);

			// cancel if ESC is pressed
			if (pointChar1 == ESC) break;

			// if C is entered use the bottom center of the screen instead
			else if (pointChar1 == 'C')
			{
				selPoint1.row = MAXROWS - 1;
				selPoint1.col = MAXCOLS / 2;
			}

			copyCanvas(backupCanvas, canvas);
			treeRecursive(canvas, selPoint1, height, 270, branchAngle, animate);
			break;
		}

		// exit to menu case
		case 'M':
		{
			exitFlag = false;
			break;
		}
		}
	}
}


// Get a single point from screen, with character entered at that point
char getPoint(Point& pt)
{
	char input;
	int row = 0, col = 0;

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

		// If the key is a printable ASCII Character between 32 and 126,
		// set the point row and col and return the input
		else if (32 <= input && input <= 126)
		{
			pt.row = row;
			pt.col = col;
			return input;
		}

		// If the key used is special key. 
		else if (input == '\0')
		{
			input = _getch();
		}

		gotoxy(row, col);
		input = _getch();

	}
	return ESC;
}


// Recursively fill a section of the screen
void fillRecursive(char canvas[][MAXCOLS], int row, int col, char oldCh, char newCh, bool animate)
{
	bool ifInBounds = row >= 0 && row < MAXROWS&& col >= 0 && col < MAXCOLS;

	// if we are inbounds and we should replace this character
	if (ifInBounds && canvas[row][col] == oldCh)
	{
		// base case fill the current character
		drawHelper(canvas, Point(row, col), newCh, animate);

		// recursive cases fill the characters around this one
		fillRecursive(canvas, row, col - 1, oldCh, newCh, animate);
		fillRecursive(canvas, row - 1, col, oldCh, newCh, animate);
		fillRecursive(canvas, row, col + 1, oldCh, newCh, animate);
		fillRecursive(canvas, row + 1, col, oldCh, newCh, animate);
	}
}


// Recursively draw a tree
void treeRecursive(char canvas[][MAXCOLS], DrawPoint start, int height, int startAngle, int branchAngle, bool animate)
{
	bool ifInBounds = start.row >= 0 && start.row < MAXROWS&& start.col >= 0 && start.col < MAXCOLS;

	// Base Case
	if (ifInBounds && height > 2)
	{
		// Creates the trunk of the Tree 
		DrawPoint end = findEndPoint(start, height / 3, startAngle);
		drawLine(canvas, start, end, animate);

		// Creates a branch to the right
		treeRecursive(canvas, end, height - 2, branchAngle + startAngle, branchAngle, animate);

		// Creates a branch to the left
		treeRecursive(canvas, end, height - 2, startAngle - branchAngle, branchAngle, animate);
	}
}



// Recursively draw nested boxes
void drawBoxesRecursive(char canvas[][MAXCOLS], Point center, int height, bool animate)
{
	// Exit case for smallest box
	if (height >= 2)
	{
		// Draws box
		drawBox(canvas, center, height, animate);

		// Calls itself to draw next box with height - 2
		drawBoxesRecursive(canvas, center, height - 2, animate);
	}

}