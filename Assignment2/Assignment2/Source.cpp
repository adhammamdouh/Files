#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int RecordSize = 100;

struct Book
{
	Book() :ISBN(""), Title(""), AuthorName(""), Price(""), Year(""), NumOfPages(""), Delim('|') {}
	char ISBN[9];
	char Title[25];
	char AuthorName[25];
	char Price[10];
	char Year[10];
	char NumOfPages[20];
	char Delim;
};

int Head = -1;

void Start();
void DisplayMenu();
void ReadWriteHead(fstream &_BOOK);

void AddRecord(fstream &_BOOK);
void Insert(fstream &_BOOK, Book &BookRecord);
void ReadFromUserThenAdd(fstream &BookToWrite, bool ReadFromUser, Book &BookRecord);
int WriteInFile(fstream &_BOOK, char feild[]);
void WriteSpaces(fstream &_BOOK, int Fill);

void ReadAll(fstream &_BOOK);
bool ReadFromFilethenDisplay(fstream &_BOOK, Book &BookRecord);
int ReadFromFile(fstream &_BOOK, char feild[], int SIZE);

bool OperationBySearch(fstream &_BOOk, char Search[], Book &BookRecord, short Witch);
void PrintUpdate(Book &LastRecord, Book &NewRecord);
char* ToLower(char feild[]);
void DeleteBook(fstream &_BOOk);

void Compact(fstream &_BOOK);
void TRUNC(fstream &_BOOK, vector<Book> BookRecords);

istream& operator >> (istream &in, Book &BookRecord);
ostream& operator << (ostream& out, Book &BookRecord);

int main() {
	Start();

	system("pause");
	return 0;
}

void Start() {
	fstream _BOOK;

	Book BookRecord; BookRecord.Delim = '|';

	_BOOK.open("Book.txt", ios::in | ios::out);
	ReadWriteHead(_BOOK);
	_BOOK.flush();
	_BOOK.close();

	int choice;
	DisplayMenu();

	while (true)
	{
		cout << "Enter the Operation you want > ";
		cin >> choice;

		switch (choice) {
		case 1: {
			AddRecord(_BOOK);
			break;
		}
		case 2: {
			char Title[25];
			cout << "Enter the Title of the Book you want : ";
			cin.ignore();
			cin.getline(Title, 25); Title[strlen(Title)] = '\0';
			if (!OperationBySearch(_BOOK, Title, BookRecord, 1))
				cout << "=====> The Title you Entered Doesn't exist <=====\n";
			else
				cout << endl << BookRecord << endl;
			break;
		}
		case 3: {
			char ISBN[10];
			cout << "Enter the ISBN of the Book you want : ";
			cin >> ISBN; ISBN[5] = '\0';
			if (!OperationBySearch(_BOOK, ISBN, BookRecord, 0))
				cout << "=====> The ISBN you Entered Doesn't exist <=====\n";
			else {
				cout << "The following book was deleted successfully" << endl;
				cout << BookRecord << endl;
			}
			break;
		}
		case 4: {
			char ISBN[10];
			Book BookRecord;
			cout << "Enter the ISBN of the Book you want : ";
			cin >> ISBN; ISBN[5] = '\0';
			cin.ignore();
			if (!OperationBySearch(_BOOK, ISBN, BookRecord, 2))
				cout << "=====> The ISBN you Entered Doesn't exist <=====\n";
			break;
		}
		case 5: {
			ReadAll(_BOOK);
			break;
		}
		case 6: {
			Compact(_BOOK);
			break;
		}
		case 7: {
			DisplayMenu();
			break;
		}
		case 8: {
			return;
		}
		}
	}
}

void DisplayMenu() {
	cout << "1 >> Add Book\n"
		<< "2 >> Print Book(Title)\n"
		<< "3 >> Delete Book(ISBN)\n"
		<< "4 >> Update Book(ISBN)\n"
		<< "5 >> Print All\n"
		<< "6 >> Compact\n"
		<< "7 >> Diplay Menu\n"
		<< "8 >> Exit\n";
}

void ReadWriteHead(fstream &_BOOK) {
	_BOOK.seekg(0, ios::end);
	int SIZE = _BOOK.tellg();
	if (SIZE - 4 < 0) _BOOK.write((char*)&Head, sizeof(Head));
	else
	{
		_BOOK.seekg(0, ios::beg);
		_BOOK.read((char*)&Head, sizeof(Head));
	}
}

void AddRecord(fstream &_BOOK) {
	_BOOK.open("Book.txt", ios::in | ios::out);
	_BOOK.clear();
	Book BookRecord;
	cin.ignore();

	if (Head == -1) {
		_BOOK.seekp(0, ios::end);
		ReadFromUserThenAdd(_BOOK, 1, BookRecord);
	}
	else
		Insert(_BOOK, BookRecord);
	_BOOK.flush();
	_BOOK.close();
}

