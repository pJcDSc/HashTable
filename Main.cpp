
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

struct Student { //student struct
  int id;
  float gpa;
  char firstName[35];
  char lastName[35];

  bool operator==(const Student &o) { //comparison function for delete, duplicity checking
    return o.id == this->id && strcmp(o.firstName, this->firstName) == 0 && strcmp(o.lastName, this->lastName) == 0;
  }
};

struct Node { //for chaining in hash table
  Node* next;
  Student* student;
};

using namespace std;

bool parse(char*, Node**&, int&, int&, vector<char*>*, vector<char*>*);
void printHelp();
void addStudent(Node**&, int&, int&);
void deleteStudent(Node**&, int, int&);
void generateRandom(Node**&, vector<char*>*, vector<char*>*, int&, int&);
void printStudents(Node**, int);
int addChain(Node**&, int, Student*);
void rehash(Node**&, int&);
int getHash(char*, int);
bool readnames(vector<char*>*, char*, int);

int main() {
  Node** ht = NULL; //This is the hashtable
  int size = 100;   //initial size
  int numStudents = 0; //number of students
  ht = new Node*[size]; 
  for (int i = 0; i < size; i++) {
    ht[i] = NULL; //make sure to initialize all values in the array to null
  }
  
  vector<char*>* fnames = new vector<char*>(); //storage of names for random student generator
  vector<char*>* lnames = new vector<char*>();
  char* fileName = new char(); strcpy(fileName, "fnms.txt");
  if (!readnames(fnames, fileName, 2000)) return 1;
  strcpy(fileName, "lnms.txt");
  if (!readnames(lnames, fileName, 1000)) return 1;

  cout << "Welcome to student list with hashtable" << endl;
  cout << "Type help for more commands" << endl;
  bool run = true;
  char* input = new char();
  while (run) {
    cin.get(input, 30);
    cin.clear();
    cin.ignore(999, '\n');

    run = parse(input, ht, size, numStudents, fnames, lnames);
  }
  delete[] ht;
  return 0;
}

//Parse user input and call functions
bool parse(char* input, Node** &ht, int &sz, int &ns, vector<char*>* fnames, vector<char*>* lnames) {
  for (int i = 0; i < strlen(input); i++) {
    input[i] = toupper(input[i]); //capitalize user input
  }
  if (strcmp(input, "HELP") == 0) {
    printHelp();
  } else if (strcmp(input, "ADD") == 0) {
    addStudent(ht, sz, ns);
  } else if (strcmp(input, "DELETE") == 0) {
    deleteStudent(ht, sz, ns);
  } else if (strcmp(input, "RANDOM") == 0) {
    generateRandom(ht, fnames, lnames, ns, sz);
  } else if (strcmp(input, "QUIT") == 0) {
    return false;
  } else if (strcmp(input, "PRINT") == 0) {
    printStudents(ht, sz);
  } else {
    cout << "Sorry, input \"" << input << "\" not recognized" << endl;
  }
  return true;
}

//Reads lines from file fname into vector names with size n
bool readnames(vector<char*>* names, char* fname, int n) {
  ifstream fs (fname); //filestream
  if (!fs.is_open()) { //make sure files are readable
    cout << "Error: Could not read from file \"" << fname << "\". Aborting." << endl;
    return false;       //if files aren't readable return false (readnames did not complete, exit the program)
  }
  for (int i = 0; i < n; i++) { //Read each line and add to vector
    char* name = new char();
    fs.getline(name, 20);
    if (n == 1000) { //lastnames file is a little weird, the newline characters are different (?) and i don't
                     //know how to fix them so this is my solution to make sure that you can delete randomly generated students
      name[strlen(name)-1] = '\0';
    }
    names -> push_back(name);
  }
  return true;
}

//help print
void printHelp() {
  cout << "StudentList help" << endl;
  cout << "HELP: Print this help message" << endl;
  cout << "ADD: Add a student to the student list" << endl;
  cout << "DELETE: Delete a student from the student list" << endl;
  cout << "RANDOM: Add a number of random students to the student list" << endl;
  cout << "PRINT: Print the students in the list" << endl;
  cout << "QUIT: Quit the program" << endl;
}

//Reads user input for new student
void addStudent(Node**& ht, int &sz, int &ns) {
  Student* newS = new Student(); //next student to add
  cout << "Enter student first name" << endl; //read student data
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
  int hash = (getHash(newS->firstName, sz) + getHash(newS->lastName, sz)*3 + newS->id)%sz; //compute hash
  int ret = addChain(ht, hash, newS);  //add to hashtable based on hash
  if (ret == -1) { //if duplicate student was found, (mainly to prevent repetitively rehashing when two students always map to same place)
    cout << "Duplicate student (first name, last name, id match) found, student not added to list" << endl;
    return;
  }
  ns++; //Increment number of students
  if(ret > 3|| ns > sz/2) { //rehash if chain length on add was greater than 3, or if the number of students in the table is > than half the slots total
    rehash(ht, sz);
  }
  cout << "Student added" << endl;
}

