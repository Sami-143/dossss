#include <iostream>
#include <list>
#include <iterator>
#include <algorithm> 
#include <string>
#include<queue>
#include<ctime>
#include<fstream>
#include <list>
#include <conio.h>  // for _getch()
#include <cstdlib>
#include <stack>
#include<windows.h>

using namespace std;


string splitString(string input, char split)
{
	string result = "";
	int startIndex = 0;

	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] == split)
		{
			startIndex = i + 1;
			break;
		}
	}

	for (int i = startIndex; i < input.length(); i++)
	{
		result = result + input[i];
	}

	return result;
}


void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}



// files class
class File
{
public:
	string name;
	string data;
	int size;


	// Default constructor
	File()
	{
		name = "";
		data = "";
		size = 0;
	}

	File(string filename, string filedata)
	{
		name = filename;
		data = filedata;
		size = data.length();
	}

	bool containsText(string searchText)
	{
		// Check if the file's data contains the search text
		size_t found = data.find(searchText);
		return found != string::npos;
	}

	void print_file_Data()
	{
		cout << data << endl;
	}

	bool operator<(const File& other) const {
		return size < other.size;
	}
};


// directory class
class Directory
{
public:
	string name;
	string path;
	list<Directory>* directory;
	list<File>* fileList;
	Directory* parent_head;

	Directory(string Directoryname, Directory* parent_head)
	{
		name = Directoryname;
		this->parent_head = parent_head;
		path = setPath();

		directory = new list<Directory>();
		fileList = new list<File>();
	}

	void addingFile(string file_name, string file_data)
	{
		// Add a new file to the current directory's file list
		fileList->push_back(File(file_name, file_data));
	}

	void addingDirectory(string directory_Name, Directory* parent_head)
	{
		// Add a new directory to the current directory's directory list
		directory->push_back(Directory(directory_Name, parent_head));
	}

	File* findingFile(string filename) {
		if (filename.find("V:\\") != string::npos || filename.find("V:") != string::npos) {
			// If the filename contains "V:\", recursively call findingFile with the substring
			return findingFile(filename.substr(filename.find("\\") + 1, filename.length() - 1));
		}
		else if (filename.find("\\") != string::npos) {
			// If the filename contains "\", extract the directory name and search in subdirectories
			string name = filename.substr(0, filename.find("\\"));

			for (auto iter = directory->begin(); iter != directory->end(); ++iter) {
				if (iter->name == name) {
					return iter->findingFile(filename.substr(filename.find("\\") + 1, filename.length() - 1));
				}
			}
		}
		else {
			// If the filename doesn't contain "\", search for the file in the current directory
			for (auto iter = fileList->begin(); iter != fileList->end(); ++iter) {
				if (iter->name == filename) {
					return &(*iter);
				}
			}
		}
		// Return nullptr if the file is not found
		return nullptr;
	}

	Directory getDirectoryByName(string directoryName)
	{
		// Search for a directory by name in the current directory's directory list
		for (auto it = directory->begin(); it != directory->end(); ++it)
		{
			if (it->name == directoryName)
			{
				return *it;
			}
		}
		// Return an empty directory if not found (you may want to handle this differently)
		return Directory("", nullptr);
	}

	File getFileByName(string fileName)
	{
		// Search for a file by name in the current directory's file list
		for (auto iter = fileList->begin(); iter != fileList->end(); ++iter)
		{
			if (iter->name == fileName)
			{
				return *iter;
			}
		}
		// Return an empty file if not found (you may want to handle this differently)
		return File();
	}

	void printingPath()
	{
		// Print the current directory's path
		cout << (path == "V" ? "V:\\" : path) << endl;
	}

	string setPath()
	{
		if (parent_head == nullptr)
		{
			// If the directory has no parent (root directory), return the root path
			return "V:\\";
		}
		// Otherwise, return the concatenated path of the parent directory, current directory name, and "\\"
		return parent_head->path + name + "\\";
	}


	// checking if directory already exists
	bool doesDirectoryExist(string directory_name)
	{
		for (auto iter = directory->begin(); iter != directory->end(); iter++)
		{
			if (iter->name == directory_name)
			{
				return true;
			}
		}
		return false;
	}

	// checking if file already exists
	bool doesFileExist(string filename)
	{
		for (auto it = fileList->begin(); it != fileList->end(); it++)
		{
			if (it->name == filename)
			{
				return true;
			}
		}
		return false;
	}

	void RENAME(string currentName, string newName)
	{
		// First search the file with the current name
		auto itFile = fileList->begin();
		while (itFile != fileList->end()) {
			if (itFile->name == currentName) {
				break;
			}
			++itFile;
		}

		// or search the directory with the current name
		auto itDirectory = directory->begin();
		while (itDirectory != directory->end()) {
			if (itDirectory->name == currentName) {
				break;
			}
			++itDirectory;
		}

		// Verify whether a file or directory with the given name is present.
		if (itFile != fileList->end() || itDirectory != directory->end())
		{
			// Determine whether a file or directory with the provided new name already exists.
			if (!doesFileExist(newName) && !doesDirectoryExist(newName))
			{
				// Change the name of the file
				if (itFile != fileList->end()) {
					itFile->name = newName;
					cout << "File renamed successfully" << endl;
				}

				// Change the name of the directory
				if (itDirectory != directory->end()) {
					itDirectory->name = newName;
					cout << "Directory renamed successfully" << endl;
				}
			}
			else
			{
				cout << "File or directory with the new name already exists" << endl;
			}
		}
		else
		{
			cout << "File or directory with the current name not found" << endl;
		}
	}
	// get directory through path
	Directory* getDirectoryFromPath(string path) {
		// Check if the path is the current directory
		if (path == "V:" || path == "V:\\") {
			return this; // Return the current directory if the path matches
		}
		// Check if the path contains "\"
		else if (path.find("\\") != string::npos) {
			// Extract the first directory name in the path
			string directoryname = path.substr(0, path.find("\\"));

			// Check if the directory name is "V:\" or "V:"
			if (directoryname == "V:\\" || directoryname == "V:") {
				// Recursively call getDirectoryFromPath with a modified path
				return this->getDirectoryFromPath(path.substr(path.find("\\") + 1, path.length() - 1));
			}

			// Iterate through subdirectories
			for (auto it = directory->begin(); it != directory->end(); ++it) {
				// If a subdirectory with the same name is found, recursively call getDirectoryFromPath with a modified path
				if (it->name == directoryname) {
					return it->getDirectoryFromPath(path.substr(path.find("\\") + 1, path.length() - 1));
				}
			}
		}
		// If no "\" is found, search for the directory in the current directory's subdirectories
		else {
			for (auto it = directory->begin(); it != directory->end(); ++it) {
				// If a subdirectory with the same name is found, return a pointer to it
				if (it->name == path) {
					return &(*it);
				}
			}
		}

		// If the directory is not found, return nullptr
		return nullptr;
	}


};

// class to store current state of text
class TextDocumentState
{
public:
	list<list<char>>* editingtxtLines;
	list<list<char>>::iterator rowIterator;
	list<char>::iterator colIterator;
	int cursor_row, cursor_col;

	TextDocumentState()
	{
		editingtxtLines = new list<list<char>>();
		editingtxtLines->push_back(list<char>());
		rowIterator = editingtxtLines->begin();
		colIterator = rowIterator->begin();
		cursor_row = cursor_col = 0;
	}

