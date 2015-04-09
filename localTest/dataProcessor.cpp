#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include "Processor.h"

using namespace std;

#define CIPHERDATA_FILEPATH "../data/cipherdata.txt"
#define ValuesPerRecord 7

int main()
{
  /*
  // Open the source data file
  ifstream file(CIPHERDATA_FILEPATH);
  string line;

  // ForEach record in the cipherdata file
  while (std::getline(file, line, ';'))
  {
    string record[ValuesPerRecord];
    stringstream values(line);
    int i;

    for(i = 0; i < ValuesPerRecord; i++)
    {
      getline(values, record[i], ',');
    }

    // Process Each Record
    std::cout << record[0] << std::endl;
  }

  */

  int mnc, lac, cid, antenna_index;
  Processor processor;

  //Values fixed
  mnc = 1;
  lac = 13527;
  cid = 85696527;

  processor.Process(mnc, lac, cid, antenna_index);

}