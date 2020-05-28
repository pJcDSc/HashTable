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

bool parse(char*, Node**&, int);
void printHelp();
void addStudent(Node**&, int);
void deleteStudent(Node**&);
void generateRandom(Node**&);
void printStudents(Node**, int);
bool addChain(Node**&, int, Student*);
void rehash(Node**&);
int powMod(int, int, int);
int getHash(char*, int);

int main() {
  Node** ht = NULL;
  int size = 5;
  ht = new Node*[size];
  for (int i = 0; i < size; i++) {
    ht[i] = NULL;
  }

  cout << "Welcome to student list with hashtable" << endl;
  cout << "Type help for more commands" << endl;
  bool run = true;
  char* input = new char();
  while (run) {
    cin.get(input, 30);
    cin.clear();
    cin.ignore(999, '\n');

    run = parse(input, ht, size);
  }
  return 0;
}

bool parse(char* input, Node** &ht, int sz) {
  for (int i = 0; i < strlen(input); i++) {
    input[i] = toupper(input[i]);
  }
  if (strcmp(input, "HELP") == 0) {
    printHelp();
  } else if (strcmp(input, "ADD") == 0) {
    addStudent(ht, sz);
  } else if (strcmp(input, "DELETE") == 0) {
    deleteStudent(ht);
  } else if (strcmp(input, "RANDOM") == 0) {
    generateRandom(ht);
  } else if (strcmp(input, "QUIT") == 0) {
    return false;
  } else if (strcmp(input, "PRINT") == 0) {
    printStudents(ht, sz);
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

void addStudent(Node**& ht, int sz) {
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
  int hash = (getHash(newS->firstName, sz) + getHash(newS->lastName, sz))%sz;
  cout << "hash = " << hash << endl;
  if(addChain(ht, hash, newS)) {
    cout << "its rehash time" << endl;
    rehash(ht);
  }
  cout << "Student added" << endl;
}

bool addChain(Node**& ht, int h, Student* s) {
  if (ht[h] == NULL) {
    ht[h] = new Node();
    ht[h] -> student = s;
    return false;
  }
  cout << "Collide, yo" << endl;
  int ln = 0;
  Node* link = ht[h];
  while (link -> next != NULL) {
    link = link -> next;
    ln++;
  }
  link -> next = new Node();
  link -> next -> student = s;
  return ln > 4;
}

void rehash(Node**& ht) {

}

void deleteStudent(Node**& ht) {

}

void generateRandom(Node**& ht) {
  
}

void printStudents(Node** ht, int sz) {
  bool students = false;
  for (int i = 0; i < sz; i++) {
    if (ht[i] == NULL) continue;
    Node* n = ht[i];
    students = true;
    do {
      cout << endl;
      cout << "Student: " << n->student->firstName << " " << n->student->lastName << endl;
      cout << "ID: " << n->student->id << endl;
      cout << "GPA: " << n->student->gpa << endl;
      n = n->next;
    } while (n != NULL);
  }
  if (!students) cout << "Student list is empty" << endl;
  cout << endl;
}

int powMod(int a, int b, int mod) { //Return a^b%mod for hash function, in case of long string
  int ret = 1;
  for (int i = 0; i < b; i++) {
    ret *= a;
    ret %= mod;
  }
  return ret;
}

int getHash(char* s, int sz) { //sz is size of hashtable
  int hash = 3;
  for (int i = 0; i < strlen(s); i++) {
    hash *= 769; //Big prime number oooo
    hash += int(s[i]);
    hash %= sz;
  }
  return hash;
}
