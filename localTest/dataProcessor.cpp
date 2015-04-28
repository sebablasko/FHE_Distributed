#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include "Processor.h"

using namespace std;

int main(int argc, char **argv)
{
  int mnc, lac, cid, antenna_index;
  Processor processor;

  //Values fixed
  mnc = 1;
  lac = 13527;
  cid = 85696527;
  //cid = 85696524;
  //cid = 33186528;

  if(argc == 4)
  {
    mnc = atoi(argv[1]);
    lac = atoi(argv[2]);
    cid = atoi(argv[3]);
  }
  else if (argc != 1)
  {
    std::cout << "Error, use ./dataProcessor mnc lac cid" << std::endl;
    return 1;
  }

  std::cout << "Starting processing data: " << std::endl;
  std::cout << "\tmnc: \t" << mnc << std::endl;
  std::cout << "\tlac: \t" << lac << std::endl;
  std::cout << "\tcid: \t" << cid << std::endl;

  processor.Process(mnc, lac, cid, antenna_index);
  std::cout << "antenna_index value: " << antenna_index << std::endl;

  return 0;
}