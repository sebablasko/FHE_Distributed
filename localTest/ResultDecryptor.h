#include "../libs/HElib/EncryptedArray.h"
#include <fstream>


/*
ResultDecryptor
Object that handles the decryption of the result of a query.

Initializes sk, pk and context from file.
Decrypts the encrypted result of a query, building a ciphertext from
a file that contains it.
*/

/* #define PROCESSED_RESULT_CTXT_FILE "../data/processed.cipher" */
#define CONTEXT_FILE "context.context"
#define SECRET_KEY_FILE "secret.key"
#define PUBLIC_KEY_FILE "public.key"
#define VERBOSE 1
#define DEBUG 1

class ResultDecryptor{
  
   /* Variables nedded for decrypt */
  FHEcontext context;
  FHESecKey secret_key;
  FHEPubKey public_key;
  ZZX G;
  EncryptedArray encrypted_array;

  /* Listed standard functions to restore object nedded to Initialize a ResultDecryptor Object */
  /* 1.- Get The context */
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

  /* 2.- Get The SecretKey */
  FHESecKey InitializeSk(){
    fstream sk_file;
    sk_file.open (SECRET_KEY_FILE);
    FHESecKey s_key(this->context);
    sk_file >> s_key;
    sk_file.close();
  
    return s_key;
  }
  
  /* 3.- Get The PublicKey */  
  FHEPubKey InitializePk(){
    fstream pk_file;
    pk_file.open (PUBLIC_KEY_FILE);
    FHEPubKey p_key(this->context);
    pk_file >> p_key;
    pk_file.close();
  
    return p_key;
  }
  
  /* 4.- Get The EncryptedArray Object */  
  EncryptedArray InitializeEncryptedArray(){
    this->G = this->context.alMod.getFactorsOverZZ()[0];
    EncryptedArray encrypted_array_(this->context, this->G);
    
    return encrypted_array_;
  }
  
  
  /* Public methods to use in the ResultDecryptor Object */
  public:
    
    /* The Constructor 1 */
    /* Without Parameters, get the objects from standard methods */
    ResultDecryptor():
      context( InitializeContext() ), 
      secret_key( InitializeSk() ), 
      public_key( InitializePk() ), 
      encrypted_array( InitializeEncryptedArray() )
      {  
        
      }
      
    /* The Constructor 2 */
    /* With a Ciphet Text as Parameter, now use context and pk from Ctxt */
    ResultDecryptor(Ctxt cipher):
      context( cipher.getContext() ),
      secret_key( InitializeSk() ),
      public_key( cipher.getPubKey() ), 
      encrypted_array( InitializeEncryptedArray() )
      {
  
      }
  
    /* DecryptResult Method 1 */
    /* Decrypt a cipher text and return the value stored in the antenna_index position */
    int DecryptResult(Ctxt cipher, int antenna_index){
      if(antenna_index < 1 || antenna_index > encrypted_array.size()){
        return -1;
      }else{
        PlaintextArray decrypted_plaintext(encrypted_array);
        encrypted_array.decrypt(cipher, secret_key, decrypted_plaintext);
        /* decrypted_plaintext.print(cerr); */
        /* cerr<< endl; */
        vector<long> vec;
        decrypted_plaintext.decode(vec);
        return vec[antenna_index-1];
      }
    }
  
    /* DecryptResult Method 2 */
    /* Decrypt a cipher text and print all the plain text array in cerr */
    void DecryptResult(Ctxt cipher)
    {
        PlaintextArray decrypted_plaintext(encrypted_array);
        encrypted_array.decrypt(cipher, secret_key, decrypted_plaintext);
        decrypted_plaintext.print(cerr);
        cerr<< endl;
    }

    /* BuildCipherFromFile Method */
    /* Build a cipher text from a specific file, given in file parameter */
    Ctxt BuildCipherFromFile(char* file)
    {
      if (VERBOSE) std::cout << "building cipher from file: " << file << std::endl;
      fstream cipher_file;
      cipher_file.open(file, fstream::in);
      Ctxt cipher(public_key);
      /* This first stream corresponds to the antenna encryption. The second stream corresponds to the zone, but we don't need it. */
      cipher_file >> cipher;
      cipher_file.close();
      return cipher;
    }
};