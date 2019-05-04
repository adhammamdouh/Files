#pragma once
#include <iostream>
#include <string>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "invertedList.h"

using namespace std;

struct Secondray
{
	string instructorName;
	int invertedPosition;
};

class secondaryFile
{
private:
	Secondray second;
	string File;
	ifstream _SECONDRAYIN;
	fstream _SECONDRAYOUT;
	vector<Secondray> Record;
	invertedList *list;

	int partition(vector<Secondray>& arr, int low, int high);
	void quickSort(vector<Secondray>& arr, int low, int high);
	int binarySearch(int l, int r, string key);
	void readRecords();
	void readData();
	void editInstructorName(char name[]);
	char* ConvertToChar(string &STR);
	void WriteSpaces(int fill);

public:
	secondaryFile();
	secondaryFile(string File, string invertedFile);
	void addRecord(string instructorName, char ID[]);
	bool deleteRecord(string instructorName, vector<string>& R);
	bool getRecord(string instructorName, vector<string>& R);
	string updateRecord(string key);
	virtual ~secondaryFile();
};

