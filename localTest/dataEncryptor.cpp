#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include "AntennaEncryptor.h"

using namespace std;

#define PLAINDATA_FILEPATH "../data/data.txt"
#define CIPHERDATA_FILEPATH "../data/data.cipher"

int main() {
    
    //Open Plain Data File
    ifstream infile(PLAINDATA_FILEPATH);
    
    //Open file for output cipher data
       // open a file in write mode.
     ofstream outfile;
     outfile.open(CIPHERDATA_FILEPATH);


    int m = 1;
    //recupero todos los records con mnc==1, como en este caso todos son asi, los recupero todos

    string line;
    AntennaEncryptor a_encryptor(m);

    while (std::getline(infile, line))
    {
      istringstream iss(line);

      // Parameters for a record
      string _id, _cls, user_id, device_id;
      int event_type;
      string timestamp;
      int network_type, telephony_standard, mcc, mnc, signal_strength__size;
      string signal_strength__mean, signal_strength__variance;
      int gsm_cid, gsm_lac, gsm_psc;
      string neighbor_list;

      if(!(iss >> _id >> _cls >> user_id >> device_id >> event_type >> timestamp >> network_type >> telephony_standard >> mcc >> mnc >> signal_strength__size >> signal_strength__mean >> signal_strength__variance >> gsm_cid >> gsm_lac >> gsm_psc >> neighbor_list)) { break; }

      Ctxt a_cipher = a_encryptor.EncryptAntenna(gsm_lac, gsm_cid);
      Ctxt z_cipher = a_encryptor.EncryptZone(gsm_lac, gsm_cid);

      stringstream acs, zcs;
      acs << a_cipher;
      zcs << z_cipher;
      int zone = a_encryptor.get_pzone_code(gsm_lac, gsm_cid);

      //outfile << "device_id " << _id << " timestamp " << timestamp << " network_type " << network_type << " mnc " << mnc <<  " antenna " << acs.str() << " privzone " << zcs.str() << " zone " << zone << endl;

      outfile << _id << "," << timestamp << "," << network_type << "," << mnc << "," << acs.str() << "," << zcs.str() << "," << zone << ";" << endl;

      //outfile << acs.str() << ";";

    }

    outfile.close();

    return EXIT_SUCCESS;
}