	void printData()
	{
		for (auto it = editingtxtLines->begin(); it != editingtxtLines->end(); ++it)
		{
			for (auto it2 = it->begin(); it2 != it->end(); ++it2)
			{
				cout << *it2;
			}
			cout << endl;
		}
	}
};

// text editor
class TextEditor
{
public:
	int cursor_row;
	int cursor_col;
	list<list<char>>* lines;
	list<list<char>>::iterator rowIterator;
	list<char>::iterator colIterator;
	stack<TextDocumentState*>* redoStack;
	deque<TextDocumentState*>* undoQueue;

	TextEditor()
	{
		cursor_row = cursor_col = 0;
		lines = new list<list<char>>();
		lines->push_back(list<char>());
		redoStack = new stack<TextDocumentState*>();
		undoQueue = new deque<TextDocumentState*>();
		rowIterator = lines->begin();
		colIterator = rowIterator->begin();
	}

	void loadFileState(File* file) {
		// Extract data from the file
		string data = file->data;
		string line = "";

		// Iterate through the data character by character
		for (int i = 0; i < data.length(); i++) {
			// Check for newline characters
			if (data[i] == '\n') {
				// Insert a new row for each newline character
				rowIterator = lines->insert(rowIterator, list<char>());

				// Copy characters from the line into the row
				for (int j = 0; j < line.length(); j++) {
					rowIterator->push_back(line[j]);
				}

				// Reset the line for the next row
				line = "";

				// Move to the next row
				rowIterator++;
			}
			else {
				// Collect characters for the current line
				line += data[i];
			}
		}

		// Handle the last line if it is not empty
		if (line.length() > 0) {
			rowIterator = lines->insert(rowIterator, list<char>());
			for (int j = 0; j < line.length(); j++) {
				rowIterator->push_back(line[j]);
			}
		}

		// Reset iterators for future use
		rowIterator = lines->begin();
		colIterator = rowIterator->begin();

		// Remove the last line if it is empty
		auto temp = lines->begin();
		for (auto it = lines->begin(); it != lines->begin(); it++) {
			temp++;
		}
		temp++;
		if (temp->empty()) {
			lines->erase(temp);
		}
	}



	void saveState()
	{
		// Create a new TextDocumentState object to store the current state
		TextDocumentState* state = new TextDocumentState();

		// Iterate through the lines in the document
		auto row_iter = state->editingtxtLines->begin();
		for (auto it = lines->begin(); it != lines->end(); it++, row_iter++) {

			// Create a new row in the state's lines list
			state->editingtxtLines->push_back(list<char>());

			// Copy characters from the current line to the corresponding row in the state
			for (auto it2 = it->begin(); it2 != it->end(); it2++)
			{
				row_iter->push_back(*it2);
			}
		}

		// Set the iterators in the state to match the current cursor position
		state->rowIterator = state->editingtxtLines->begin();
		for (int i = 0; i < cursor_col; i++) {
			state->rowIterator++;
		}
		state->colIterator = state->rowIterator->begin();
		for (int i = 0; i < cursor_row; i++) {
			state->colIterator++;
		}

		// Set the cursor position in the state
		state->cursor_row = cursor_row;
		state->cursor_col = cursor_col;

		// Add the state to the undoQueue
		undoQueue->push_back(state);

		// Limit the size of the undoQueue to 5
		if (undoQueue->size() > 5) {
			undoQueue->pop_front();
		}
	}

	
	void loadState(TextDocumentState* state) {
		// Load the state from the given TextDocumentState object

		// Set the lines of the document to match the state's editingtxtLines
		lines = state->editingtxtLines;

		// Set the rowIterator to the beginning of the lines
		rowIterator = lines->begin();

		// Set the cursor position based on the state's cursor_row and cursor_col
		cursor_row = state->cursor_row;
		cursor_col = state->cursor_col;

		// Move the rowIterator to match the cursor_col
		for (int idx = 0; idx < cursor_col; idx++) {
			rowIterator++;
		}

		// Set the colIterator to the beginning of the current row and move it to match the cursor_row
		colIterator = rowIterator->begin();
		for (int idx = 0; idx <= cursor_row; idx++) {
			colIterator++;
		}
	}

	//void print() {
	//	// Print the content of the document lines

	//	for (auto iter = lines->begin(); iter != lines->end(); ++iter) {
	//		for (auto iter1 = iter->begin(); iter1 != iter->end(); ++iter1) {
	//			cout << *iter1;
	//		}
	//		cout << endl;
	//	}
	//}

	void print() {
		try {
			if (lines == nullptr) {
				throw runtime_error("Document lines are null");
			}

			for (auto iter = lines->begin(); iter != lines->end(); ++iter) {
				for (auto iter1 = iter->begin(); iter1 != iter->end(); ++iter1) {
					cout << *iter1;
				}
				cout << endl;
			}
		}
		catch (const exception& e) {
			cerr << "Exception: " << e.what() << endl;
		}
	}

	void Save(File* file)
	{
		// Save the content of the document lines into the given file

		string data = "";
		for (auto iter = lines->begin(); iter != lines->end(); ++iter) {
			for (auto iter1 = iter->begin(); iter1 != iter->end(); ++iter1) {
				data += *iter1;
			}
			data += "\n";
		}

		// Remove the last newline character
		data.pop_back();

		// Set the file's data to the accumulated content
		file->data = data;
	}


	//void movingUpAfterKeyPressed() {
	//	// Move the cursor up in the document after a key press

	//	// Check if the cursor is not at the beginning of the row
	//	if (cursor_col != 0) {
	//		// Decrement the cursor column
	//		cursor_col--;

	//		// Move the rowIterator to the previous row
	//		rowIterator--;

	//		// Adjust cursor_row if it exceeds the size of the current row
	//		if (cursor_row >= rowIterator->size()) {
	//			cursor_row = rowIterator->size();
	//		}

	//		// Set colIterator to the beginning of the current row
	//		colIterator = rowIterator->begin();

	//		// Move colIterator to match the cursor_row
	//		for (int i = 0; i < cursor_row; i++) {
	//			colIterator++;
	//		}
	//	}
	//}
	void movingUpAfterKeyPressed() {
		try {
			// Check if the cursor is not at the beginning of the row
			if (cursor_col != 0) {
				// Decrement the cursor column
				cursor_col--;

				// Move the rowIterator to the previous row
				rowIterator--;

				// Adjust cursor_row if it exceeds the size of the current row
				if (cursor_row >= rowIterator->size()) {
					cursor_row = rowIterator->size();
				}

				// Set colIterator to the beginning of the current row
				colIterator = rowIterator->begin();

				// Move colIterator to match the cursor_row
				for (size_t i = 0; i < cursor_row; i++) {
					colIterator++;
				}
			}
		}
		catch (const exception& e) {
			cerr << "Exception: " << e.what() << endl;
		}
	}
	//void movingDownAfterKeyPressed() {
	//	// Move the cursor down in the document after a key press

	//	// Check if the cursor is not at the last row
	//	if (cursor_col != lines->size() - 1) {
	//		// Increment the cursor column
	//		cursor_col++;

	//		// Move the rowIterator to the next row
	//		rowIterator++;

	//		// Adjust cursor_row if it exceeds the size of the current row
	//		if (cursor_row >= rowIterator->size()) {
	//			cursor_row = rowIterator->size();
	//		}

	//		// Set colIterator to the beginning of the current row
	//		colIterator = rowIterator->begin();

