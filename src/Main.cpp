//============================================================================
// Name        : Epidemics.cpp
// Author      : Hamid
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Person.h"
#include "Enumerated_Types.h"
#include "Utils.h"
#include <string>
#include  <random>
#include  <iterator>
#include <algorithm>
#include "Random_Element.h"

using namespace std;

int myMin(int a, int b);
void run_simulation();
void initialize_simulation();
int getRandomHouseholdSize();
string getAgents();
void addAllHomeConnections(vector<Person*> people);
void addAllWorkConnections(vector<Person*> colleagues);
void addAllSchoolConnections(vector<Person*> groupOfStudents);

vector<Person*> agents;
vector<vector<Person*>> families;
vector<vector<Person*>> schools;
vector<vector<Person*>> workPlaces;
vector<Person*> students;
vector<Person*> employees;

int meetingAtSchoolProb = 40;
int meetingAtHomeProb = 20;
int meetingAtWorkProb = 20;
int populationSize = 10000;
int initialNumberOfInfected = 1;
int period = 2000;

int main() {
	initialize_simulation();
	//write the connections into a file
	writeHouseholdToFile(families, "generated/family");
	writeSchoolsToFile(schools, "generated/school");
	writeWorkplacesToFile(workPlaces, "generated/workplace");

	run_simulation();
//	Person* r = *select_randomly(agents.begin(), agents.end());
//	todo government and intervention
	cout << "done";
	return 0;
}

void run_simulation() {
	int time_step = 0;
	string peopleInfo = "";
	string numberOfInfectiousInfo = "";
	string numberOfInfectedInfo = "";
	string numberOfCriticalCareInfo = "";
	string numberOfRecoveredInfo = "";
	string numberOfSusceptibleInfo = "";

	//the location of everybody should be specified
	while (time_step < period) {
		for (auto &person : agents) {
			person->updateCurrentLocation(time_step);

		}

		int remain = time_step % 24;
		if (remain > 8) {
			for (auto &person1 : agents) {
				switch (person1->currentLocation) {
				case atHome:
					for (auto &person2 : person1->homeConnections) {
						if (person2->currentLocation == atHome) {
							if (yesOrNo(meetingAtHomeProb)) {
								person1->meet(person2, time_step);
							}
						}
					}
					break;
				case atSchool:
					for (auto &person2 : person1->schoolConnections) {
						if (person2->currentLocation == atSchool) {
							if (yesOrNo(meetingAtSchoolProb)) {
								person1->meet(person2, time_step);
							}
						}
					}
					break;
				case atWork:
					for (auto &person2 : person1->workConnections) {
						if (person2->currentLocation == atWork) {
							if (yesOrNo(meetingAtWorkProb)) {
								person1->meet(person2, time_step);
							}
						}
					}
					break;
					//TODO: write for in community
				default:
					break;
				}
			}
		}
		for (auto &person : agents) {
			person->individual_disease_progression();
		}
		int numberOfInfectuios = 0;
		double numberOfInfected = 0;
		double numberOfCriticalCare = 0;
		int numberOfRecovered = 0;
		int numberOfSusceptible = 0;

		//print
//		peopleInfo += "time: " + to_string(time_step) + "\n";
//		for (auto &person : agents) {
//			peopleInfo += person->toString() + "\n";
//		}

//		if (time_step % 24 == 0){
//			stat += "time: " + to_string(time_step) + " number of infectious people: ";
//			for (auto &person : agents) {
//				if (person->currentState == Infectious){
//					a ++;
//				}
//			}
//			stat += to_string(a) + "\n";
//		}
//		stat += "time: " + to_string(time_step) + " number of infectious people: ";

		for (auto &person : agents) {
			if (person->currentState == Infectious) {
				numberOfInfectuios++;
				numberOfInfected++;
			} else if (person->currentState == Exposed) {
				numberOfInfected++;
			} else if (person->currentState == Recovered) {
				numberOfRecovered++;
			} else {
				numberOfSusceptible++;
			}
		}
		numberOfCriticalCare = numberOfInfected / 20;
		numberOfInfectiousInfo += to_string(numberOfInfectuios) + "\n";
		numberOfInfectedInfo += to_string(numberOfInfected) + "\n";
		numberOfCriticalCareInfo += to_string(numberOfCriticalCare) + "\n";
		numberOfRecoveredInfo += to_string(numberOfRecovered) + "\n";
		numberOfSusceptibleInfo += to_string(numberOfSusceptible) + "\n";

		time_step++;
	}
//	writeToFile(peopleInfo, "PeopleInfo");
	writeToFile(numberOfInfectiousInfo, "generated/numberOfInfectiousInfo.csv");
	writeToFile(numberOfInfectedInfo, "generated/numberOfInfectedInfo.csv");
	writeToFile(numberOfCriticalCareInfo,
			"generated/numberOfCriticalCareInfo.csv");
	writeToFile(numberOfRecoveredInfo, "generated/numberOfRecoveredInfo.csv");
	writeToFile(numberOfSusceptibleInfo,
			"generated/numberOfSusceptibleInfo.csv");
}

