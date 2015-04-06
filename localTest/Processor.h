#include "mongo/client/dbclient.h"
#include "AntennaEncryptor.h"
#define VERBOSE 1

/*
Processor
Object that handles the connection to the encrypted database and computation of 
the count function on encrypted data.
*/

#define MAX_ADDITIONS 8170 

class Processor{
  mongo::DBClientConnection c;
  

  int ZRotation(int antenna_index, int zone_index, int encrypted_array_size){
    /* computes the rotation of the Zone ciphertext necessary to perform 
    the element by element multiplication with the right slot of the antenna
    ciphertext.
    Args:
      antenna_index: slot corresponding to the antenna that we want to count
      zone_index: slot corresponding to the zone of the antenna that we want to count
      encrypted_array_size: number of slots of the arrays. 
    */
    int distance = antenna_index - zone_index;
    if(distance <= encrypted_array_size/2 && distance >= - encrypted_array_size/2)
      return distance;
    else if( distance > 0)
      return distance - encrypted_array_size;
    else
      return distance + encrypted_array_size;
  }

  void run(){
    c.connect("localhost");
    if (VERBOSE){
      std::cout << "connected ok" << std::endl;
    }
    
  }

  void CountAntenna(mongo::auto_ptr<mongo::DBClientCursor> cursor, int rotation, AntennaEncryptor &antenna_encryptor, EncryptedArray &encrypted_array){
  /*
  Computes the Count function and writes the encrypted result on a file. The count 
  function works on encrypted data. For each record, rotates the zone cipher and 
  multiplies it by the antenna cipher. Then performs the addition of all the results.

  Args:
    cursor: Database cursor (pointer to first record)
    rotation: rotation that needs to be applied to the zone before multiplication
    antenna_encryptor: object that handles the encryption of the antenna identifiers.
    encrypted_array: object that handles the encrypted array.

  */
    if (VERBOSE){
      std::cout << "Counting antennas ..." << std::endl;
    }
  
    int i=1;

    if(cursor->more() ){
      //Get the first record pointed by the cursor
      mongo::BSONObj record = cursor->next();

      //build the ciphers from the record, corresponding to the antenna and the zone
      string antenna_cipher_string = record["antenna"].str();
      string zone_cipher_string = record["privzone"].str();
      Ctxt antenna_cipher_1 = antenna_encryptor.CtxtFromString(antenna_cipher_string);
      Ctxt zone_cipher_1 = antenna_encryptor.CtxtFromString(zone_cipher_string);

      
      //rotate zone cipher: 
      encrypted_array.rotate(zone_cipher_1, rotation);
      //multiply ciphers: 
      antenna_cipher_1.multiplyBy(zone_cipher_1);
      
      while(cursor->more() && i<MAX_ADDITIONS) {
        //Get the next record
        i++;
        record = cursor->next();

        //build the cipher from the record, corresponding to the antenna and the zone
        antenna_cipher_string = record["antenna"].str();
        zone_cipher_string = record["privzone"].str();
        Ctxt antenna_cipher = antenna_encryptor.CtxtFromString(antenna_cipher_string);
        Ctxt zone_cipher = antenna_encryptor.CtxtFromString(zone_cipher_string);
        
        //rotate zone cipher: 
        encrypted_array.rotate(zone_cipher, rotation);
        //multiply ciphers: 
        antenna_cipher.multiplyBy(zone_cipher);
        //add to previous: 
        antenna_cipher_1 += antenna_cipher;
   
      }
      if (VERBOSE){
        std::cout << "writing encrypted result" << std::endl;
      }
      antenna_encryptor.CtxtToFile(antenna_cipher_1);
    }
  }

public:
  Processor(){
    mongo::client::initialize();
    try {
        run();
    } catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
    }
  }

  void Process(int mnc, int lac, int cid, int &antenna_index){
    /*
    Given an antenna identifier (mnc, lac, cid) computes the cipher of
    the number of records that correspond to the antenna.
    returns the antenna index. (0 if it doesn't work)
    */
    int zone_index, rotation;

    if (VERBOSE){
      std::cout << "initializing process" << std::endl;
    }
    
    //Initialize objects
    AntennaEncryptor antenna_encryptor(mnc);
    EncryptedArray encrypted_array = antenna_encryptor.get_encrypted_array();

    //Get antenna encoding, zone encoding  and rotation
    // (from antenna identifiers and hash)
    antenna_index = antenna_encryptor.get_antenna_code(lac, cid);
    zone_index = antenna_encryptor.get_zone_code(lac, cid);
    rotation = ZRotation(antenna_index, zone_index, encrypted_array.size());

    if (VERBOSE){
      std::cout << "querying database" << std::endl;
    }
    //Obtain records from database
    //TODO: we are restricting to 100 the number of records.
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c.query("adkevents.encrypted_fhe_events",  BSON("mnc" << mnc << "zone" << zone_index), 100);
    //mongo::auto_ptr<mongo::DBClientCursor> cursor = c.query("adkevents.encrypted_fhe_events",  BSON("mnc" << mnc << "zone" << zone_index));


    //Call the count function.
    CountAntenna(cursor, rotation, antenna_encryptor, encrypted_array);
  }
};