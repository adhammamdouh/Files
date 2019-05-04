#include "File.h"

File::File()
{
}

File::File(string FILE, string PrimFile, string secFile, string invertedFile) {
	this->FILE = FILE;
	Prim = new PrimaryFile(PrimFile);
	second = new secondaryFile(secFile, invertedFile);
}

void File::Start() {
	int choice;

	Display();

	while (true)
	{
		cout << "\nPlease Enter your choice : ";
		cin >> choice;
		switch (choice) {
			case 1: {
				cin.ignore();
				int byte = AddRecord();
				Prim->AddRecord(Course.InstructorName, Course.ID, byte);
				second->addRecord(Course.InstructorName, Course.ID);
				break;
			}
			case 2: {
				string IDSearch;
				cout << "Enter the ID : ";
				cin >> IDSearch;
				IDSearch[5] = '\0';
			
				OperationBySearch(IDSearch, 2);
				break;
			}
			case 3: {
				string InstructorNameSearch;
				cout << "Enter the instructor Name : ";
				cin >> InstructorNameSearch;
			
				OperationBySearch(InstructorNameSearch, 3);
				break;
			}
			case 4: {
				string IDSearch;
				cout << "Enter the ID : ";
				cin >> IDSearch;
				IDSearch[5] = '\0';

				OperationBySearch(IDSearch, 4);
				break;
			}
			case 5: {
				string InstructorNameSearch;
				cout << "Enter the instructor Name : ";
				cin >> InstructorNameSearch;

				OperationBySearch(InstructorNameSearch, 5);
				break;
			}
			case 6: {
				string IDSearch;
				cout << "Enter the ID : ";
				cin >> IDSearch;
				IDSearch[5] = '\0';
			
				cin.ignore();
				OperationBySearch(IDSearch, 6);
				break;
			}
			case 7: {
				string InstructorNameSearch;
				cout << "Enter the instructor Name : ";
				cin >> InstructorNameSearch;
			
				cin.ignore();
				OperationBySearch(InstructorNameSearch, 7);
				break;
			}
			case 8: {
				Display();
				break;
			}
			case 9: {
				return;
			}
		}
	}
}

void File::Display() {
	cout << "1) Add New course\n"
		"2) Delete course(ID)\n"
		"3) Delete course(instructor name)\n"
		"4) Print course(ID)\n"
		"5) Print course(instructor name)\n"
		"6) update course(ID)\n"
		"7) update course(instructor name)\n"
		"8) Display\n"
		"9) Exit\n";
}

int File::AddRecord() {
	cin >> Course;
	_COURSEOUT.open(FILE, ios::out | ios::app);
	_COURSEOUT.seekp(0, ios::end);
	int byte = (int)_COURSEOUT.tellp();

	const char Avail[2]{ '$','\0' };
	const char Delim[2]{ '|' ,'\0' };
	Course.RecordLength = strlen(Course.ID) + Course.InstructorName.length() + Course.CourseName.length() + sizeof(Course.Week) + 4 + 1;
	_COURSEOUT.write((char*)&Course.RecordLength, sizeof(Course.RecordLength));

	_COURSEOUT.write(Avail, 1);
	
	WriteField(Course.ID);
	WriteField(ConvertToChar(Course.CourseName));
	WriteField(ConvertToChar(Course.InstructorName));
	_COURSEOUT.write((char*)&Course.Week, sizeof(Course.Week));

	_COURSEOUT.write(Delim, 1);

	_COURSEOUT.flush();
	_COURSEOUT.close();
	
	return byte;
}

void File::WriteField(char Field[]) {
	int length = strlen(Field);
	const char Delim[2]{ '|' ,'\0' };
	_COURSEOUT.write(Field, length);
	_COURSEOUT.write(Delim, 1);
}

char* File::ConvertToChar(string &STR) {
	int Size = STR.length();
	char *temp = new char[Size + 1];
	for (int i = 0; i < Size; ++i) {
		temp[i] = STR[i];
	}
	temp[Size] = '\0';
	return temp;
}

bool File::ReadRecord() {
	_COURSEIN.read((char*)&Course.RecordLength, sizeof(Course.RecordLength));
	
	char MarkChar[2];
	_COURSEIN.read(MarkChar, 1); MarkChar[1] = '\0';
	if (MarkChar[0] != '$') {
		_COURSEIN.seekg(Course.RecordLength - 1, ios::cur);
		return 0;
	}

	CopyArray(Course.ID, ReadField(7),7);
	Course.CourseName = ConvertToString(ReadField(40));
	Course.InstructorName = ConvertToString(ReadField(40));
	_COURSEIN.read((char*)&Course.Week, sizeof(Course.Week));
	_COURSEIN.read(MarkChar, 1);
	return 1;
}

