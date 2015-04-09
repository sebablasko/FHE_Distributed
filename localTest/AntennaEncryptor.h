#include "../libs/HElib/EncryptedArray.h"
#include <fstream>
#include <map>
#include <sstream>

#define CTXT_FILE "result_cipher"
#define CONTEXT_FILE "context.context"
#define PUBLIC_KEY_FILE "public.key"
#define HASH_FILES_PATH "../data/"
#define DEBUG 1

class AntennaEncryptor
{
    FHEcontext context;
    FHEPubKey public_key;
    ZZX G;
    EncryptedArray encrypted_array;
    std::map< string, string > antenna_hash;

  FHEcontext InitializeContext()
  {
    fstream context_file;
    context_file.open(CONTEXT_FILE, fstream::in);
    unsigned long m1, p1, r1;
    readContextBase(context_file, m1, p1, r1);
    FHEcontext context_(m1, p1, r1);
    context_file >> context_;
    context_file.close();
    return context_;
  }

  FHEPubKey InitializePk()
  {
    fstream pk_file;
    pk_file.open (PUBLIC_KEY_FILE);
    FHEPubKey p_key(this->context);
    pk_file >> p_key;
    pk_file.close();
    return p_key;
  }

  EncryptedArray InitializeEncryptedArray()
  {
    this->G = this->context.alMod.getFactorsOverZZ()[0];
    EncryptedArray encrypted_array_(this->context, this->G);
    return encrypted_array_;
  }

  void InitializeHash(int mnc_)
  {
    if (DEBUG > 0) std::cout << "AntennaEncryptor: Initializing Hash..."<< std::endl;

    string hash_mnc[3] = {"","a_1_hash","a_2_hash"};

    string hashFilePath = HASH_FILES_PATH;
    hashFilePath.append(hash_mnc[mnc_]);

    if (DEBUG > 1) std::cout << "hash file path:" << hashFilePath << std::endl;

    std::ifstream hash_file(hashFilePath.c_str());
    string mnc, lac, cid, a,z,pz, key;
    while (hash_file >> mnc >> lac >> cid >> a >> z >> pz)
    {
      key = lac + " " + cid;
      this->antenna_hash[key] = a + " " + z + " " + pz;
      
      if (DEBUG > 1) std::cout << "AntennaEncryptor: Added to Hash: key:"<< key << ", val:" << this->antenna_hash[key] << std::endl;
    }
  }

  vector<long> BuildVector(int code, int encrypted_array_size)
  {
    //code starts in 1 / vector starts in 0. code<= encrypted_array_size
    vector<long> code_vector;
    for(int i = 0 ; i < code-1; i++)
    {
      code_vector.push_back(0);
    }
    code_vector.push_back(1);

    for(int i = code ; i < encrypted_array_size; i++)
    {
      code_vector.push_back(0);
    }

    return code_vector;
  }

  public:
    AntennaEncryptor(int mnc):
      context(InitializeContext()), public_key(InitializePk()), encrypted_array(InitializeEncryptedArray())
      {
        if (DEBUG > 0) std::cout << "AntennaEncryptor Initializing... "<< std::endl;
        InitializeHash(mnc);
        if (DEBUG > 0) std::cout << "AntennaEncryptor Correctly Initialized"<< std::endl;
      }
  
  FHEcontext get_context()
  {
    return context;
  }

  FHEPubKey get_public_key()
  {
    return public_key;
  }

  EncryptedArray get_encrypted_array()
  {
    return encrypted_array;
  }

  std::map< string, string > get_antenna_hash()
  {
    return antenna_hash;
  }

  int get_antenna_code(int lac, int cid)
  {

    int a_code, z_code, pz_code;
    stringstream lac_cid;
    lac_cid << lac << " " << cid;
    string in_code = lac_cid.str();
    string out_code = antenna_hash[in_code];

    if (DEBUG > 1) std::cout << "AntennaEncryptor: get_antenna_code: lac:" << lac << ", cid:" << cid << ", in_code:" << in_code << ", out_code:" << out_code << std::endl;

    stringstream ss_lac_cid(out_code);
    
    while(ss_lac_cid >> a_code >> z_code >> pz_code)
    {
      return a_code;
    }
    return 0;
      
  }

