#include "PrimaryFile.h"


PrimaryFile::PrimaryFile()
{
	Flag = true;
}

PrimaryFile::PrimaryFile(string File)
{
	Flag = true;
	this->File = File;
	bool read = ReadWriteHead();
	if (read) {
		_PRIMARYIN.open(File, ios::in | ios::binary);
		_PRIMARYIN.seekg(sizeof(Flag), ios::beg);
		if (_PRIMARYIN.fail() || _PRIMARYIN.peek() == EOF)return;

		while (true)
		{
			ReadRecord();
			Record.push_back(PrimaryRecord);
			if (_PRIMARYIN.fail() || _PRIMARYIN.peek() == EOF)break;
		}
		_PRIMARYIN.close();
		Flag = true;
		_PRIMARYOUT.open(File, ios::out | ios::in);
		_PRIMARYOUT.seekp(0, ios::beg);
		_PRIMARYOUT.write((char*)&Flag, sizeof(Flag));
		_PRIMARYOUT.close();
	}
}

bool PrimaryFile::ReadWriteHead() {
	_PRIMARYIN.open(File, ios::in | ios::binary);
	_PRIMARYOUT.open(File, ios::in | ios::out);
	_PRIMARYIN.seekg(0, ios::end);
	int SIZE = (int)_PRIMARYIN.tellg();
	if (SIZE == 0) {
		_PRIMARYOUT.write((char*)&Flag, sizeof(Flag));
		_PRIMARYIN.close();
		_PRIMARYOUT.close();
		return 0;
	}

	_PRIMARYIN.seekg(0, ios::beg);
	_PRIMARYIN.read((char*)&Flag, sizeof(Flag));
	if (Flag == true)cout << "the File not Up to Date!!\n";
	_PRIMARYIN.close();
	_PRIMARYOUT.close();
	return 1;
}

void PrimaryFile::ReadRecord() {
	_PRIMARYIN.read(PrimaryRecord.ID, 5);
	_PRIMARYIN.read((char*)& PrimaryRecord.ByteOffset, sizeof(PrimaryRecord.ByteOffset));
}


void PrimaryFile::AddRecord(string instructorName, char ID[], int Byte)
{
	ID[5] = '\0';
	Primary temp; strcpy_s(temp.ID, ID); temp.ByteOffset = Byte;
	
	Record.push_back(temp);
	quickSort(Record, 0, Record.size() - 1);
}

int PrimaryFile::searchByID(string search) {
	int index = binarySearch(0, Record.size() - 1, search);
	
	if (index == -1)return -1;
	return Record[index].ByteOffset;
}

int PrimaryFile::deleteByID(string search) {
	int index = binarySearch(0, Record.size() - 1, search);
	
	if (index == -1)return -1;
	
	int byte = Record[index].ByteOffset;
	Record.erase(Record.begin() + index);
	return byte;
}

int PrimaryFile::partition(vector<Primary>& arr, int low, int high){
	string pivot = arr[high].ID;
	int i = (low - 1); 

	for (int j = low; j <= high - 1; j++)
	{
		if (arr[j].ID <= pivot)
		{
			i++; 
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[high]);
	return (i + 1);
}

void PrimaryFile::quickSort(vector<Primary>& arr, int low, int high){
	if (low < high)
	{
		int pi = partition(arr, low, high);

		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

int PrimaryFile::binarySearch(int l, int r, string key)
{
	while (l <= r) {
		int m = l + (r - l) / 2;

		if (Record[m].ID == key)
			return m;

		if (Record[m].ID < key)
			l = m + 1;

		else
			r = m - 1;
	}

	return -1;
}

PrimaryFile::~PrimaryFile()
{
	_PRIMARYOUT.open(File, ios::out | ios::in | ios::trunc);
	Flag = false;
	_PRIMARYOUT.write((char*)&Flag, sizeof(Flag));

	for (int i = 0; i < Record.size(); ++i) {
		_PRIMARYOUT.write(Record[i].ID, strlen(Record[i].ID));
		_PRIMARYOUT.write((char*)&Record[i].ByteOffset, sizeof(Record[i].ByteOffset));
		_PRIMARYOUT.flush();
	}

	_PRIMARYOUT.close();
}