	//		// Move colIterator to match the cursor_row
	//		for (int i = 0; i < cursor_row; i++) {
	//			colIterator++;
	//		}
	//	}
	//}
	void movingDownAfterKeyPressed() {
		try {
			// Check if the cursor is not at the last row
			if (cursor_col != lines->size() - 1) {
				// Increment the cursor column
				cursor_col++;

				// Move the rowIterator to the next row
				rowIterator++;

				// Adjust cursor_row if it exceeds the size of the current row
				if (cursor_row >= rowIterator->size()) {
					cursor_row = rowIterator->size();
				}

				// Set colIterator to the beginning of the current row
				colIterator = rowIterator->begin();

				// Move colIterator to match the cursor_row
				for (size_t i = 0; i < cursor_row; i++) {
					colIterator++;
				}
			}
		}
		catch (const exception& e) {
			cerr << "Exception: " << e.what() << endl;
		}
	}

	//void movingLeftAfterKeyPressed() {
	//	// Move the cursor left in the document after a key press

	//	// Check if the cursor is at the beginning of the first row, and not at the beginning of the document
	//	if (cursor_row == 0 && cursor_col != 0) {
	//		// Move to the previous row
	//		rowIterator--;

	//		// Decrement the cursor column
	//		cursor_col--;

	//		// Set cursor_row to the size of the current row
	//		cursor_row = rowIterator->size();

	//		// Set colIterator to the beginning of the current row
	//		colIterator = rowIterator->begin();

	//		// Move colIterator to match the cursor_row
	//		for (int i = 0; i < cursor_row; i++) {
	//			colIterator++;
	//		}
	//	}
	//	// Check if the cursor is not at the beginning of the row
	//	else if (cursor_row != 0) {
	//		// Decrement the cursor row
	//		cursor_row--;

	//		// Move colIterator to the previous character
	//		colIterator--;
	//	}
	//}
	void movingLeftAfterKeyPressed() {
		try {
			// Check if the cursor is at the beginning of the first row, and not at the beginning of the document
			if (cursor_row == 0 && cursor_col != 0) {
				// Move to the previous row
				rowIterator--;

				// Decrement the cursor column
				cursor_col--;

				// Set cursor_row to the size of the current row
				cursor_row = rowIterator->size();

				// Set colIterator to the beginning of the current row
				colIterator = rowIterator->begin();

				// Move colIterator to match the cursor_row
				for (size_t i = 0; i < cursor_row; i++) {
					colIterator++;
				}
			}
			// Check if the cursor is not at the beginning of the row
			else if (cursor_row != 0) {
				// Decrement the cursor row
				cursor_row--;

				// Move colIterator to the previous character
				colIterator--;
			}
		}
		catch (const exception& e) {
			cerr << "Exception: " << e.what() << endl;
		}
	}

	//void movingRightAfterKeyPressed() {
	//	// Move the cursor right in the document after a key press

	//	// Check if the current row is empty
	//	if (rowIterator->size() == 0) {
	//		return;
	//	}

	//	// Check if the cursor is beyond the last character in the current row and not at the last row
	//	if (cursor_row > rowIterator->size() - 1 && cursor_col != lines->size() - 1) {
	//		// Move to the next row
	//		rowIterator++;

	//		// Increment the cursor column
	//		cursor_col++;

	//		// Set cursor_row to 0
	//		cursor_row = 0;

	//		// Set colIterator to the beginning of the current row
	//		colIterator = rowIterator->begin();
	//	}
	//	// Check if the cursor is within the bounds of the current row
	//	else if (cursor_row <= rowIterator->size() - 1) {
	//		// Increment the cursor row
	//		cursor_row++;

	//		// Move colIterator to the next character
	//		colIterator++;
	//	}
	//}

	void movingRightAfterKeyPressed() {
		try {
			// Check if the current row is empty
			if (rowIterator->empty()) {
				throw runtime_error("Current row is empty");
			}

			// Check if the cursor is beyond the last character in the current row and not at the last row
			if (cursor_row > rowIterator->size() - 1 && cursor_col != lines->size() - 1) {
				// Move to the next row
				rowIterator++;

				// Increment the cursor column
				cursor_col++;

				// Set cursor_row to 0
				cursor_row = 0;

				// Set colIterator to the beginning of the current row
				colIterator = rowIterator->begin();
			}
			// Check if the cursor is within the bounds of the current row
			else if (cursor_row <= rowIterator->size() - 1) {
				// Increment the cursor row
				cursor_row++;

				// Move colIterator to the next character
				colIterator++;
			}
		}
		catch (const exception& e) {
			cerr << "Exception: " << e.what() << endl;
		}
	}
	void BackspaceCommandInKeyBoard() {
		// Handle the Backspace key press in the document

		// Check if the cursor is at the beginning of the first row, and not at the beginning of the document
		if (cursor_row == 0 && cursor_col != 0) {

			// Create a temporary list to store characters from colIterator to the end of the row
			list<char>* temp = new list<char>();
			for (auto it = colIterator; it != rowIterator->end(); ++it) {
				temp->push_back(*it);
			}

			// Erase the current row from the lines list
			rowIterator = lines->erase(rowIterator);

			// Move to the previous row
			rowIterator--;

			// Decrement the cursor column
			cursor_col--;

			// Set cursor_row to the size of the current row
			cursor_row = rowIterator->size();

			// Set colIterator to the beginning of the current row
			colIterator = rowIterator->begin();

			// Move colIterator to match the cursor_row
			for (int i = 0; i < cursor_row; i++) {
				colIterator++;
			}

			// Append characters from the temporary list to the updated row
			for (auto it = temp->begin(); it != temp->end(); ++it) {
				rowIterator->push_back(*it);
			}
		}
		// Check if the cursor is not at the beginning of the row
		else if (cursor_row != 0) {
			// Decrement the cursor row
			cursor_row--;

			// Move colIterator to the previous character and erase it
			colIterator--;
			colIterator = rowIterator->erase(colIterator);
		}
	}


	void removeForward() {
		// Remove characters after the cursor position

		// Check if the cursor is at the end of the current row and not at the last row
		if (cursor_row == rowIterator->size() && cursor_col != lines->size() - 1) {

			// Create a temporary list to store characters from the next row
			list<char>* temp = new list<char>();
			auto tempItr = rowIterator;
			tempItr++;
			for (auto it = tempItr->begin(); it != tempItr->end(); ++it) {
				temp->push_back(*it);
			}

			// Erase the next row
			lines->erase(tempItr);

			// Append characters from the temporary list to the current row
			for (auto it = temp->begin(); it != temp->end(); ++it) {
				rowIterator->push_back(*it);
			}

			// Set colIterator to the beginning of the current row
			colIterator = rowIterator->begin();

			// Move colIterator to match the cursor_row
			for (int i = 0; i < cursor_row; i++) {
				colIterator++;
			}
		}
		// Check if the cursor is within the bounds of the current row
		else if (cursor_row != rowIterator->size()) {
			// Erase the character at the cursor position
			colIterator = rowIterator->erase(colIterator);
		}
	}

	void AppendAtNextline() {
		// Append characters to the next line

		// Create a temporary list to store characters from the cursor position to the end of the row
		list<char>* temp = new list<char>();
		for (auto it = colIterator; it != rowIterator->end(); ++it) {
			temp->push_back(*it);
		}

		// Erase characters from the cursor position to the end of the row
		rowIterator->erase(colIterator, rowIterator->end());

		// Insert a new row with characters from the temporary list after the current row
		rowIterator = lines->insert(++rowIterator, *temp);

		// Set colIterator to the beginning of the new row
		colIterator = rowIterator->begin();

		// Update cursor position
		cursor_col++;
		cursor_row = 0;
	}