void Insert(fstream &_BOOK, Book &BookRecord) {
	char CharMark = '$';
	int Position = (RecordSize * Head) + 4;
	_BOOK.seekp(Position, ios::beg);
	_BOOK.seekg(Position, ios::beg);

	_BOOK.read((char*)&CharMark, sizeof(CharMark));
	_BOOK.read((char*)&Head, sizeof(Head));
	_BOOK.seekp(Position, ios::beg);
	ReadFromUserThenAdd(_BOOK, 1, BookRecord);

	_BOOK.seekp(0, ios::beg);
	_BOOK.write((char*)&Head, sizeof(Head));
}

void ReadFromUserThenAdd(fstream &_BOOK, bool ReadFromUser, Book &BookRecord) {
	int UsedINRecord = 0;
	const char MarkChar[2]{ '$','\0' };
	if (ReadFromUser) cin >> BookRecord;
	_BOOK.write(MarkChar, 1); UsedINRecord++;
	UsedINRecord += WriteInFile(_BOOK, BookRecord.ISBN);
	UsedINRecord += WriteInFile(_BOOK, BookRecord.Title);
	UsedINRecord += WriteInFile(_BOOK, BookRecord.AuthorName);
	UsedINRecord += WriteInFile(_BOOK, BookRecord.Price);
	UsedINRecord += WriteInFile(_BOOK, BookRecord.Year);
	UsedINRecord += WriteInFile(_BOOK, BookRecord.NumOfPages);
	WriteSpaces(_BOOK, RecordSize - UsedINRecord);
}

int WriteInFile(fstream &_BOOK, char feild[]) {
	int SIZE = strlen(feild);
	const char Delim[2]{ '|','\0' };
	_BOOK.write(feild, SIZE);
	_BOOK.write(Delim, 1);
	return SIZE + 1;
}

void WriteSpaces(fstream &_BOOK, int Fill) {
	const char Space[2] = { ' ' , '\0' };
	for (int i = 0; i < Fill; ++i)
		_BOOK.write(Space, 1);
}

void ReadAll(fstream &_BOOK) {
	_BOOK.open("Book.txt", ios::in | ios::out);
	_BOOK.seekg(4, ios::beg);

	Book BookRecord;
	while (true)
	{
		bool Print = ReadFromFilethenDisplay(_BOOK, BookRecord);

		if (_BOOK.peek() == EOF || _BOOK.fail() || _BOOK.eof()) {
			if (Print) {
				cout << BookRecord;
				cout << "==========================================================" << endl;
			}
			break;
		}
		if (Print) {
			cout << BookRecord;
			cout << "==========================================================" << endl;
		}
	}
	_BOOK.flush();
	_BOOK.close();
}

bool ReadFromFilethenDisplay(fstream &_BOOK, Book &BookRecord) {
	int ReadFeildsSize = 0;
	char MarkChar[2];
	_BOOK.read(MarkChar, 1); MarkChar[1] = '\0'; ReadFeildsSize++;
	if (MarkChar[0] != '$') { _BOOK.seekg(RecordSize - 1, ios::cur); return 0; }

	ReadFeildsSize += ReadFromFile(_BOOK, BookRecord.ISBN, 9);
	ReadFeildsSize += ReadFromFile(_BOOK, BookRecord.Title, 25);
	ReadFeildsSize += ReadFromFile(_BOOK, BookRecord.AuthorName, 25);
	ReadFeildsSize += ReadFromFile(_BOOK, BookRecord.Price, 10);
	ReadFeildsSize += ReadFromFile(_BOOK, BookRecord.Year, 10);
	ReadFeildsSize += ReadFromFile(_BOOK, BookRecord.NumOfPages, 20);

	_BOOK.seekg(RecordSize - ReadFeildsSize, ios::cur);
	return 1;
}

int ReadFromFile(fstream &_BOOK, char feild[], int SIZE) {
	for (int i = 0; i < SIZE; ++i) {
		char CurChar[1];
		_BOOK.read(CurChar, 1);
		if (CurChar[0] == '|') {
			feild[i] = '\0';
			break;
		}
		feild[i] = CurChar[0];
	}
	return strlen(feild) + 1;
}

