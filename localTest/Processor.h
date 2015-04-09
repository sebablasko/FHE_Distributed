#include "AntennaEncryptor.h"
#define VERBOSE 1
#define MAX_ADDITIONS 8170 

class Processor{  

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

  void run()
  {
    if (VERBOSE) std::cout << "Processor.h: Started" << std::endl;
    
  }

  //void CountAntenna(mongo::auto_ptr<mongo::DBClientCursor> cursor, int rotation, AntennaEncryptor &antenna_encryptor, EncryptedArray &encrypted_array)
  void CountAntenna(int rotation, AntennaEncryptor &antenna_encryptor, EncryptedArray &encrypted_array)
  {
    if (VERBOSE) std::cout << "Processor.h: CountAntenna ..." << std::endl;
  
  }

public:
  Processor()
  {
    if (VERBOSE) std::cout << "Processor.h: Processor" << std::endl;
  }

  void Process(int mnc, int lac, int cid, int &antenna_index){

    if (VERBOSE) std::cout << "Processor.h: Process: mnc:" << mnc << ", lac:" << lac << ", cid:" << cid << std::endl;

    int zone_index, rotation;
    
    //Initialize objects
    AntennaEncryptor antenna_encryptor(mnc);
    EncryptedArray encrypted_array = antenna_encryptor.get_encrypted_array();

    //Get antenna encoding, zone encoding  and rotation
    // (from antenna identifiers and hash)
    antenna_index = antenna_encryptor.get_antenna_code(lac, cid);
    zone_index = antenna_encryptor.get_zone_code(lac, cid);
    rotation = ZRotation(antenna_index, zone_index, encrypted_array.size());

    if(VERBOSE) std::cout << "Processor.h: Process: Using mnc:" << mnc << ", zone:" << zone_index << std::endl;

    //Obtain records from database
    //mongo::auto_ptr<mongo::DBClientCursor> cursor = c.query("adkevents.encrypted_fhe_events",  BSON("mnc" << mnc << "zone" << zone_index), 100);
    //mongo::auto_ptr<mongo::DBClientCursor> cursor = c.query("adkevents.encrypted_fhe_events",  BSON("mnc" << mnc << "zone" << zone_index));

    //POR DEFAULT; SE CONSIDERARÁ QUE EL ARCHIVO CONTIENE VALORES VALIDOS DEL RESULTADO DE UNA QUERY: mnc y zone válidos

    //Call the count function.
    //CountAntenna(cursor, rotation, antenna_encryptor, encrypted_array);
  }
};