	void addChar(char c) {
		// Add a character at the cursor position in the current row
		rowIterator->insert(colIterator, c);

		// Increment cursor_row
		cursor_row++;
	}


	void undoCommandInKeyBoard() {
		// Undo the last command

		// Check if the undoQueue is not empty
		if (!undoQueue->empty()) {
			// Get the last state from the undoQueue
			TextDocumentState* current_State = undoQueue->back();

			// Remove the last state from the undoQueue
			undoQueue->pop_back();

			// Push the current state to the redoStack
			redoStack->push(current_State);

			// Load the state and refresh the display
			loadState(current_State);
			system("cls"); // Clear the console screen
			print();
		}
	}

	void redoCommandInKeyBoard(bool doIt) {
		// Redo the last undone command

		// Check if the redoStack is not empty and the 'doIt' flag is true
		if (!redoStack->empty() && doIt) {
			// Get the last state from the redoStack
			TextDocumentState* current_State = redoStack->top();

			// Remove the last state from the redoStack
			redoStack->pop();

			// Load the state and refresh the display
			loadState(current_State);
			system("cls"); // Clear the console screen
			print();
		}
	}




	bool clearRedoStack(bool doIt) {
		// Clear the redoStack

		// Check if the 'doIt' flag is true
		if (doIt) {
			// Remove all elements from the redoStack
			while (!redoStack->empty()) {
				redoStack->pop();
			}
		}
		return false;
	}


	void handleKeyPress(File* file) {
		loadFileState(file);

		system("color F0");
		system("cls");

		print();

		bool isEnabled = false;
		bool doIt = false;

		if (rowIterator->empty()) {
			rowIterator->push_back(' ');
			colIterator = rowIterator->begin();
		}

		while (true) {

			gotoxy(cursor_row, cursor_col);
			char character = _getch();

			bool capsLockState = GetKeyState(VK_CAPITAL);

			if (character == 0 || GetAsyncKeyState(VK_SHIFT) || character == 9 || capsLockState)
				continue;
			else if (character == 72)
			{
				movingUpAfterKeyPressed();
			}
			else if (character == 80)
			{
				movingDownAfterKeyPressed();
			}
			else if (character == 75)
			{
				movingLeftAfterKeyPressed();
			}
			else if (character == 77)
			{
				movingRightAfterKeyPressed();
			}
			else if (character == 13)
			{ // enter key pressed
				saveState();
				AppendAtNextline();
				isEnabled = true;
			}
			else if (character == 27)
			{ // escape key pressed
				break;
			}
			else if (character == 8)
			{ // backspace pressed
				saveState();
				BackspaceCommandInKeyBoard();
				isEnabled = true;
			}
			else if (character == 83)
			{ // delete pressed
				saveState();
				removeForward();
				isEnabled = true;
			}
			else if (character == 26)
			{ // CTRL + Z
				undoCommandInKeyBoard();
				doIt = true;
			}
			else if (character == 25)
			{ // CTRL + Y
				redoCommandInKeyBoard(doIt);
			}
			else
			{ // insert character
				saveState();
				addChar(character);
				isEnabled = true;
			}

			if (isEnabled)
			{
				system("cls");
				print();
				isEnabled = false;
				doIt = clearRedoStack(doIt);
			}
		}

		Save(file);
		system("color 07");
		system("cls");
	}
};


// tree class
class TreeNode
{
	priority_queue<string> priorityPrintQueue;
	string extension;
	void saveTreeToFile(Directory* currentDirectory, ofstream& outputFile, int level)
	{
		// Save the current directory
		outputFile << "D " << currentDirectory->name << endl;

		// Save files in the current directory
		for (auto it = currentDirectory->fileList->begin(); it != currentDirectory->fileList->end(); ++it)
		{
			outputFile << "F " << it->name << " " << it->data << endl;
		}

		// Recursively save subdirectories
		for (auto it = currentDirectory->directory->begin(); it != currentDirectory->directory->end(); ++it)
		{
			saveTreeToFile(&(*it), outputFile, level + 1);
		}
	}

public:
	Directory* root;
	Directory* current_ptr;
	string prompt = "> ";

	TreeNode() :extension(".txt")
	{
		current_ptr = root = new Directory("V", nullptr);
	}

	void addFile(string file_name, string file_data)
	{
		root->fileList->push_back(File(file_name, file_data));
	}

	void addDirectory(string directory_name)
	{
		root->directory->push_back(Directory(directory_name, root));
	}

	void printingPath()
	{
		cout << root->path << endl;
	}

	// dir function
	void DIR()
	{
		// Check if the current directory has no subdirectories and no files
		if (current_ptr->directory->empty() && current_ptr->fileList->empty())
		{
			cout << "No files or directory in the current directory." << endl;
		}
		else
		{
			// Print subdirectories (if any)
			cout << "directory:" << endl;
			for (auto it = current_ptr->directory->begin(); it != current_ptr->directory->end(); ++it)
			{
				cout << "   [DIR]  " << it->name << endl;
			}

			// Print files (if any)
			cout << endl << "Files:" << endl;
			for (auto it = current_ptr->fileList->begin(); it != current_ptr->fileList->end(); ++it)
			{
				cout << "   [FILE] " << it->name << endl;
			}

			// Print the total number of subdirectories and files in the current directory
			cout << endl << "\t\t(" << current_ptr->directory->size() << ") Dir(s)" << endl;
			cout << "\t\t(" << current_ptr->fileList->size() << ") File(s)" << endl;
		}
	}


	// rmdir function
	void RMDIR(string directoryname) {
		// Check if the specified directory exists in the current directory
		if (!current_ptr->doesDirectoryExist(directoryname)) {
			cout << "Directory not found" << endl;
			return; // Exit the function if the directory is not found
		}

		// Iterate through subdirectories in the current directory
		for (auto iter = current_ptr->directory->begin(); iter != current_ptr->directory->end(); ++iter) {
			// Check if the current subdirectory's name matches the specified directory name
			if (iter->name == directoryname) {
				// Remove the matching subdirectory from the current directory's list
				current_ptr->directory->erase(iter);
				break; // Exit the loop after removing the directory
			}
		}
	}

	// mkdir function
	void MKDIR(string directoryName) {
		// Check if the directory name is empty
		if (directoryName.empty()) {
			cout << "Error: Invalid syntax. Usage: mkdir <directory_name>" << endl;
			return;
		}

		// Check if the directory name contains any special characters
		if (!std::all_of(directoryName.begin(), directoryName.end(), ::isalnum)) {
			cout << "Error: Invalid characters in directory name. Use only alphanumeric characters." << endl;
			return;
		}

		// Checking if the directory or file already exists
		if (!current_ptr->doesDirectoryExist(directoryName) && !current_ptr->doesFileExist(directoryName)) {
			// If it doesn't exist, add a new Directory object to the current directory's vector
			current_ptr->directory->push_back(Directory(directoryName, current_ptr));
			cout << "Success: Directory created successfully: " << directoryName << endl;
			SAVETREE("Saved_tree.txt");
		}
		else {
			// If it already exists, display an error message
			cout << "Error: Directory or file with the same name already exists" << endl;
		}
	}