bool OperationBySearch(fstream &_BOOK, char Search[], Book &BookRecord, short Witch) {
	_BOOK.open("Book.txt", ios::in | ios::out);
	_BOOK.seekg(4, ios::beg);
	_BOOK.seekp(4, ios::beg);
	while (true)
	{
		while (!ReadFromFilethenDisplay(_BOOK, BookRecord)) {}

		switch (Witch)
		{
		case 0:
			if (!strcmp(Search, BookRecord.ISBN)) {
				int Position = _BOOK.tellg();
				DeleteBook(_BOOK);
				_BOOK.flush();
				_BOOK.close();
				return 1;
			}
			break;
		case 1:
			if (!strcmp(ToLower(Search), ToLower(BookRecord.Title))) {
				_BOOK.flush();
				_BOOK.close();
				return 1;
			}
			break;
		case 2:
			if (!strcmp(Search, BookRecord.ISBN)) {
				Book LastRecord = BookRecord;
				_BOOK.seekp((-1 * RecordSize), ios::cur);
				cout << "Enter the details of the book to update" << endl;
				ReadFromUserThenAdd(_BOOK, 1, BookRecord);
				PrintUpdate(LastRecord, BookRecord);
				_BOOK.flush();
				_BOOK.close();
				return 1;
			}
			break;
		}

		if (_BOOK.eof() || _BOOK.fail() || _BOOK.peek() == EOF) { break; }
	}
	_BOOK.flush();
	_BOOK.close();
	return 0;
}

void PrintUpdate(Book &LastRecord, Book &NewRecord) {
	cout << "\nUpdated successfully" << endl;
	cout << LastRecord.ISBN << " => " << NewRecord.ISBN << endl;
	cout << LastRecord.Title << " => " << NewRecord.Title << endl;
	cout << LastRecord.AuthorName << " => " << NewRecord.AuthorName << endl;
	cout << LastRecord.Price << " => " << NewRecord.Price << endl;
	cout << LastRecord.Year << " => " << NewRecord.Year << endl;
	cout << LastRecord.NumOfPages << " => " << NewRecord.NumOfPages << endl;
	cout << "==========================================================" << endl;
}

void DeleteBook(fstream &_BOOK) {
	char DELETE[2]{ '*','\0' };

	_BOOK.seekg(-1 * RecordSize, ios::cur); int POSITION = _BOOK.tellg();
	_BOOK.seekp(POSITION, ios::beg);
	_BOOK.write(DELETE, strlen(DELETE));
	_BOOK.write((char*)&Head, sizeof(Head));

	Head = ((int)_BOOK.tellp() - 4) / RecordSize;

	WriteSpaces(_BOOK, RecordSize - ((sizeof(Head) + strlen(DELETE))));


	_BOOK.seekp(0, ios::beg);
	_BOOK.write((char*)&Head, sizeof(Head));
	_BOOK.flush();
	cout << _BOOK.tellp() << endl;
}

char* ToLower(char feild[]) {
	int SIZE = strlen(feild);
	char* tmp = new char[SIZE + 2];
	for (int i = 0; i < SIZE; ++i) {
		tmp[i] = (feild[i] >= 'A' && feild[i] <= 'A') ? feild[i] + 32 : feild[i];
	}
	tmp[SIZE] = '\0';
	return tmp;
}

void Compact(fstream &_BOOK) {
	_BOOK.open("Book.txt", ios::in | ios::out);

	Book BookRecord;
	vector<Book> BookRecords;

	_BOOK.seekg(4, ios::beg);
	while (_BOOK.peek() != EOF)
	{
		if (ReadFromFilethenDisplay(_BOOK, BookRecord) == 1) {
			BookRecords.push_back(BookRecord);
		}
	}
	_BOOK.flush();
	_BOOK.close();
	TRUNC(_BOOK, BookRecords);
}

void TRUNC(fstream &_BOOK, vector<Book> BookRecords) {
	_BOOK.close();
	Head = -1;
	_BOOK.open("Book.txt", ios::out | ios::in | ios::trunc);
	ReadWriteHead(_BOOK);
	for (int i = 0; i < BookRecords.size(); ++i)
		ReadFromUserThenAdd(_BOOK, 0, BookRecords[i]);
	_BOOK.flush();
	_BOOK.close();
}

istream& operator >> (istream &in, Book &BookRecord) {
	cout << "Enter the ISBN : ";
	in.getline(BookRecord.ISBN, 9);
	cout << "Enter the Title : ";
	in.getline(BookRecord.Title, 25);
	cout << "Enter the Author Name : ";
	in.getline(BookRecord.AuthorName, 25);
	cout << "Enter the Price : ";
	in.getline(BookRecord.Price, 10);
	cout << "Enter the Year : ";
	in.getline(BookRecord.Year, 10);
	cout << "Enter the Number of Pages : ";
	in.getline(BookRecord.NumOfPages, 20);
	return in;
}
ostream& operator << (ostream& out, Book &BookRecord) {
	cout << "ISBN : " << BookRecord.ISBN << endl;
	cout << "Title : " << BookRecord.Title << endl;
	cout << "Author Name : " << BookRecord.AuthorName << endl;
	cout << "Price : " << BookRecord.Price << endl;
	cout << "Year : " << BookRecord.Year << endl;
	cout << "Number Of pages : " << BookRecord.NumOfPages << endl;
	return out;
}