void initialize_simulation() {

	for (int i = 0; i < populationSize; i++) {
		Person* person = new Person(i);
		agents.push_back(person);
		person->setState(Susceptible);
	}

	for (int i = 0; i < initialNumberOfInfected; i++) {
		Person* person = *select_randomly(agents.begin(), agents.end());
		person->setState(Exposed);
	}

	//for the whole people, initialize their home connections
	//pick 20% of samples and set their status to student, then initialize their schoolConnections
	//pick 75% of samples and set their status to employees, then initialize their workConnections
	vector<Person*> agents_copy = agents;
	vector<Person*> household;
	vector<Person*> workPlace;
	vector<Person*> school;

	while (!agents.empty()) {
		int i = myMin(getRandomHouseholdSize(), agents.size());
		for (int j = 0; j < i; j++) {
			Person* person = *select_randomly(agents.begin(), agents.end());
			household.push_back(person);
			agents.erase(remove(agents.begin(), agents.end(), person),
					agents.end());
		}
		families.push_back(household);
		addAllHomeConnections(household);
		household.clear();
	}
	agents = agents_copy;

	//set status randomly
	int numOfStudents = agents.size() / 5;
	int numOfEmployees = agents.size() * 3 / 4;

	//students
	for (int j = 0; j < numOfStudents; j++) {
		Person* person = *select_randomly(agents.begin(), agents.end());
		agents.erase(remove(agents.begin(), agents.end(), person),
				agents.end());
		students.push_back(person);
		person->setStatus(student);
	}

	//employees
	for (int j = 0; j < numOfEmployees; j++) {
		Person* person = *select_randomly(agents.begin(), agents.end());
		agents.erase(remove(agents.begin(), agents.end(), person),
				agents.end());
		employees.push_back(person);
		person->setStatus(employee);
	}

//	//others
	while (!agents.empty()) {
		Person* person = *select_randomly(agents.begin(), agents.end());
		agents.erase(remove(agents.begin(), agents.end(), person),
				agents.end());
		person->setStatus(other);
	}

	agents = agents_copy;

	//partitioning employees
	vector<Person*> employees_copy = employees;
	while (!employees.empty()) {
		int i = myMin(getRandomHouseholdSize(), employees.size());
		for (int j = 0; j < i; j++) {
			Person* person = *select_randomly(employees.begin(),
					employees.end());
			workPlace.push_back(person);
			employees.erase(remove(employees.begin(), employees.end(), person),
					employees.end());
		}
		workPlaces.push_back(workPlace);
		addAllWorkConnections(workPlace);
		//household.addtoConnections
		workPlace.clear();
	}

	employees = employees_copy;

	//partitioning students
	vector<Person*> students_copy = students;
	while (!students.empty()) {
		int i = myMin(getRandomHouseholdSize(), students.size());
		for (int j = 0; j < i; j++) {
			Person* person = *select_randomly(students.begin(), students.end());
			school.push_back(person);
			students.erase(remove(students.begin(), students.end(), person),
					students.end());
		}
		schools.push_back(school);
		addAllSchoolConnections(school);
		//household.addtoConnections
		school.clear();
	}

	students = students_copy;
}

string getAgents() {
	string str = "Agents are: \n";
	for (auto &person : agents) {
		str += "id: " + to_string(person->id) + "\n";
	}
	return str;
}

//TODO: for schools and workplaces
int getRandomHouseholdSize() {
	vector<int> v;
	int percentageOfSizeOne = 30;
	int percentageOfSizeTwo = 35;
	int percentageOfSizeThree = 16;
	int percentageOfSizeFour = 14;
	int percentageOfSizeFive = 5;

	for (int i = 0; i < percentageOfSizeOne; i++) {
		v.push_back(1);
	}

	for (int i = 0; i < percentageOfSizeTwo; i++) {
		v.push_back(2);
	}

	for (int i = 0; i < percentageOfSizeThree; i++) {
		v.push_back(3);
	}

	for (int i = 0; i < percentageOfSizeFour; i++) {
		v.push_back(4);
	}

	for (int i = 0; i < percentageOfSizeFive; i++) {
		v.push_back(5);
	}

	random_shuffle(v.begin(), v.end());
	int randomNumber = *select_randomly(v.begin(), v.end());
	return randomNumber;
}

void addAllHomeConnections(vector<Person*> people) {
	vector<Person*> people_copy = people;
	for (auto &person1 : people_copy) {
		people.erase(remove(people.begin(), people.end(), person1),
				people.end());
		for (auto &person2 : people) {
			person1->addToHomeConnections(person2);
		}
		people = people_copy;
	}
}

void addAllSchoolConnections(vector<Person*> groupOfStudents) {
	vector<Person*> groupOfStudentsCopy = groupOfStudents;
	for (auto &person1 : groupOfStudentsCopy) {
		groupOfStudents.erase(
				remove(groupOfStudents.begin(), groupOfStudents.end(), person1),
				groupOfStudents.end());
		for (auto &person2 : groupOfStudents) {
			person1->addToSchoolConnections(person2);
		}
		groupOfStudents = groupOfStudentsCopy;
	}
}

void addAllWorkConnections(vector<Person*> colleagues) {
	vector<Person*> colleaguesCopy = colleagues;
	for (auto &person1 : colleaguesCopy) {
		colleagues.erase(remove(colleagues.begin(), colleagues.end(), person1),
				colleagues.end());
		for (auto &person2 : colleagues) {
			person1->addToWorkConnections(person2);
		}
		colleagues = colleaguesCopy;
	}
}



