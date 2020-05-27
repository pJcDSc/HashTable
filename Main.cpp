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

bool parse(char*, Node**);

int main() {
  Node* ht[5];
  for (int i = 0; i < 5; i++) {
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

bool parse(char* input, Node** ht) {
  for (int i = 0; i < strlen(input); i++) {
    input[i] = toupper(input[i]);
  }
  if (strcmp(input, "HELP") == 0) {

  } else if (strcmp(input, "ADD") == 0) {

  } else if (strcmp(input, "DELETE") == 0) {

  } else if (strcmp(input, "RANDOM") == 0) {

  } else if (strcmp(input, "QUIT") == 0) {
    return false;
  } else if (strcmp(input, "PRINT") == 0) {

  } else {
    cout << "Sorry, input not recognized" << endl;
  }
  return true;
}

