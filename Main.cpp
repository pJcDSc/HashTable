/*
Author: Peter Jin
Date: 5/26/20
Hash table student list
*/

#include <iostream>
#include <cstring>

using namespace std;

struct Student {
  int id;
  float gpa;
  char firstName[25];
  char lastName[25];
};

struct Node {
  Node* next;
  Student* student;
};

bool parse(char*, Node**&);
void printHelp();
void addStudent(Node**&);
void deleteStudent(Node**&);
void generateRandom(Node**&);
void printStudents(Node**);

int main() {
  Node** ht = NULL;
  ht = new Node*[5];
  for (int i = 0; i < sizeof(ht)/sizeof(ht[0]); i++) {
    ht[i] = new Node();
  }

  cout << "Welcome to student list with hashtable" << endl;
  cout << "Type help for more commands" << endl;
  bool run = true;
  char* input = new char();
  while (run) {
    cin.get(input, 30);
    cin.clear();
    cin.ignore(999, '\n');

    run = parse(input, ht);
  }
  return 0;
}

bool parse(char* input, Node** &ht) {
  for (int i = 0; i < strlen(input); i++) {
    input[i] = toupper(input[i]);
  }
  if (strcmp(input, "HELP") == 0) {
    printHelp();
  } else if (strcmp(input, "ADD") == 0) {
    addStudent(ht);
  } else if (strcmp(input, "DELETE") == 0) {
    deleteStudent(ht);
  } else if (strcmp(input, "RANDOM") == 0) {
    generateRandom(ht);
  } else if (strcmp(input, "QUIT") == 0) {
    return false;
  } else if (strcmp(input, "PRINT") == 0) {
    printStudents(ht);
  } else {
    cout << "Sorry, input not recognized" << endl;
  }
  return true;
}


void printHelp() {
  cout << "StudentList help" << endl;
  cout << "HELP: Print this help message" << endl;
  cout << "ADD: Add a student to the student list" << endl;
  cout << "DELETE: Delete a student from the student list" << endl;
  cout << "RANDOM: Add a number of random students to the student list" << endl;
  cout << "QUIT: Quit the program" << endl;
}

void addStudent(Node**& ht) {
  Student* newS = new Student();
  cout << "Enter student first name" << endl;
  cin.get(newS->firstName, 25);
  cin.clear();
  cin.ignore(999, '\n');
  cout << "Enter student last name" << endl;
  cin.get(newS->lastName, 25);
  cin.clear();
  cin.ignore(999, '\n');
  cout << "Enter student id" << endl;
  cin >> newS->id;
  cin.clear();
  cin.ignore(999, '\n');
  cout << "Enter student gpa" << endl;
  cin >> newS->gpa;
  cin.clear();
  cin.ignore(999, '\n');
  //Something something add student
}

void deleteStudent(Node**& ht) {

}

void generateRandom(Node**& ht) {

}

void printStudents(Node** ht) {

}