//Returns length of chain, or -1 if a duplicate is found
int addChain(Node**& ht, int h, Student* s) {
  if (ht[h] == NULL) { //If there is no node here, this is the first student that goes into this slot, no chaining is needed
    ht[h] = new Node();
    ht[h] -> student = s;
    return 0;
  }
  int ln = 0; //chaining is needed, store length of chain
  Node* link = ht[h];
  while (link -> next != NULL) {    //When there is no next node, we have reached the last node of the chain
    if (*(link -> student) == *s) { //Duplicate student
      return -1;
    }
    link = link -> next;
    ln++;
  }
  if (*(link -> student) == *s) { //Duplicate student
      return -1;
  }
  link -> next = new Node(); //Add node
  link -> next -> student = s;
  return ln;
}

//Rehashes all values from table by looping over all slots and recomputing hash for new table size 
void rehash(Node**& ht, int &sz) {
  Node** tempnew = new Node*[sz*2]; //new hashtable
  for (int i = 0; i < sz*2; i++) { //initialize all new values to null
    tempnew[i] = NULL;
  }
  for (int i = 0; i < sz; i++) { //rehash each student by going throught the chain and recomputing hash for new size
    if (ht[i] == NULL) continue;
    Node* n = ht[i];
    do {
      Student* s = n->student;
      int hash = (getHash(s->firstName, sz*2) + getHash(s->lastName, sz*2)*3 + s->id)%(sz*2);
      addChain(tempnew, hash, s);
      n=n->next;
    } while (n != NULL);
  }
  delete[] ht; //Deallocate old hashtable 
  ht = tempnew;//Set reference of old hashtable to new hashtable
  sz *= 2;     //make sure to update size
}

//Delete a student with matching first last name and id
void deleteStudent(Node**& ht, int sz, int &ns) {
  Student* s = new Student();
  cout << "Enter student first name" << endl; //get user input
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
  int hash = (getHash(s->firstName, sz) + getHash(s->lastName, sz)*3 + s->id)%sz; //compute hash of student to delete
  
  Node* n = ht[hash];
  Node* prev = NULL;
  do { //Loop through corresponding chain
    if (*(n -> student) == *s) { 
      if (prev == NULL) { //Casework for deletion
	ht[hash] = n->next;
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
  //reach end of chain, that means no student in the chain matched
  cout << "Student with corresponding name and id not found" << endl;
  return;
}

//Asks for user input and generates random students
void generateRandom(Node**& ht, vector<char*>* fnames, vector<char*>* lnames, int& ns, int& sz) {
  cout << "Please enter number of students to generate (max 1000)" << endl;
  int num; //Get number of students to generate
  cin >> num;
  cin.ignore(999, '\n');
  if (num <= 0) { //Obviously <= 0 doesn't mean anything
    cout << "Please enter a number between 1 and 1000" << endl;
    return;
  }
  if (num > 1000) { //don't let them enter something stupid like 99999999
    cout << "Number entered is greater than 1000, so only 1000 students will be generated." << endl;
    num = 1000;
  }
  for (int i = 0; i < num; i++) { //Loop n times, where n is user inputted number
    Student* s = new Student();
    int random = rand() % 2000; //Generate random first name from list of 2000 first names
    strcpy(s -> firstName, fnames -> at(random));
    random = rand() % 1000;     //generate random last name from list of 1000 last names
    strcpy(s -> lastName, lnames -> at(random)); 
    s -> id = ns++;             //increment and assign id
    s -> gpa = float(rand()%350+100)/100; //generate random gpa between 1.0 and 4.5 by generating numbers between 100 and 450 then dividing by 100
    int hash = (getHash(s->firstName, sz) + getHash(s->lastName, sz)*3 + s->id)%sz;
    int chainlength = addChain(ht, hash, s); //add this student to the hashtable
    if (chainlength > 3 || ns > sz/2) rehash(ht, sz); //make sure to rehash along the way
  }
  cout << "Students generated." << endl;
}

//Prints all students in the list
void printStudents(Node** ht, int sz) {
  bool students = false;
  for (int i = 0; i < sz; i++) { //loop through the list
    if (ht[i] == NULL) continue; //nothing in this slot, continue
    Node* n = ht[i];
    students = true;
    do { //loop through chain and print student data
      cout << endl;
      cout << "Student: " << n->student->firstName << " " << n->student->lastName << endl;
      cout << "ID: " << n->student->id << endl;
      cout << "GPA: " << n->student->gpa << endl;
      //cout << "Hash: " << i << endl; //Debug only
      n = n->next;
    } while (n != NULL);
  }
  if (!students) cout << "Student list is empty" << endl;
  cout << endl;
}

//computes hash from char*
int getHash(char* s, int sz) { //sz is size of hashtable
  int hash = 3;
  for (int i = 0; i < strlen(s); i++) {
    hash *= 69697; //Big prime number oooo
    hash += int(s[i]);
    hash %= sz;
  }
  hash *= 69697; //the best prime number 
  hash %= sz; //make sure hash is within size of table
  return hash;
}
