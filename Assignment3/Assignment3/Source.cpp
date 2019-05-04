#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "File.h"

using namespace std;

int main() {

	File *Course = new File("Course.txt", "Primary.txt", "Secondary.txt", "InvertedList.txt");
	Course->Start();
	delete Course;
	system("pause");
	return 0;
}

