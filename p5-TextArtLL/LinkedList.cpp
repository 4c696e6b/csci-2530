// Link Parrish
// Tim Butts
// Christian Grandy
// Trey Roberts

#include <iostream>
#include <Windows.h>
#include "Definitions.h"
using namespace std;


Node* newCanvas()
{
	// create new node
	Node* newNode = NULL;
	newNode = new Node;

	// ititalize values for node
	initCanvas(newNode->item);
	newNode->next = NULL;

	return newNode;
}


Node* newCanvas(Node* oldNode)
{
	// create new node
	Node* newNode = NULL;
	newNode = new Node;

	// ititalize values for node
	copyCanvas(newNode->item, oldNode->item);
	newNode->next = NULL;

	return newNode;
}


void play(List& clips)
{
	// clears the menu to print instructions on how to stop
	clearLine(MAXROWS + 1, MAXCOLS + 40);
	clearLine(MAXROWS + 2, MAXCOLS + 40);

	// loops as long as the ESCAPE key is not currently being pressed
	while (!(GetKeyState(VK_ESCAPE) & 0x8000))
	{
		playRecursive(clips.head, clips.count);
	}
}


void playRecursive(Node* head, int count)
{
	// checks if escape is being held
	// recursive case there are more clips
	if (count > 1)
	{
		// recursive call to handle the rest of the clips
		playRecursive(head->next, count - 1);
	}
	// displays the clip number
	clearLine(MAXROWS + 1, MAXCOLS + 20);
	cout << "hold <ESC> to stop \t clip: " << count;

	// displays only if ESC is not being held
	if (!(GetKeyState(VK_ESCAPE) & 0x8000))
	{
		// base case display the current clip
		displayCanvas(head->item);

		// Pause for 100 milliseconds to slow down animation
		Sleep(100);
	}
}


void addUndoState(List& undoList, List& redoList, Node*& current)
{
	// makes a copy of the current canvas and adds it to the undo list
	Node* temp = newCanvas(current);
	addNode(undoList, temp);

	deleteList(redoList);
}


void restore(List& undoList, List& redoList, Node*& current)
{
	// adds node to the redo list, and sets the current canvas to the most recent undo list node
	addNode(redoList, current);
	current = removeNode(undoList);
}


void addNode(List& list, Node* nodeToAdd)
{
	// if the list is empty, set the head pointer to the new node
	if (list.head == NULL)
		list.head = nodeToAdd;

	// if the list is not empty, set the pointer in the new node to the old head, and set the head to the new node
	else
	{
		nodeToAdd->next = list.head;
		list.head = nodeToAdd;
	}

	list.count++;
}


Node* removeNode(List& list)
{
	// create a temp pointer and set it to the first node
	Node* temp = list.head;

	// sets the head to the second node
	list.head = list.head->next;

	list.count--;
	return temp;
}


void deleteList(List& list)
{
	// while the list is not empty
	while (list.head != NULL)
	{
		// create a temp pointer and set it to the second node
		Node* temp = list.head->next;

		// delete the first node
		delete list.head;

		// set the head to the second node
		list.head = temp;
	}

	// reset the list counter
	list.count = 0;
}


bool loadClips(List& clips, char filename[])
{

	Node* temp = NULL;
	int x = 1;
	char baseFilename[MAXINPUTSIZE];
	bool loadMore = true;

	deleteList(clips);

	// copies the filename to base filename to preserve it
	snprintf(baseFilename, MAXINPUTSIZE, filename);

	// while the last load worked
	while (loadMore)
	{
		// copies the baseFilename with a number at the end to filename
		snprintf(filename, MAXINPUTSIZE, "%s-%d", baseFilename, x);

		// creates a new canvas to load into
		temp = newCanvas();

		// attempts to load into the temp canvas
		loadMore = loadCanvas(temp->item, filename);

		// if the load fails and this is the first file return false
		if (!loadMore && x == 1)
		{
			delete temp;
			return false;
		}

		// if the load succeeds, add the node to clips
		if (loadMore)
		{
			addNode(clips, temp);
			x++;
		}
	}

	// deletes the extra node created by the loop
	delete temp;
	return true;	
}


bool saveClips(List& clips, char filename[])
{
	Node* node = clips.head;
	bool saved;
	char baseFilename[MAXINPUTSIZE];

	// copies the filename to base filename to preserve it
	snprintf(baseFilename, MAXINPUTSIZE, filename);
	
	// for loop to loop through the entire clips list
	for (int i = 1; i <= clips.count; i++)
	{
		// creates our clip filename based on i
		snprintf(filename, MAXINPUTSIZE, "%s-%d", baseFilename, i);

		// attempts to save a clip
		bool saved = saveCanvas(node->item, filename);

		// if the clip saved and the next clip is valid select the next clip
		if (saved && node->next != NULL)
			node = node->next;

		// if the clip failed to save and this is not the last cycle return false
		else if (i != clips.count)
			return false;
	}

	return true;
	
}