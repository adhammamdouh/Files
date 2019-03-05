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
	char Delim[1];
};

fstream _BOOK;

void Start();
short Display();
char* ToLower(char feild[]);

void ReadFromUserThenAdd(Book &BookRecord);
void WriteInFile(char Field[]);
istream& operator >> (istream &in, Book &BookRecord);

bool ReadFromFileThenDisplay(Book &BookRecord);
void ReadFromFile(char field[]);
ostream& operator << (ostream& out, Book &BookRecord);

bool OperationBySearch(char Search[], Book &BookRecord, bool Witch, bool Update);
void DeleteBook();

int main() {
	_BOOK.open("book.txt", ios::in | ios::out | ios::ate);
	
	Start();
	
	_BOOK.close();
	system("pause");
	return 0;
}

void Start() {
	while (true)
	{
		short Choice = Display();

		if (Choice == 1) {
			cin.ignore();
			_BOOK.seekp(0, ios::end);
			Book BookRecord;
			BookRecord.Delim[0] = '|';
			ReadFromUserThenAdd(BookRecord);

		}
		else if (Choice == 2) {
			char ISBN[10];
			Book BookRecord;
			cout << "Enter the ISBN of the Book you want : ";
			cin >> ISBN; ISBN[5] = '\0';
			if (!OperationBySearch(ISBN, BookRecord, 0, 0))
				cout << "=====> The ISBN you Entered Doesn't exist <=====\n";
			else {
				cout << "The following book was deleted successfully" << endl;
				cout << BookRecord << endl;
			}
		}
		else if (Choice == 3) {
			char ISBN[10];
			Book BookRecord;
			cout << "Enter the ISBN of the Book you want : ";
			cin >> ISBN; ISBN[5] = '\0';
			cin.ignore();
			if (!OperationBySearch(ISBN, BookRecord, 0, 1))
				cout << "=====> The ISBN you Entered Doesn't exist <=====\n";
			else {
				cout << "Updated successfully\nTo :\n";
				cout << BookRecord << endl;
			}
		}
		else if (Choice == 4)
		{
			char Title[100];
			Book BookRecord;
			cout << "Enter the Title of the Book you want : ";
			cin.ignore();
			cin.getline(Title, 100); Title[strlen(Title)] = '\0';
			if (!OperationBySearch(Title, BookRecord, 1, 0))
				cout << "=====> The Title you Entered Doesn't exist <=====\n";
			else
				cout << endl << BookRecord << endl;
		}

		else if (Choice == 5)
		{
			_BOOK.seekg(0, ios::beg);
			while (true)
			{
				Book BookRecord;
				if (!ReadFromFileThenDisplay(BookRecord))break;
				cout << endl << BookRecord;
				cout << "========================================================" << endl;
			}
		}
		else if (Choice == 6)
		{
			break;
		}
	}
}

short Display() {
	short choice = 0;
	cout << "1 >> Add Book\n"
		<< "2 >> Delete Book\n"
		<< "3 >> Update Book\n"
		<< "4 >> Print Book\n"
		<< "5 >> print all books\n"
		<< "6 >> Exit\n"
		<< "=> ";
	cin >> choice;
	return choice;
}

bool OperationBySearch(char Search[], Book &BookRecord, bool Witch , bool Update) {
	_BOOK.clear();
	_BOOK.seekg(ios::beg);
	_BOOK.seekp(ios::beg);
	while (true)
	{
		if (!ReadFromFileThenDisplay(BookRecord)) break;

		switch (Witch)
		{
		case 0:
			if (!strcmp(Search, BookRecord.ISBN)) {
				DeleteBook();
				if(Update)
				{
					_BOOK.seekp(0, ios::end);
					cout << "Enter the details of the book to update" << endl;
					ReadFromUserThenAdd(BookRecord);
				}
				return 1;
			}
			break;
		case 1:
			if (!strcmp(ToLower(Search), ToLower(BookRecord.Title))) {
				return 1;
			}
			break;
		}
	}
	return 0;
}

void DeleteBook() {
	char DELETE[2]{ '*','\0' };
	
	_BOOK.seekg(-1, ios::cur); int POSITION = _BOOK.tellg();
	_BOOK.seekp(POSITION, ios::beg);
	_BOOK.write(DELETE, strlen(DELETE));
}

bool ReadFromFileThenDisplay(Book &BookRecord) {
	while (true)
	{
		ReadFromFile(BookRecord.ISBN);
		ReadFromFile(BookRecord.Title);
		ReadFromFile(BookRecord.AuthorName);
		ReadFromFile(BookRecord.Price);
		ReadFromFile(BookRecord.Year);
		ReadFromFile(BookRecord.NumOfPages);
		_BOOK.read(BookRecord.Delim, sizeof(BookRecord.Delim));
		if(BookRecord.Delim[0] == '|' || _BOOK.eof() || _BOOK.fail())
			break;
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

void ReadFromUserThenAdd(Book &BookRecord) {
	cin >> BookRecord;
	int SIZE = 0;

	WriteInFile(BookRecord.ISBN);
	WriteInFile(BookRecord.Title);
	WriteInFile(BookRecord.AuthorName);
	WriteInFile(BookRecord.Price);
	WriteInFile(BookRecord.Year);
	WriteInFile(BookRecord.NumOfPages);
	BookRecord.Delim[0] = '|';
	_BOOK.write(BookRecord.Delim, sizeof(BookRecord.Delim));
}

void WriteInFile(char Field[]) {
	int SIZE = 0;
	SIZE = strlen(Field);
	_BOOK.write((char*)&SIZE, sizeof(SIZE));
	_BOOK.write(Field, SIZE);
}

char* ToLower(char feild[]) {
	int SIZE = strlen(feild);
	char* tmp = new char[SIZE+2];
	for (int i = 0; i < SIZE; ++i) {
		tmp[i] = (feild[i] >= 'A' && feild[i] <= 'A') ? feild[i] + 32 : feild[i];
	}
	return tmp;
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
