#pragma once
#include <iostream>
#include <string>
#include <string>
#include <fstream>
#include "PrimaryFile.h"
#include "secondaryFile.h"

using namespace std;

struct Courses {
	int RecordLength;
	char ID[6];
	string CourseName;
	string InstructorName;
	int Week;
};

class File
{
private:
	string FILE;
	Courses Course;
	ifstream _COURSEIN;
	fstream _COURSEOUT;
	PrimaryFile* Prim;
	secondaryFile *second;

	void Display();

	int AddRecord();
	void WriteField(char Field[]);
	char* ConvertToChar(string &STR);

	bool ReadRecord();
	void CopyArray(char Destination[], char Source[], int SIZE);
	char* ReadField(int SIZE);
	string ConvertToString(char Feild[]);

	void OperationBySearch(string Search, short Witcher);
	void DeleteRecord(int position);
	void WriteSpaces(int Fill);

	char* ToLower(char Attribute[]);

public:
	File();
	File(string FILE, string PrimFile, string secFile, string invertedFile);

	void Start();
	
	friend istream& operator >> (istream& in, Courses& course);
	friend ostream& operator << (ostream& out, const Courses& course);

	virtual ~File();
}; 

inline istream & operator >> (istream & in, Courses & course) {
	cout << "Enter the ID : ";
	in.getline(course.ID, 7); course.ID[5] = '\0';
	cout << "Enter the Course Name : ";
	getline(in, course.CourseName);
	cout << "Enter the Instructor Name : ";
	getline(in, course.InstructorName);
	cout << "Enter the Week : ";
	in >> course.Week;
	return in;
}

inline ostream & operator << (ostream & out, Courses & course) {
	out << "ID : " << course.ID << endl;
	out << "Course Name : " << course.CourseName << endl;
	out << "Instructor Name : " << course.InstructorName << endl;
	out << "Week : " << course.Week << endl;
	return out;
}