  /*  get the private zone code, used in ZRotation for the full process operation */
  int get_zone_code(int lac, int cid)
  {

    int a_code, z_code, pz_code;
    stringstream lac_cid;
    lac_cid << lac << " " << cid;
    string in_code = lac_cid.str();
    string out_code = antenna_hash[in_code];

    if (DEBUG > 1) std::cout << "AntennaEncryptor: get_zone_code: lac:" << lac << ", cid:" << cid << ", in_code:" << in_code << ", out_code:" << out_code << std::endl;

    stringstream ss_lac_cid(out_code);

    while(ss_lac_cid >> a_code >> z_code >> pz_code)
    {
      return z_code;
    }
    return 0;
  }

  /*  get the public zone code, used in the encryption process, saved as plain text, used to retrieve data from database as filter */
  int get_pzone_code(int lac, int cid)
  {
    int a_code, z_code, pz_code;
    stringstream lac_cid;
    lac_cid << lac << " " << cid;
    string in_code = lac_cid.str();
    string out_code = antenna_hash[in_code];
    stringstream ss_lac_cid(out_code);
    
    while(ss_lac_cid >> a_code >> z_code >> pz_code)
    {
      return pz_code;
    }
    return 0;
  }
  
  Ctxt EncryptAntenna(int lac, int cid)
  {
    
    int antenna_code = get_antenna_code(lac,cid);
    int zone_code = get_zone_code(lac,cid);

    if (DEBUG > 0) std::cout << "AntennaEncryptor: EncryptAntenna: lac:" << lac << ", cid:" << cid << ", antenna_code:" << antenna_code << ", zone_code:" << zone_code << std::endl;

    Ctxt antenna_cipher(this->public_key);
    PlaintextArray antenna_plaintext(this->encrypted_array);

    if(antenna_code == 0 && zone_code==0){
      //Nothing to encrypt
      throw 33;
    }

    std::vector<long> antenna_vector = BuildVector(antenna_code, this->encrypted_array.size());
    antenna_plaintext.encode(antenna_vector);
    this->encrypted_array.encrypt(antenna_cipher, this->public_key, antenna_plaintext);
    
    if (DEBUG > 1) std::cout << "AntennaEncryptor: EncryptAntenna: Done" << std::endl;

    return antenna_cipher;
  }

  Ctxt EncryptZone(int lac, int cid)
  {
    int antenna_code = get_antenna_code(lac,cid);
    int zone_code = get_zone_code(lac,cid);

    if (DEBUG > 0) std::cout << "AntennaEncryptor: EncryptZone: lac:" << lac << ", cid:" << cid << ", antenna_code:" << antenna_code << ", zone_code:" << zone_code << std::endl;

    Ctxt zone_cipher(this->public_key);
    PlaintextArray zone_plaintext(this->encrypted_array);

   
    if(antenna_code == 0 && zone_code==0){
      //Nothing to encrypt
      throw 33;
    }
    
    std::vector<long> zone_vector = BuildVector(zone_code, this->encrypted_array.size());
    
    zone_plaintext.encode(zone_vector);
    this->encrypted_array.encrypt(zone_cipher, this->public_key, zone_plaintext);
    
    if (DEBUG > 1) std::cout << "AntennaEncryptor: EncryptZone: Done" << std::endl;

    return zone_cipher;

  }
  Ctxt CtxtFromString(string cipher_string)
  {
    Ctxt cipher(public_key);
    stringstream ss_cipher;
    ss_cipher << cipher_string;
    ss_cipher >> cipher;
    return cipher; 
  }

  void CtxtToFile(Ctxt cipher)
  {
    ofstream cipher_file;
    cipher_file.open(CTXT_FILE);
    cipher_file << cipher ;
    cipher_file.close();
  }
  
};