	// createFile function 
	void createFile(string fileName) {
		if (fileName.empty()) {
			cout << "Invalid syntax: File name cannot be empty" << endl;
			return;
		}

		// Check if the file name contains any special characters
		if (!std::all_of(fileName.begin(), fileName.end(), ::isalnum)) {
			cout << "Invalid file name: Use only alphanumeric characters for the file name." << endl;
			return;
		}

		// Checking for duplicate file names
		if (!current_ptr->doesFileExist(fileName) && !current_ptr->doesDirectoryExist(fileName)) {
			cout << "Enter file data: ";
			string fileData = take_input();  // Assuming you have a function to take input
			current_ptr->addingFile(fileName + extension, fileData); // Use default extension
			cout << "File created successfully" << endl;
			SAVETREE("Saved_tree.txt");
		}
		else {
			cout << "File or name already exists" << endl;
		}
	}

	// Function to check if a file name is valid
	bool isValidFileName(const string& fileName)
	{
		// Check if the file name contains only alphanumeric characters and no spaces
		return all_of(fileName.begin(), fileName.end(), [](char c) { return isalnum(c) || c == '_'; });
	}


	// rmfile function
	void DEL(string fileName)
	{
		// Check if the file does not exist in the current directory
		if (!current_ptr->doesFileExist(fileName))
		{
			// If the file is not found, display an error message and return
			cout << "Error: File not found." << endl;
			return;
		}

		// Iterate through the list of files in the current directory
		for (auto it = current_ptr->fileList->begin(); it != current_ptr->fileList->end(); ++it)
		{
			// Check if the current file's name matches the specified fileName
			if (it->name == fileName)
			{
				// If found, remove the file from the current directory's file list
				current_ptr->fileList->erase(it);
				cout << "Success: The file '" << fileName << "' has been successfully deleted." << endl;
				break;
			}
		}
	}


	// Add this function in the TreeNode class
	void PROMPT(string newPrompt)
	{
		// Set the current directory's path to the new prompt value
		current_ptr->path = newPrompt;
	}



	void ATTRIB(string attrib)
	{
		// Find the file with the specified attribute in the current directory
		File* file = current_ptr->findingFile(attrib);

		// Check if the file is not found
		if (file == nullptr) {
			cout << "Error: Unable to find the file." << endl;
			return;
		}

		// Display file attributes
		cout << "The name of File is: " << file->name << endl;
		cout << "The data of File is: " << file->data << endl;
		cout << "The size of File is: " << file->size << " bytes" << endl;
	}

	void find(string filename)
	{
		// Find the file with the specified filename in the current directory
		File* file = current_ptr->findingFile(filename);

		// Check if the file is not found
		if (file == nullptr) {
			cout << "Error: Unable to find the specified file." << endl;
			return;
		}

		// Display file attributes
		cout << "The name of file is: " << file->name << endl;
		cout << "The data of file is: " << file->data << endl;
	}

	// find the File
	void Findf(string inputString) {
		// Extracting the name and text from the inputString
		string name = inputString.substr(0, inputString.find(" "));
		string text = inputString.substr(inputString.find(" ") + 1, inputString.length() - 1);

		// Attempting to find the file using the extracted name
		File* file = current_ptr->findingFile(name);

		// Checking if the file was not found
		if (file == nullptr) {
			cout << "Could not find :(" << endl;
			return;
		}

		// Checking if the text is present in the file's data
		if (file->data.find(text) != string::npos) {
			// Outputting success message and file information
			cout << "Found the text :)" << endl;
			cout << "The name of file is: " << file->name << endl;
			cout << "The data of file is: " << file->data << endl;
		}
		else {
			// Outputting a message indicating that the text was not found
			cout << "Text is not found :( " << endl;
		}
	}

	// Update the CONVERT function to take two arguments
	void CONVERT(const string& oldExtension, const string& newExtension)
	{
		// Validate new extension
		if (isValidExtension(newExtension))
		{
			ConvertExtensionsRecursive(root, oldExtension, newExtension);
		}
		else
		{
			cout << "Invalid new file extension. Conversion aborted." << endl;
		}
	}

	void ConvertExtensionsRecursive(Directory* directories, const string& oldExtension, const string& newExtension)
	{
		for (auto& file : *directories->fileList)
		{
			size_t dotIndex = file.name.find_last_of('.');
			if (dotIndex != string::npos)
			{
				string fileExtension = file.name.substr(dotIndex + 1);
				if (fileExtension == oldExtension)
				{
					// Modify the file extension
					file.name = file.name.substr(0, dotIndex) + newExtension;
					cout << "\tChanged   to " << file.name << "\"" << endl;
				}
			}
		}

		for (auto& subdirectory : *directories->directory)
		{
			ConvertExtensionsRecursive(&subdirectory, oldExtension, newExtension);
		}
	}

	bool isValidExtension(const string& extension)
	{
		if (extension.empty() || extension.size() >= 10 || extension[0] != '.') {
			return false;  // Extension must start with a dot, be non-empty, and less than 10 characters
		}

		return all_of(extension.begin() + 1, extension.end(), [](char c) { return isalnum(c); });
	}



	// find string
	void FINDSTR(string txt) {
		// Iterating through the fileList to search for the specified text
		for (auto iter = current_ptr->fileList->begin(); iter != current_ptr->fileList->end(); ++iter) {
			// Checking if the text is present in the current file's data
			if (iter->data.find(txt) != string::npos) {
				// Outputting the name and data of the file if text is found
				cout << "The name of file is: " << iter->name << endl;
				cout << "The data of file is: " << iter->data << endl;
			}
			else {
				cout << "Text not found :(" << endl;
			}
		}
	}

	// format function
	void FORMAT() {
		// Clearing the fileList to format 
		current_ptr->fileList->clear();

		// Outputting a message indicating that the file has been formatted
		cout << "File formatted: " << current_ptr->path << endl;

		// Clearing the directory to format 
		current_ptr->directory->clear();

		// Outputting a message indicating that the directory has been formatted
		cout << "Directory formatted: " << current_ptr->path << endl;
	}


	//function to delete a file at given path
	void del_File(string filepath) {
		// Checking if the filepath contains the "V:\" or "V:" prefix
		if (filepath.find("V:\\") != string::npos || filepath.find("V:") != string::npos) {
			// Recursively calling del_File with the substring after the first backslash
			return del_File(filepath.substr(filepath.find("\\") + 1, filepath.length() - 1));
		}
		// Checking if the filepath contains a backslash
		else if (filepath.find("\\") != string::npos) {
			// Extracting the name from the filepath
			string name = filepath.substr(0, filepath.find("\\"));

			// Searching for the directory with the extracted name in the current directory
			for (auto iter = current_ptr->directory->begin(); iter != current_ptr->directory->end(); ++iter) {
				if (iter->name == name) {
					// Recursively calling del_File with the substring after the first backslash
					return del_File(filepath.substr(filepath.find("\\") + 1, filepath.length() - 1));
				}
			}
		}
		// If no backslash is found, treating filepath as a filename
		else {
			// Searching for the file with the specified name in the fileList
			for (auto iter = current_ptr->fileList->begin(); iter != current_ptr->fileList->end(); ++iter) {
				if (iter->name == filepath) {
					// Erasing the file from the fileList
					current_ptr->fileList->erase(iter);
					return;
				}
			}
		}
	}

