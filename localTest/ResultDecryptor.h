#include "../libs/HElib/EncryptedArray.h"
#include <fstream>


/*
ResultDecryptor
Object that handles the decryption of the result of a query.

Initializes sk, pk and context from file.
Decrypts the encrypted result of a query, building a ciphertext from
a file that contains it.
*/

#define PROCESSED_RESULT_CTXT_FILE "../data/processed.cipher"
#define CONTEXT_FILE "context.context"
#define SECRET_KEY_FILE "secret.key"
#define PUBLIC_KEY_FILE "public.key"
#define VERBOSE 1
#define DEBUG 1

class ResultDecryptor{
  FHEcontext context;
  FHESecKey secret_key;
  FHEPubKey public_key;
  ZZX G;
  EncryptedArray encrypted_array;

FHEcontext InitializeContext(){
  fstream context_file;
  context_file.open(CONTEXT_FILE, fstream::in);
  unsigned long m1, p1, r1;
  readContextBase(context_file, m1, p1, r1);
  FHEcontext context_(m1, p1, r1);
  context_file >> context_;
  context_file.close();
  
  return context_;
}

FHESecKey InitializeSk(){
  fstream sk_file;
  sk_file.open (SECRET_KEY_FILE);
  FHESecKey s_key(this->context);
  sk_file >> s_key;
  sk_file.close();

  return s_key;
}
FHEPubKey InitializePk(){
  fstream pk_file;
  pk_file.open (PUBLIC_KEY_FILE);
  FHEPubKey p_key(this->context);
  pk_file >> p_key;
  pk_file.close();

  return p_key;
}
EncryptedArray InitializeEncryptedArray(){
  this->G = this->context.alMod.getFactorsOverZZ()[0];
  EncryptedArray encrypted_array_(this->context, this->G);
  return encrypted_array_;
}
public:
  ResultDecryptor():
    context(InitializeContext()), secret_key(InitializeSk()), public_key(InitializePk()), encrypted_array(InitializeEncryptedArray()){
      
    }
  

  ResultDecryptor(Ctxt cipher):
    context(cipher.getContext()),secret_key(InitializeSk()),public_key(cipher.getPubKey()), encrypted_array(InitializeEncryptedArray()){}
  
  int DecryptResult(Ctxt cipher, int antenna_index){
    /*
      Returns the decrypted value of the result in index antenna_index
      Args: 
        cipher: the ciphertext of the encrypted array of the result
        antenna_index: goes from 1 to encrypted_array.size()
    */
    if(antenna_index < 1 || antenna_index > encrypted_array.size()){
      return 0;
    }else{
      PlaintextArray decrypted_plaintext(encrypted_array);
      encrypted_array.decrypt(cipher, secret_key, decrypted_plaintext);
      decrypted_plaintext.print(cerr);
      cerr<< endl;
      vector<long> vec;
      decrypted_plaintext.decode(vec);
      return vec[antenna_index-1];
    }

  }

  void DecryptResult(Ctxt cipher)
  {
      PlaintextArray decrypted_plaintext(encrypted_array);
      encrypted_array.decrypt(cipher, secret_key, decrypted_plaintext);
      decrypted_plaintext.print(cerr);
      cerr<< endl;
  }

  Ctxt BuildCipherFromFile(){
    /*
    Returns a Ciphertext (that can later be decrypted) built
    from a predefined file.
    Args: 
      no args
    */

    if (VERBOSE){
      std::cout << "building cipher from file" << std::endl;
    }
    fstream cipher_file;
    cipher_file.open(PROCESSED_RESULT_CTXT_FILE, fstream::in);
    Ctxt cipher(public_key);
    //This first stream corresponds to the antenna encryption.
    cipher_file >> cipher;
    if (DEBUG > 0) std::cout << "cipher_antenna_encryption: "<< cipher << std::endl;
    // The second stream corresponds to the zone, but we don't need it.
    cipher_file.close();
    return cipher; 

  }

};


//TESTS

void ResultDecryptorTest(){
  ResultDecryptor result_decryptor;
  //test context:
  if(DEBUG == 1){
    Ctxt cipher = result_decryptor.BuildCipherFromFile();
    cout << result_decryptor.DecryptResult(cipher, 149) << endl;
    cout << result_decryptor.DecryptResult(cipher, 148) << endl;

  }
    
  
}