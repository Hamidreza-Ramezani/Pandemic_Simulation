/*
 * Utils.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: guest
 */

#include <iostream>
#include <cstdlib>
#include <random>
#include <string>

#include <fstream>
#include <iostream>
#include "Person.h"
#include <cstring>

using namespace std;

bool yesOrNo(double probabilityOfYes) {
	return rand() % 10000 < (probabilityOfYes * 100);
}

void writeHouseholdToFile(vector<vector<Person*>> outputVector, char* name) {
	ofstream myfile;
//	char str2[16];
//	strcpy(str2, "generated/");
//	strcat(name, str2);

    char* str1;
    char* str2;
    str2 = name;
    str1 = "generated/";
    char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
    strcpy(str3, str1);
    strcat(str3, str2);
    myfile.open(str3);

//	myfile.open(name);



	string str = "";
	for (auto family : outputVector) {
		str += "family: \n";
		for (auto &person : family) {
			str += "id: " + to_string(person->id) + "\t";
		}
		str += "\n \n \n";
	}
	myfile << str;
	myfile.close();
}

void writeSchoolsToFile(vector<vector<Person*>> outputVector, char* name) {
	ofstream myfile;

    char* str1;
    char* str2;
    str2 = name;
    str1 = "generated/";
    char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
    strcpy(str3, str1);
    strcat(str3, str2);
    myfile.open(str3);

//	char str2[16];
//	strcpy(str2, "generated/");
//	strcat(name, str2);
//
//	myfile.open(name);

	string str = "";
	for (auto group : outputVector) {
		str += "school: \n";
		for (auto &person : group) {
			str += "id: " + to_string(person->id) + "\t";
		}
		str += "\n \n \n";
	}
	myfile << str;
	myfile.close();
}


void writeWorkplacesToFile(vector<vector<Person*>> outputVector, char* name) {
	ofstream myfile;

    char* str1;
    char* str2;
    str2 = name;
    str1 = "generated/";
    char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
    strcpy(str3, str1);
    strcat(str3, str2);
    myfile.open(str3);

//	char str2[16];
//	strcpy(str2, "generated/");
//	strcat(name, str2);
//	myfile.open(name);
	string str = "";
	for (auto colleagues : outputVector) {
		str += "workplace: \n";
		for (auto &person : colleagues) {
			str += "id: " + to_string(person->id) + "\t";
		}
		str += "\n \n \n";
	}
	myfile << str;
	myfile.close();
}

void writeToFile(string data, char* name) {
	ofstream myfile;
    char* str1;
    char* str2;
    str1 = "generated/";
    str2 = name;

    char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
    strcpy(str3, str1);
    strcat(str3, str2);
    myfile.open(str3);

//	char str2[16];
//	strcpy(str2, "generated/");
//	strcat(name, str2);
//	myfile.open(name);

	myfile << data;
	myfile.close();
}




//bool flip_coin(){
//	random_device rd;
//	uniform_int_distribution<int> distribution(1, 100);
//	mt19937 engine(rd()); // Mersenne twister MT19937
//
//	int value=distribution(engine);
//	if(value > threshold) ...
//}