	// function to delete a directory at given path
	void del_Directory(string filepath) {
		// Checking if the filepath contains the "V:\" or "V:" prefix
		if (filepath.find("V:\\") != string::npos || filepath.find("V:") != string::npos) {
			// Recursively calling del_Directory with the substring after the first backslash
			return del_Directory(filepath.substr(filepath.find("\\") + 1, filepath.length() - 1));
		}
		// Checking if the filepath contains a backslash
		else if (filepath.find("\\") != string::npos) {
			// Extracting the name from the filepath
			string name = filepath.substr(0, filepath.find("\\"));

			// Searching for the directory with the extracted name in the current directory
			for (auto it = current_ptr->directory->begin(); it != current_ptr->directory->end(); ++it) {
				if (it->name == name) {
					// Recursively calling del_Directory with the substring after the first backslash
					return del_Directory(filepath.substr(filepath.find("\\") + 1, filepath.length() - 1));
				}
			}
		}
		// If no backslash is found, treating filepath as a directory name
		else {
			// Searching for the directory with the specified name in the current directory
			for (auto it = current_ptr->directory->begin(); it != current_ptr->directory->end(); ++it) {
				if (it->name == filepath) {
					// Erasing the directory from the directory list
					current_ptr->directory->erase(it);
					return;
				}
			}
		}
	}

	// function to set path after moving directory
	void seting_Path_After_Moving(Directory* filepath) {
		// Iterating through the directories within the specified directory
		for (auto iter = filepath->directory->begin(); iter != filepath->directory->end(); ++iter) {
			// Updating the path of each subdirectory by appending its name and a backslash
			iter->path = filepath->path + iter->name + "\\";

			// Recursively calling seting_Path_After_Moving for the current subdirectory
			seting_Path_After_Moving(&(*iter));
		}
	}



	void MOVE(string input) {
		// Extracting source and destination paths from the input
		string sourcePath = input.substr(0, input.find(" "));
		string destinationPath = input.substr(input.find(" ") + 1, input.length() - 1);

		// Checking for invalid syntax
		if (sourcePath.empty() || destinationPath.empty()) {
			cout << "Invalid syntax" << endl;
			return;
		}

		// Retrieving source directory and file objects
		Directory* sourceDir = current_ptr->getDirectoryFromPath(sourcePath);
		File* sourceFile = current_ptr->findingFile(sourcePath);

		// Handling the case when the source is not found
		if (sourceDir == nullptr && sourceFile == nullptr) {
			cout << "Couldn't find the source :(" << endl;
			return;
		}

		// Retrieving destination directory and file objects
		Directory* destinationDir = current_ptr->getDirectoryFromPath(destinationPath);
		File* destinationFile = current_ptr->findingFile(destinationPath);

		// Checking if moving to a file is attempted
		if (destinationFile != nullptr) {
			cout << "Not able to move to a file" << endl;
			return;
		}

		// Handling the move for a source directory
		if (sourceDir != nullptr) {
			// Checking if a directory with the same name already exists in the destination
			if (destinationDir->doesDirectoryExist(sourceDir->name)) {
				cout << "The destination directory already has a directory with a matching name." << endl;
				return;
			}

			// Updating directory pointers and paths after the move
			sourceDir->parent_head = destinationDir;
			sourceDir->path = destinationDir->path + sourceDir->name + "\\";
			destinationDir->directory->push_back(*sourceDir);
			seting_Path_After_Moving(sourceDir);
			del_Directory(sourcePath);

			cout << "The directory has been successfully moved. :)" << endl;
		}
		// Handling the move for a source file
		else if (sourceFile != nullptr) {
			// Checking if a file with the same name already exists in the destination directory
			if (destinationDir->doesFileExist(sourceFile->name)) {
				cout << "Destination directory already contains a file with the same name" << endl;
				return;
			}

			// Moving the file to the destination directory
			destinationDir->fileList->push_back(*sourceFile);
			del_File(sourcePath);

			cout << "File moved successfully" << endl;
		}
	}


	void copy(string input) {
		// Explanation of different scenarios in the copy function:
		// When copying from a folder to a folder, all files within the source directory are copied to the destination.
		// If copying a file to a folder, the file is copied to the destination only if there is no existing file with the same name.
		// In the case of copying from a file to a file, the data is duplicated from the source file to the destination file.
		// When copying from a folder to a file, the data from all files in the source directory is copied and appended to the destination file.

		int fileCount = 0; // Counter to track the number of files copied

		// Extracting source and destination paths from the input
		string sourcePath = input.substr(0, input.find(" "));
		string destinationPath = input.substr(input.find(" ") + 1, input.length() - 1);

		// Checking for invalid syntax
		if (sourcePath.empty() || destinationPath.empty()) {
			cout << "Syntax is not valid" << endl;
			return;
		}

		// Retrieving source directory and file objects
		Directory* sourceDir = current_ptr->getDirectoryFromPath(sourcePath);
		File* sourceFile = current_ptr->findingFile(sourcePath);

		// Handling the case when the source is not found
		if (sourceDir == nullptr && sourceFile == nullptr) {
			cout << "Couldn't find the source :(" << endl;
			return;
		}

		// Retrieving destination directory and file objects
		Directory* destinationDir = current_ptr->getDirectoryFromPath(destinationPath);
		File* destinationFile = current_ptr->findingFile(destinationPath);

		// Handling the case when the destination is not found
		if (destinationFile == nullptr && destinationDir == nullptr) {
			cout << "Couldn't find the Destination :(" << endl;
			return;
		}

		// Copying files from a source directory to a destination directory
		if (sourceDir != nullptr && destinationDir != nullptr) {
			for (auto it = sourceDir->fileList->begin(); it != sourceDir->fileList->end(); ++it) {
				if (destinationDir->doesFileExist(it->name)) {
					File* temp = destinationDir->findingFile(it->name);
					temp->data = it->data;
					temp->size = it->size;
				}
				else {
					destinationDir->fileList->push_back(copyFile(&(*it)));
				}
				fileCount++;
			}
			cout << "Copied " << fileCount << " items" << endl;
		}
		// Copying a file from the source to a destination directory
		else if (sourceFile != nullptr && destinationDir != nullptr) {
			if (destinationDir->doesFileExist(sourceFile->name)) {
				File* temp = destinationDir->findingFile(sourceFile->name);
				temp->data = sourceFile->data;
				temp->size = sourceFile->size;
				cout << "copied 1 item successfully" << endl;
				return;
			}
			destinationDir->fileList->push_back(copyFile(sourceFile));
			cout << "1 item copied" << endl;
		}
		// Copying a file from the source to the destination file
		else if (sourceFile != nullptr && destinationFile != nullptr) {
			destinationFile->data = sourceFile->data;
			destinationFile->size = sourceFile->size;
			cout << "Successfully copied 1 file" << endl;
		}
		// Copying files from a source directory to the destination file
		else if (sourceDir != nullptr && destinationFile != nullptr) {
			destinationFile->data = "";
			destinationFile->size = 0;
			for (auto it = sourceDir->fileList->begin(); it != sourceDir->fileList->end(); ++it) {
				destinationFile->data += it->data;
				destinationFile->size += it->size;
				fileCount++;
			}
			cout << "Copied " << fileCount << " items" << endl;
		}
	}



	void VER()
	{
		// Displaying the program version
		cout << "Program Version: 1.0\n";
	}

	File copyFile(File* filepath) {
		// Creating a copy of the file
		return File(filepath->name, filepath->data);
	}

