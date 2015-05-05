#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include "AntennaEncryptor.h"

#define VERBOSE 0
#define MAX_ADDITIONS 8170 
#define ValuesPerRecord 7

using namespace std;

class Processor{

  /* Function for calculate the number of rotations needed */
  int ZRotation(int antenna_index, int zone_index, int encrypted_array_size)
  {
    int distance = antenna_index - zone_index;

    if(distance <= encrypted_array_size/2 && distance >= - encrypted_array_size/2)
      return distance;
    else if( distance > 0)
      return distance - encrypted_array_size;
    else
      return distance + encrypted_array_size;
  }

  /* CountAntenna */
  /* take a record, get the zone and antenna from it, and operate it to restore the correct value of antenna encrypted */
  Ctxt CountAntenna(int rotation, AntennaEncryptor &antenna_encryptor, EncryptedArray &encrypted_array, string *record)
  {
    if (VERBOSE) std::cout << "Processor.h: CountAntenna timestamp:" << record[1] << std::endl;
    Ctxt antenna_cipher = antenna_encryptor.CtxtFromString(record[4]);
    Ctxt zone_cipher = antenna_encryptor.CtxtFromString(record[5]);
    /* Rotate zone cipher */ 
    encrypted_array.rotate(zone_cipher, rotation);
    /* Multiply ciphers */ 
    antenna_cipher.multiplyBy(zone_cipher);
    /* Return the corrected value */
    return antenna_cipher;
  }

  public:

    char* CIPHERDATA_FILEPATH;

    /* Constructor */
    Processor(char* baseFileCipherData)
    {
      CIPHERDATA_FILEPATH = baseFileCipherData;
      /* TODO: Check for the params have to be created at this stage */
      if(VERBOSE) std::cout << "Processor.h: Constructor to work with: " << CIPHERDATA_FILEPATH << std::endl;
    }

  void Process(int mnc, int lac, int cid, int &antenna_index)
  {

    if (VERBOSE) std::cout << "Processor.h: Process: mnc:" << mnc << ", lac:" << lac << ", cid:" << cid << std::endl;

    int zone_index, rotation, zone_p_index;
    
    //Initialize objects
    AntennaEncryptor antenna_encryptor(mnc);
    EncryptedArray encrypted_array = antenna_encryptor.get_encrypted_array();

    //Get antenna encoding, zone encoding  and rotation (from antenna identifiers and hash)
    antenna_index = antenna_encryptor.get_antenna_code(lac, cid);
    zone_index = antenna_encryptor.get_zone_code(lac, cid);
    rotation = ZRotation(antenna_index, zone_index, encrypted_array.size());

    zone_p_index = antenna_encryptor.get_pzone_code(lac, cid);

    if(VERBOSE) std::cout << "Processor.h: Process: Using mnc:" << mnc << ", publicZone:" << zone_p_index << std::endl;

    // Collect data from the cipherDataFile and process it

    // 1.-Open the source data file
    ifstream file(CIPHERDATA_FILEPATH);
    string line;

    int checks = 0;

    // 2.-ForEach record in the cipherdata file...
    while (std::getline(file, line, ';'))
    {
      // 3.-locate the first matched record
      string record[ValuesPerRecord];
      stringstream values(line);
      int i;

      if(VERBOSE) std::cout << "Checking line " << ++checks << std::endl;

      for(i = 0; i < ValuesPerRecord; i++)
        getline(values, record[i], ',');
        
      /* Now the record store:
          [0]: id
          [1]: timestamp
          [2]: network_type
          [3]: mnc
          [4]: antenna (encrypted)
          [5]: privzone (encrypted)
          [6]: zone (publicZone in plain text)
      */ 

      // 4.a-Process Each Record...
      if(atoi(record[3].c_str())==mnc && atoi(record[6].c_str())==zone_p_index)
      {
        
        if(VERBOSE) std::cout << "\tFirst record Matched!" << std::endl;
        Ctxt total = CountAntenna(rotation, antenna_encryptor, encrypted_array, record);

        // 4.b-...and find the anothers records to match
        while(std::getline(file, line, ';'))
        {
          string record2[ValuesPerRecord];
          stringstream values2(line);

          if(VERBOSE) std::cout << "Checking line " << ++checks << std::endl;

          for(i = 0; i < ValuesPerRecord; i++)
            getline(values2, record2[i], ',');

          if(atoi(record2[3].c_str())==mnc && atoi(record2[6].c_str())==zone_p_index)
          {
            if(VERBOSE) std::cout << "\t\tA new match!" << std::endl;
            
            // 4.c-...operating all the selected records, storing the result in total variable
            total += CountAntenna(rotation, antenna_encryptor, encrypted_array, record2);
          }else{
            if(VERBOSE) std::cout << "\tNo match" << std::endl;
          }
          
        }
          // 5.- Save the processed data
          antenna_encryptor.CtxtToFile(total);
          break;
      }else{
        if(VERBOSE) std::cout << "\tNo match" << std::endl;
        continue;
      }
    }
  }
};