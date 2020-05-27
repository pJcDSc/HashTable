/*
Author: Peter Jin
Date: 5/26/20
Hash table student list
*/

#include <iostream>
#include <cstring>

using namespace std;

int main() {
  cout << "Welcome to student list with hashtable" << endl;
  cout << "Type help for more commands" << endl;
  bool run = true;
  char* input = new char();
  while (run) {
    cin.get(input, 30);
    cin.clear();
    cin.ignore(999, '\n');

    run = parse(input);
  }
  return 0;
}