	void printDirectoryStructure(Directory* currentDirectory, int level)
	{
		// Printing the directory structure recursively with indentation
		for (int i = 0; i < level; ++i)
		{
			cout << "   ";
		}

		cout << "[DIR] " << currentDirectory->name << "\n";

		for (auto it = currentDirectory->directory->begin(); it != currentDirectory->directory->end(); ++it)
		{
			printDirectoryStructure(&(*it), level + 1);
		}

		for (auto it = currentDirectory->fileList->begin(); it != currentDirectory->fileList->end(); ++it)
		{
			for (int i = 0; i < level + 1; ++i)
			{
				cout << "   ";
			}

			cout << "[FILE] " << it->name << "\n";
		}
	}

	void TREE()
	{
		// Displaying the complete directory structure
		cout << "Complete Directory Structure:\n";
		printDirectoryStructure(root, 0);
	}

	void PRINT(string fileName)
	{
		// Adding a file to the print queue
		current_ptr->addingFile(fileName, "File content goes here");
		cout << "File added to the print queue: " << fileName << endl;
	}

	void PPRINT(string fileName)
	{
		// Adding a file to the priority print queue
		priorityPrintQueue.push(fileName);
		cout << "File added to the priority print queue: " << fileName << endl;
	}

	void PQUEUE()
	{
		// Displaying the priority-based print queue
		cout << "Priority Based Print Queue:" << endl;
		int count = 1;
		priority_queue<string> tempQueue = priorityPrintQueue;
		while (!tempQueue.empty())
		{
			cout << count++ << ". " << tempQueue.top() << endl;
			tempQueue.pop();
		}
	}

	void QUEUE(const list<File>* fileList)
	{
		// Displaying the print queue with time left
		cout << "Print Queue with Time Left:" << endl;

		queue<File> tempQueue;

		for (const File& file : *fileList)
		{
			tempQueue.push(file);
		}

		int count = 1;
		while (!tempQueue.empty())
		{
			File file = tempQueue.front();
			cout << count++ << ". " << file.name << " - " << file.size << " bytes" << endl;

			time_t currentTime = time(nullptr);
			time_t endTime = currentTime + (file.size / 1024); // Assuming here 1 KB per second
			cout << "   Time left: " << difftime(endTime, currentTime) << " seconds" << endl;

			tempQueue.pop();
		}
	}

	void openEditor(string filename)
	{
		try
		{
			File* file = current_ptr->findingFile(filename);
			if (file == nullptr)
			{
				cout << "File not found" << endl;
				return;
			}

			TextEditor editor;
			editor.handleKeyPress(file);
		}
		catch (const std::exception& e)
		{
			cerr << "Exception caught in openEditor: " << e.what() << endl;
		}
		catch (...)
		{
			cerr << "Unknown exception caught in openEditor." << endl;
		}
	}


	void SAVE(string fileName)
	{
		// Saving the content of a file to a new file
		File targetFile = current_ptr->getFileByName(fileName);

		if (!targetFile.name.empty())
		{
			ofstream outputFile(fileName);
			if (outputFile.is_open())
			{
				outputFile << targetFile.data;
				outputFile.close();
				cout << "File saved: " << fileName << endl;
			}
			else
			{
				cout << "Unable to save file: " << fileName << endl;
			}
		}
		else
		{
			cout << "File not found: " << fileName << endl;
		}
	}



	// Update the LOADTREE function to handle the exclusion of the root node
	void LOADTREE(const string& filename)
	{
		// Open the specified file for reading
		ifstream inputFile(filename);

		// Check if the file is successfully opened
		if (!inputFile.is_open())
		{
			cout << "Error: Unable to open file " << filename << endl;
			return;
		}

		// Clear the existing directory and file lists in the root directory
		root->directory->clear();
		root->fileList->clear();

		// Skip the first line (root node) and process each subsequent line in the file
		string line;
		getline(inputFile, line); // Skip the root node line
		int lineNumber = 1;
		while (getline(inputFile, line))
		{
			lineNumber++;
			cout << "Processing line " << lineNumber << ": " << line << endl;
			processTreeLine(line);
		}

		// Close the input file
		inputFile.close();
		cout << "Tree loaded successfully from " << filename << endl;
	}

	// Update the processTreeLine function to correctly identify directories and files
	void processTreeLine(const string& line)
	{
		if (line.empty())
		{
			cout << "Error: Empty line in tree file." << endl;
			return;
		}

		if (line[0] == 'D')
		{
			// Directory entry
			current_ptr->addingDirectory(line.substr(2), current_ptr);
		}
		else if (line[0] == 'F')
		{
			// File entry
			size_t firstSpacePos = line.find(' ', 2);
			if (firstSpacePos != string::npos)
			{
				string fileName = line.substr(2, firstSpacePos - 2);
				string fileData = line.substr(firstSpacePos + 1);

				// Create a new file with the name and data
				current_ptr->addingFile(fileName, fileData);
			}
			else
			{
				cout << "Error: Invalid file entry in tree file." << endl;
			}
		}
		else
		{
			cout << "Error: Unknown entry type in tree file. Skipping line." << endl;
		}
	}

	void SAVETREE(const string& filename)
	{
		// Open the specified file for writing
		ofstream outputFile(filename);

		// Check if the file is successfully opened
		if (!outputFile.is_open())
		{
			cout << "Error: Unable to open file " << filename << " for writing." << endl;
			return;
		}

		// Save the root node separately
		saveTreeToFile(root, outputFile, 0);

		// Close the output file
		outputFile.close();
		cout << "Tree saved successfully to " << filename << endl;

	}



