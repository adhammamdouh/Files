#pragma once
#include <iostream>
#include <string>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Primary
{
	char ID[6];
	int ByteOffset;
};

class PrimaryFile
{
private:
	bool Flag;
	string File;
	Primary PrimaryRecord;
	ifstream _PRIMARYIN;
	fstream _PRIMARYOUT;
	vector<Primary> Record;

	bool ReadWriteHead();
	int partition(vector<Primary>& arr, int low, int high);
	void quickSort(vector<Primary>& arr, int low, int high);
	int binarySearch(int l, int r, string key);
	void ReadRecord();

public:
	PrimaryFile();
	PrimaryFile(string FILE);

	int searchByID(string search);
	int deleteByID(string search);
	void AddRecord(string instructorName, char ID[], int Byte);

	virtual ~PrimaryFile();
};