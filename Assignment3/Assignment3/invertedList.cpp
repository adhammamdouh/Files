#include "invertedList.h"
#include <vector>

invertedList::invertedList()
{
}

invertedList::invertedList(string file) {
	this->file = file;
	recordSize = sizeof(listRecord.link) + 5;
	_LISTIN.open(file, ios::in | ios::binary);
	if (_LISTIN.peek() == EOF)return;
	readData();
	_LISTIN.close();
}

void invertedList::readData() {
	_LISTIN.seekg(0, ios::beg);
	while (true)
	{
		_LISTIN.read(listRecord.ID, 5);
		_LISTIN.read((char*)&listRecord.link, sizeof(listRecord.link));
		Record.push_back(listRecord);

		if (_LISTIN.peek() == EOF)break;
	}
}

int invertedList::getRecordSize() {
	return recordSize;
}

int invertedList::getAddByte() {
	return Record.size();
}

void invertedList::addRecord(char ID[], int link)
{
	inverted temp;
	strcpy_s(temp.ID, ID);
	temp.link = link;

	Record.push_back(temp);
}

void invertedList::getAll(int begin,vector<string>& R) {
	int end = 0;
	end = begin;
	while (end != -1)
	{
		R.push_back(Record[end].ID);
		end = Record[end].link;
	}
}

invertedList::~invertedList()
{
	_LISTOUT.open(file, ios::in | ios::out | ios::trunc);
	for (int i = 0; i < Record.size(); ++i) {
		_LISTOUT.write(Record[i].ID, 5);
		_LISTOUT.write((char*)&Record[i].link, sizeof(Record[i].link));
		_LISTOUT.flush();
	}
	_LISTOUT.close();
}