	bool input_option()
	{
		cout << current_ptr->path << prompt;
		string input = take_input();

		if (input == "exit") {
			system("cls");
			return false;
		}
		else if (input == "prompt") {
			prompt = prompt == "> " ? "$ " : "> ";
		}
		else if (input == "format") {
			FORMAT();
		}
		else if (input == "dir") {
			DIR();
		}
		else if (input == "tree") {

			TREE();
		}
		else if (input == "cd.." || input == "cd ..") {
			if (current_ptr->parent_head != nullptr) {
				current_ptr = current_ptr->parent_head;
			}
		}
		else if (input == "cd/") {
			current_ptr = root;
		}
		else if (input == "cd.") {
			current_ptr->printingPath();
		}
		else if (input == "help") {
			HelperCommand();
		}
		else if (input == "cls") {
			system("cls");
			Header();
			VER();
		}
		else if (input.substr(0, 3) == "cd ") {

			Directory* temp = current_ptr->getDirectoryFromPath(input.substr(3, input.length() - 1));
			if (temp != nullptr) {
				current_ptr = temp;
			}
			else {
				cout << "Directory not found" << endl;
			}

		}
		else if (input.substr(0, 8) == "findstr ") {
			FINDSTR(input.substr(8, input.length() - 1));
		}
		else if (input.substr(0, 6) == "findf ") {
			Findf(input.substr(6, input.length() - 1));
		}
		else if (input.substr(0, 5) == "copy ") {
			copy(input.substr(5, input.length() - 1));
		}
		else if (input.substr(0, 5) == "move ") {
			MOVE(input.substr(5, input.length() - 1));
		}
		else if (input.substr(0, 7) == "attrib ") {
			ATTRIB(input.substr(7, input.length() - 1));
		}
		else if (input.substr(0, 5) == "mkdir") {
			MKDIR(input.substr(6, input.length() - 1));
		}
		else if (input.substr(0, 6) == "create") {
			createFile(input.substr(7, input.length() - 1));
		}
		else if (input.substr(0, 5) == "rmdir") {
			RMDIR(input.substr(6, input.length() - 1));
		}
		else if (input == "pwd") {
			current_ptr->printingPath();
		}
		else if (input.substr(0, 6) == "rename") {
			string oldName = input.substr(7, input.find(" ", 7) - 7);
			string newName = input.substr(input.find(" ", 7) + 1, input.length() - 1);

			if (oldName.empty() || newName.empty()) {
				cout << "Invalid syntax for rename command" << endl;
			}
			else {
				current_ptr->RENAME(oldName, newName);
			}
		}

		else if (input.substr(0, 7) == "convert")
		{
			// Assuming the syntax is "convert <old_extension> <new_extension>"
			size_t firstSpaceIndex = input.find(' ');
			if (firstSpaceIndex != string::npos)
			{
				string parameters = input.substr(firstSpaceIndex + 1);
				size_t secondSpaceIndex = parameters.find(' ');
				if (secondSpaceIndex != string::npos)
				{
					string oldExtension = parameters.substr(0, secondSpaceIndex);
					string newExtension = parameters.substr(secondSpaceIndex + 1);

					CONVERT(oldExtension, newExtension);
				}
			}
		}

		else if (input.substr(0, 3) == "del") {
			DEL(input.substr(4, input.length() - 1));
		}
		else if (input.substr(0, 5) == "find ") {
			string filename = input.substr(5, input.length() - 1);
			find(filename);
		}

		else if (input.substr(0, 5) == "print")
		{
			PRINT(input.substr(6, input.length() - 1));
		}
		else if (input.substr(0, 6) == "print")
		{
			PPRINT(input.substr(7, input.length() - 1));
		}

		else if (input.substr(0, 5) == "queue")
		{
			QUEUE(current_ptr->fileList);
		}

		else if (input.substr(0, 6) == "pqueue")
		{
			PQUEUE();
		}


		else if (input.substr(0, 5) == "edit ")
		{
			openEditor(input.substr(5, input.length() - 1));
			system("cls");
			Header();
			VER();
		}

		else {
			cout << "Invalid command" << endl;
		}
		return true;
	}

	// taking input
	string take_input()
	{
		string input;
		getline(cin, input);
		return input;
	}

	// run the ftn
	void run()
	{
		system("cls");
		Header();
		VER();
		while (input_option())
			;
	}

	// helper commands
	void HelperCommand()
	{
		SetConsoleColor(FOREGROUND_GREEN);
		cout << "attrib <file name> - Displays file attributes." << endl;
		SetConsoleColor(FOREGROUND_RED);
		cout << "cd <directory name> - Displays the name of or changes the current directory." << endl;
		SetConsoleColor(FOREGROUND_BLUE);
		cout << "cd. - Prints working directory (current directory)." << endl;
		SetConsoleColor(FOREGROUND_INTENSITY);
		cout << "cd.. - Changes directory to the previous directory." << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "cd\\ - Changes directory to the root directory (V:\\>)." << endl;
		SetConsoleColor(FOREGROUND_GREEN);
		cout << "convert -<old extension> <.new extension> " << endl;
		SetConsoleColor(FOREGROUND_RED);
		cout << "copy <source> <destination> - Copies one file in the current directory to another location." << endl;
		SetConsoleColor(FOREGROUND_BLUE);
		cout << "create <file name> - Creates a file with the provided name and allows user to enter contents." << endl;
		SetConsoleColor(FOREGROUND_INTENSITY);
		cout << "del <file name> - Deletes a file with the provided name." << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "dir - Displays a list of files and subdirectories in the current directory." << endl;
		SetConsoleColor(FOREGROUND_GREEN);
		cout << "edit <file name> - Opens a file, allows user to edit, and saves the contents of the file." << endl;
		SetConsoleColor(FOREGROUND_RED);
		cout << "exit - Quits the program." << endl;
		SetConsoleColor(FOREGROUND_BLUE);
		cout << "find <file name> - Searches for a file in the current virtual directory." << endl;
		SetConsoleColor(FOREGROUND_INTENSITY);
		cout << "findf <file name> <text> - Searches for a text string in the currently open file or a specified file." << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "findstr <text> - Searches for strings in all files of the current virtual directory." << endl;
		SetConsoleColor(FOREGROUND_GREEN);
		cout << "format - Formats the current virtual directory, empties the current directory and all subdirectories." << endl;
		SetConsoleColor(FOREGROUND_RED);
		cout << "help - Provides Help information for all commands." << endl;
		SetConsoleColor(FOREGROUND_BLUE);
		cout << "loadtree - Load a given tree in the tree structure from a file named tree.txt." << endl;
		SetConsoleColor(FOREGROUND_INTENSITY);
		cout << "mkdir <directory name> - Creates a virtual directory." << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "move <source> <destination> - Moves one file from one directory to another." << endl;
		SetConsoleColor(FOREGROUND_GREEN);
		cout << "pprint <file name> - Adds a text file to the priority-based print queue and displays the current priority queue." << endl;
		SetConsoleColor(FOREGROUND_RED);
		cout << "prompt - Changes the Windows command prompt (e.g., from V:\\> to V$)." << endl;
		SetConsoleColor(FOREGROUND_BLUE);
		cout << "PRINT <file name> - Adds a text file to the print queue and displays the current queue." << endl;
		SetConsoleColor(FOREGROUND_INTENSITY);
		cout << "PQUEUE - Shows the current state of the priority-based print queue with time left for each element." << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "pwd - Prints the working directory." << endl;
		SetConsoleColor(FOREGROUND_GREEN);
		cout << "queue - Shows the current state of the print queue with time left for each element." << endl;
		SetConsoleColor(FOREGROUND_RED);
		cout << "rename <old name> <new name> - Renames a file." << endl;
		SetConsoleColor(FOREGROUND_BLUE);
		cout << "rmdir <directory name> - Removes a directory along with its contents." << endl;
		SetConsoleColor(FOREGROUND_INTENSITY);
		cout << "save - Saves the currently open file to disk." << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "tree - Displays the complete directory structure." << endl;
		SetConsoleColor(FOREGROUND_GREEN);
		cout << "ver - Displays the version of your program." << endl;
		ResetConsoleColor();
	}

	void SetConsoleColor(int color)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}

	// Reset text color to default
	void ResetConsoleColor()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	void Header() {
		cout << "/******************************************************************************" << endl;
		cout << " *                      D O S S   S H E L L   -   P R O J E C T               *" << endl;
		cout << " *                      T E X T - E D I T O R  - P R O J E C T                *" << endl;
		cout << " *                                                                            *" << endl;
		cout << " * Course Code:     CS-153 & CS-143                                           *" << endl;
		cout << " * Academic Year:   2022                                                      *" << endl;
		cout << " * Author:          Ayesha Khalid and Sami Ullah                              *" << endl;
		cout << " *                                                                            *" << endl;
		cout << " * Description:     Welcome to the DOSS Shell, a sophisticated command-line   *" << endl;
		cout << " *                  interface designed for CS-153 & CS-143 coursework. Dive   *" << endl;
		cout << " *                  into a seamless user experience with powerful commands    *" << endl;
		cout << " *                  and intuitive functionalities.                            *" << endl;
		cout << " *****************************************************************************/" << endl;
	}
};



int main() {
	TreeNode tree;
	tree.LOADTREE("Saved_tree.txt");
	Sleep(50);
	// Run the main functionality of the tree
	tree.run();

	return 0;
}