#include "secondaryFile.h"



int secondaryFile::partition(vector<Secondray>& arr, int low, int high)
{
	string pivot = arr[high].instructorName;
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++)
	{
		if (arr[j].instructorName <= pivot)
		{
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[high]);
	return (i + 1);
}

void secondaryFile::quickSort(vector<Secondray>& arr, int low, int high)
{
	if (low < high)
	{
		int pi = partition(arr, low, high);

		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

int secondaryFile::binarySearch(int l, int r, string key)
{
	while (l <= r) {
		int m = l + (r - l) / 2;

		if (Record[m].instructorName == key)
			return m;

		if (Record[m].instructorName < key)
			l = m + 1;

		else
			r = m - 1;
	}

	return -1;
}

secondaryFile::secondaryFile()
{
}

secondaryFile::secondaryFile(string File,string invertedFile) {
	this->File = File;
	list = new invertedList(invertedFile);
	readData();
}

void secondaryFile::readData()
{
	_SECONDRAYIN.open(File, ios::in | ios::binary);
	_SECONDRAYIN.seekg(0, ios::end);
	int size = (int)_SECONDRAYIN.tellg();
	if (size == 0) { 
		_SECONDRAYIN.close();
		return; 
	}

	readRecords();

	_SECONDRAYIN.close();
}

void secondaryFile::readRecords() {
	_SECONDRAYIN.seekg(0, ios::beg);
	while (true)
	{
		char temp[20];
		_SECONDRAYIN.read(temp, 20);
		this->editInstructorName(temp);
		second.instructorName = temp;
		_SECONDRAYIN.read((char*)&second.invertedPosition, sizeof(second.invertedPosition));

		Record.push_back(second);

		if (_SECONDRAYIN.peek() == EOF)break;
	}
}

void secondaryFile::editInstructorName(char name[]) {
	for (int i = 19; i >= 0; --i) {
		if (name[i] == ' ')
			name[i] = '\0';
		else break;
	}
}

char* secondaryFile::ConvertToChar(string &STR) {
	int Size = STR.length();
	char *temp = new char[Size + 1];
	for (int i = 0; i < Size; ++i) {
		temp[i] = STR[i];
	}
	temp[Size] = '\0';
	return temp;
}

void secondaryFile::WriteSpaces(int fill) {
	const char Space = ' ';
	for (int i = 0; i < fill; ++i) {
		_SECONDRAYOUT.write((char*)&Space, sizeof(Space));
	}
}

string secondaryFile::updateRecord(string key)
{
	int index = binarySearch(0, Record.size(), key);
	if (index == -1)return "-1";
	vector<string> R;
	list->getAll(Record[index].invertedPosition, R);
	cout << "chose the ID you want to update" << endl;
	for (int i = 0; i < R.size(); ++i) {
		cout << i + 1 << " >> " << R[i] << endl;
	}
	int i;
	cin >> i;
	cin.ignore();
	return R[i - 1];
}

void secondaryFile::addRecord(string instructorName, char ID[]) {
	
	int index = binarySearch(0, Record.size() - 1, instructorName);

	if (index != -1) {
		int byte = list->getAddByte();
		list->addRecord(ID, Record[index].invertedPosition);
		Record[index].invertedPosition = byte;
		return;
	}
	else if(index != -1 && Record[index].invertedPosition == -1)
	{
		Record[index].invertedPosition = list->getAddByte();
		list->addRecord(ID, -1);
	}
	else
	{
		Secondray temp;
		temp.instructorName = instructorName;
		temp.invertedPosition = list->getAddByte();

		Record.push_back(temp);
		list->addRecord(ID, -1);
	}
	quickSort(Record, 0, Record.size() - 1);
}

bool secondaryFile::deleteRecord(string instructorName,vector<string>& R) {
	int index = binarySearch(0, Record.size() - 1,instructorName);
	
	if (index == -1)return 0;
	list->getAll(Record[index].invertedPosition, R);
	Record[index].invertedPosition = -1;
	return 1;
}

bool secondaryFile::getRecord(string instructorName, vector<string>& R)
{
	int index = binarySearch(0, Record.size() - 1, instructorName);
	if (index == -1)return 0;
	list->getAll(Record[index].invertedPosition, R);
}

secondaryFile::~secondaryFile()
{
	_SECONDRAYOUT.open(File, ios::in | ios::out | ios::trunc);
	for (int i = 0; i < Record.size(); ++i) {
		
		char temp[20];
		strcpy_s(temp, ConvertToChar(Record[i].instructorName));
		_SECONDRAYOUT.write(temp, strlen(temp));
		WriteSpaces(20 - strlen(temp));
		
		_SECONDRAYOUT.write((char*)&Record[i].invertedPosition, sizeof(Record[i].invertedPosition));
		_SECONDRAYOUT.flush();
	}
	_SECONDRAYOUT.close();

	delete list;
}
