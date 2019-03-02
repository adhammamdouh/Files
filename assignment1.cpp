#include <iostream>
#include <fstream>
#include <cstring>


using namespace std;

struct Book
{
	char ISBN[10];
	char Title[100];
	char AuthorName[100];
	char Price[30];
	char Year[10];
	char NumOfPages[20];
	char Delim[1]{ '\n' };
};

fstream _BOOK;

short Display();

void ReadFromUserThenAdd();
void WriteInFile(char Field[]);
istream& operator >> (istream &in, Book &BookRecord);

bool ReadFromFileThenDisplay(Book &BookRecord);
void ReadFromFile(char field[]);
ostream& operator << (ostream& out, Book &BookRecord);

bool OperationBySearch(char Search[], Book &BookRecord, bool Witch);
void WriteInPosition(int Here, char DELETE[]);

int main() {
	_BOOK.open("book.txt", ios::in | ios::out );
	while (true)
	{
		short Choice = Display();
		
		if (Choice == 1) {
			cin.ignore();
			;
			for (int i = 0; i < 2; ++i) {
				ReadFromUserThenAdd();
			}
		}
		else if (Choice == 2) {
			_BOOK.seekg(0, ios::beg);
			_BOOK.seekp(0, ios::beg);
			while (true)
			{
				Book BookRecord;
				if (!ReadFromFileThenDisplay(BookRecord))break;
				//cout << BookRecord;
				cout << "========================================================" << endl;
			}
		}
		else if (Choice == 3) {
			char ISBN[10];
			Book BookRecord;
			cout << "ENter the ISBN of the Book you want : ";
			cin >> ISBN; ISBN[5] = '\0';
			if (!OperationBySearch(ISBN, BookRecord, 0)) {
				cout << "The ISBN you Entered Doesn't exist\n";
			}
			else
			{
				//cout << BookRecord;

			}
		}


	}
	system("pause");
	return 0;
}

short Display() {
	short choice = 0;
	cout<< "1 >> Add Book\n"
		<< "2 >> Delete Book\n" 
		<< "3 >> Update Book\n"
		<< "4 >> Print Book\n"
		<< "5 >> print all books\n"
		<< "=> ";
	cin >> choice;
	return choice;
}

bool OperationBySearch(char Search[] , Book &BookRecord , bool Witch) {
	_BOOK.seekg(ios::beg);
	_BOOK.seekp(ios::beg);
	cout << _BOOK.tellg() << "  " << _BOOK.tellp() << endl;
	while (true)
	{
		if (!ReadFromFileThenDisplay(BookRecord)) {
			cout << "10" << endl;
			break;
		}

		switch (Witch)
		{
		case 0:
			if (!strcmp(Search, BookRecord.ISBN)) {
				_BOOK.seekg(-1, ios::cur); int POSITION = _BOOK.tellg();
				char DELETE[2]{ '*','\0'};
				cout << "1" << endl;
				WriteInPosition(POSITION ,DELETE);
				cout << "2" << endl;
				return 1;
			}
			break;
		case 1:
			if (!strcmp(Search, BookRecord.Title)) {
				return 1;
			}
			break;
		}
	}
	return 0;
}

void WriteInPosition(int Here , char DELETE[]) {
	_BOOK.seekp(Here, ios::beg);
	_BOOK.write(DELETE, strlen(DELETE));
}

bool ReadFromFileThenDisplay(Book &BookRecord) {
	cout << _BOOK.tellg() << "  " << _BOOK.tellp() << endl;
	while (true)
	{
		ReadFromFile(BookRecord.ISBN);
		ReadFromFile(BookRecord.Title);
		ReadFromFile(BookRecord.AuthorName);
		ReadFromFile(BookRecord.Price);
		ReadFromFile(BookRecord.Year);
		ReadFromFile(BookRecord.NumOfPages);
		_BOOK.read(BookRecord.Delim, sizeof(BookRecord.Delim));
		if (BookRecord.Delim[0] == '\n' || _BOOK.eof() || _BOOK.fail()) {
			cout << BookRecord;
			break;
		}
	}
	
	if (_BOOK.eof() || _BOOK.fail()) return 0;
	
	return 1;
}

void ReadFromFile(char field[]) {
	int SIZE = 0;
	_BOOK.read((char*)&SIZE, sizeof(SIZE));
	_BOOK.read(field, SIZE);
	field[SIZE] = '\0';
}

void ReadFromUserThenAdd() {
	Book BookRecord;
	cin >> BookRecord;
	int SIZE = 0;

	WriteInFile(BookRecord.ISBN);
	WriteInFile(BookRecord.Title);
	WriteInFile(BookRecord.AuthorName);
	WriteInFile(BookRecord.Price);
	WriteInFile(BookRecord.Year);
	WriteInFile(BookRecord.NumOfPages);
	_BOOK.write(BookRecord.Delim, sizeof(BookRecord.Delim));
}

void WriteInFile(char Field[]) {
	int SIZE = 0;
	SIZE = strlen(Field);
	_BOOK.write((char*)&SIZE, sizeof(SIZE));
	_BOOK.write(Field, SIZE);
}

istream& operator >> (istream &in, Book &BookRecord) {
	cout << "Enter the ISBN : ";
	in.getline(BookRecord.ISBN, 10);
	cout << "Enter the Title : ";
	in.getline(BookRecord.Title, 100);
	cout << "Enter the Author Name : ";
	in.getline(BookRecord.AuthorName, 100);
	cout << "Enter the Price : ";
	in.getline(BookRecord.Price, 30);
	cout << "Enter the Year : ";
	in.getline(BookRecord.Year, 6);
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
