#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
  ifstream file("../data/cipherdata.txt");

  string str;

  getline(file,str,';');

  std::cout << "total: " << str.length() << std::endl;

}