#pragma once
#include <iostream>
#include <string>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct inverted
{
	char ID[6];
	int link;
};

class invertedList
{
private:
	string file;
	ifstream _LISTIN;
	fstream _LISTOUT;
	inverted listRecord;
	vector<inverted> Record;
	int recordSize;

	void readData();
public:
	invertedList();
	invertedList(string File);

	int getRecordSize();
	int getAddByte();
	void addRecord(char ID[], int link);
	void getAll(int begin, vector<string>& R);
	
	virtual ~invertedList();
};

