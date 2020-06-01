
/*
Author: Peter Jin
Date: 5/26/20
Hash table student list
*/

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>  //file input
#include <stdlib.h> //rand

struct Student {
  int id;
  float gpa;
  char firstName[25];
  char lastName[25];

  bool operator==(const Student &o) {
    return o.id == this->id && strcmp(o.firstName, this->firstName) == 0 && strcmp(o.lastName, this->lastName) == 0;
  }
};

struct Node {
  Node* next;
  Student* student;
};

using namespace std;

bool parse(char*, Node**&, int&, int&);
void printHelp();
void addStudent(Node**&, int&, int&);
void deleteStudent(Node**&, int, int&);
void generateRandom(Node**&);
void printStudents(Node**, int);
int addChain(Node**&, int, Student*);
void rehash(Node**&, int&);
int powMod(int, int, int);
int getHash(char*, int);
bool readnames(vector<char*>*, char*);

int main() {
  Node** ht = NULL;
  int size = 5;
  int numStudents = 0;
  ht = new Node*[size];
  for (int i = 0; i < size; i++) {
    ht[i] = NULL;
  }
  
  vector<char*>* fnames = new vector<char*>();
  vector<char*>* lnames = new vector<char*>();
  char* fileName = new char(); strcpy(fileName, "fnms.txt");
  if (!readnames(fnames, fileName)) return 1;
  strcpy(fileName, "lnms.txt");
  if (!readnames(lnames, fileName)) return 1;
  
  srand(time(NULL));
  
  cout << "Welcome to student list with hashtable" << endl;
  cout << "Type help for more commands" << endl;
  bool run = true;
  char* input = new char();
  while (run) {
    cin.get(input, 30);
    cin.clear();
    cin.ignore(999, '\n');

    cout << "Ht: " << ht << ", size: " << size << ", students: " << numStudents << endl;
    run = parse(input, ht, size, numStudents);
  }
  return 0;
}

bool parse(char* input, Node** &ht, int &sz, int &ns) {
  for (int i = 0; i < strlen(input); i++) {
    input[i] = toupper(input[i]);
  }
  if (strcmp(input, "HELP") == 0) {
    printHelp();
  } else if (strcmp(input, "ADD") == 0) {
    addStudent(ht, sz, ns);
  } else if (strcmp(input, "DELETE") == 0) {
    deleteStudent(ht, sz, ns);
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

bool readnames(vector<char*>* names, char* fname) {
  ifstream fs (fname);
  if (!fs.is_open()) {
    cout << "Error: Could not read from file \"" << fname << "\". Aborting." << endl;
    return false;
  }
  for (int i = 0; i < 1000; i++) {
    char* name = new char();
    fs.getline(name, 20);
    names -> push_back(name);
  }
  return true;
}

void printHelp() {
  cout << "StudentList help" << endl;
  cout << "HELP: Print this help message" << endl;
  cout << "ADD: Add a student to the student list" << endl;
  cout << "DELETE: Delete a student from the student list" << endl;
  cout << "RANDOM: Add a number of random students to the student list" << endl;
  cout << "PRINT: Print the students in the list" << endl;
  cout << "QUIT: Quit the program" << endl;
}

void addStudent(Node**& ht, int &sz, int &ns) {
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
  int hash = (getHash(newS->firstName, sz) + getHash(newS->lastName, sz)*3 + newS->id)%sz;
  int ret = addChain(ht, hash, newS);
  if (ret == -1) {
    cout << "Duplicate student (first name, last name, id match) found, student not added to list" << endl;
    return;
  }
  ns++;
  if(ret > 3|| ns > sz/2) {
    cout << "its rehash time" << endl;
    rehash(ht, sz);
  }
  cout << "Student added" << endl;
}

//Returns length of chain
int addChain(Node**& ht, int h, Student* s) {
  if (ht[h] == NULL) {
    ht[h] = new Node();
    ht[h] -> student = s;
    return 0;
  }
  cout << "Collide, yo" << endl;
  int ln = 0;
  Node* link = ht[h];
  while (link -> next != NULL) {
    if (*(link -> student) == *s) { //Duplicate student
      return -1;
    }
    link = link -> next;
    ln++;
  }
  if (*(link -> student) == *s) { //Duplicate student
      return -1;
  }
  link -> next = new Node();
  link -> next -> student = s;
  return ln;
}

void rehash(Node**& ht, int &sz) {
  Node** tempnew = new Node*[sz*2];
  for (int i = 0; i < sz*2; i++) {
    tempnew[i] = NULL;
  }
  for (int i = 0; i < sz; i++) {
    if (ht[i] == NULL) continue;
    Node* n = ht[i];
    do {
      Student* s = n->student;
      int hash = (getHash(s->firstName, sz*2) + getHash(s->lastName, sz*2)*3 + s->id)%(sz*2);
      addChain(tempnew, hash, s);
      n=n->next;
    } while (n != NULL);
  }
  cout << ht << endl;
  delete ht; //Deallocate old hashtable
  ht = tempnew;
  cout << ht << endl;
  sz *= 2;
  
}

void deleteStudent(Node**& ht, int sz, int &ns) {
  Student* s = new Student();
  cout << "Enter student first name" << endl;
  cin.get(s->firstName, 25);
  cin.clear();
  cin.ignore(999, '\n');
  cout << "Enter student last name" << endl;
  cin.get(s->lastName, 25);
  cin.clear();
  cin.ignore(999, '\n');
  cout << "Enter student id" << endl;
  cin >> s->id;
  cin.clear();
  cin.ignore(999, '\n');
  int hash = (getHash(s->firstName, sz) + getHash(s->lastName, sz)*3 + s->id)%sz;
  for (int i = 0; i < sz; i++) {
    if (ht[i] == NULL) continue;
    Node* n = ht[i];
    Node* prev = NULL;
    do {
      if (*(n -> student) == *s) {
	if (prev == NULL) {
	  ht[i] = n->next;
	  delete n;
	}
	else {
	  prev->next = n->next;
	  delete n;
	}
	ns--;
	cout << "Student deleted." << endl;
	return;
      }
      prev = n;
      n = n->next;
    } while (n != NULL);
  }
  cout << "Student with corresponding name and id not found" << endl;
  return;
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
      cout << "Hash: " << i << endl; //Debug only
      n = n->next;
    } while (n != NULL);
  }
  if (!students) cout << "Student list is empty" << endl;
  cout << endl;
}

//This function is irrelevant.
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
  hash *= 769; 
  hash %= sz;
  return hash;
}