void File::CopyArray(char Destination[], char Source[], int SIZE) {
	for (int i = 0; i < SIZE; ++i) {
		Destination[i] = Source[i];
	}
}

char* File::ReadField(int SIZE) {
	char *temp = new char[SIZE];
	for (int i = 0; i < SIZE; ++i) {
		char CurChar[1];
		_COURSEIN.read(CurChar, 1);
		if (CurChar[0] == '|') {
			temp[i] = '\0';
			break;
		}
		temp[i] = CurChar[0];
	}
	return temp;
}

string File::ConvertToString(char Feild[]) {
	string temp = "";
	int SIZE = strlen(Feild);
	for (int i = 0; i < SIZE; ++i) {
		temp += Feild[i];
	}
	return temp;
}

void File::OperationBySearch(string Search, short Witcher) {
	int position = -1;
	switch (Witcher)
	{
	case 2: {
		position = Prim->deleteByID(Search);
		if (position == -1) {
			cout << "the ID doesn't exist" << endl;
			break;
		}

		DeleteRecord(position);
		break;
	}
	case 3: {
		vector<string> R;
		bool _continue = second->deleteRecord(Search, R);
		if (!_continue) {
			cout << "the Instructor name doesn't exist" << endl;
			break;
		}
		for (int i = 0; i < R.size(); ++i) {
			position = Prim->deleteByID(R[i]);
			if (position == -1)continue;

			DeleteRecord(position);
		}
		break;
	}
	case 4: {
		position = Prim->searchByID(Search);

		if (position == -1) {
			cout << "the ID name doesn't exist" << endl;
			break;
		}

		_COURSEIN.open(FILE, ios::in | ios::binary);
		_COURSEIN.seekg(position, ios::beg);
		ReadRecord();
		cout << Course << endl;
		_COURSEIN.close();
		break;
	}
	case 5: {
		vector<string> R;
		bool _continue = second->getRecord(Search, R);
		if (!_continue) {
			cout << "the Instructor name doesn't exist" << endl;
			break;
		}
		_COURSEIN.open(FILE, ios::in | ios::binary);
		for (int i = 0; i < R.size(); ++i) {
			position = Prim->searchByID(R[i]);
			if (position == -1)continue;
			_COURSEIN.seekg(position, ios::beg);
			ReadRecord();
			cout << Course << endl;
		}
		_COURSEIN.close();
		break;
	}
	case 6: {
		int byte;
		position = Prim->deleteByID(Search);
		if (position == -1) {
			cout << "the ID name doesn't exist" << endl;
			break;
		}

		DeleteRecord(position);
		byte = AddRecord();
		Prim->AddRecord(Course.InstructorName, Course.ID, byte);
		second->addRecord(Course.InstructorName, Course.ID);
		break;
	}
	case 7: {
		vector<string> R;
		string _continue = second->updateRecord(Search);
		if (_continue == "-1") {
			cout << "the Instructor name doesn't exist" << endl;
			break;
		}
		int byte;
		position = Prim->deleteByID(_continue);
		if (position == -1) break;		

		DeleteRecord(position);
		byte = AddRecord();
		Prim->AddRecord(Course.InstructorName, Course.ID, byte);
		second->addRecord(Course.InstructorName, Course.ID);
	}
	
	}
}

void File::DeleteRecord(int position) {
	int length;
	_COURSEIN.open(FILE, ios::in | ios::binary);
	_COURSEIN.seekg(position, ios::beg);
	_COURSEIN.read((char*)&length, sizeof(length));
	_COURSEIN.close();
	
	_COURSEOUT.open(FILE, ios::out | ios::in);
	_COURSEOUT.seekp(position + sizeof(length));
	
	const char DeleteChar = '*';
	_COURSEOUT.write((char*)&DeleteChar,sizeof(DeleteChar));
	
	WriteSpaces(length - 1);
	_COURSEOUT.flush();
	_COURSEOUT.close();
}

void File::WriteSpaces(int Fill) {
	const char Space = ' ';
	for (int i = 0; i < Fill; ++i) {
		_COURSEOUT.write((char*)&Space, sizeof(Space));
	}
}

char* File::ToLower(char Attribute[]) {
	int SIZE = strlen(Attribute);
	char *temp = new char[SIZE+1];
	for (int i = 0; i < SIZE; ++i) {
		if (Attribute[i] >= 'A' && Attribute[i] <= 'Z') {
			temp[i] = Attribute[i] + 32;
		}
		else temp[i] = Attribute[i];
	}
	temp[SIZE] = '\0';
	return temp;
}

File::~File()
{
	delete Prim;
	delete second;
}
