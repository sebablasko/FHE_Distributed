#include "EncryptedArray.h"
#include <fstream>
#include <sstream>

//TODO: remove magic numbers and magic strings
/* Generates the public and secret key and stores them on separate files. */
/* Use default params: P: 8179, L: 2, K: 112 */

#define HEADKM_KEY_GENERATOR_CPP_R 1
#define HEADKM_KEY_GENERATOR_CPP_C 2
#define HEADKM_KEY_GENERATOR_CPP_W 64
#define HEADKM_KEY_GENERATOR_CPP_D 1
#define HEADKM_KEY_GENERATOR_CPP_S 0

#define AUTO_DEFAULT_MODE 1
#define AUTO_P 8179
#define AUTO_L 2
#define AUTO_K 112

#define PATHFILE_SECRETKEY "secret.key"
#define PATHFILE_PUBLICKEY "public.key"
#define PATHFILE_CONTEXT "context.context"

void generate_keys(long p, long L, long k){
  /*
  Generates the public and secret key and stores them on separate files.
  Args: 
    p: Prime number. Defines the modulus of the operations.
    L: Number of ciphertext supported (levels).
    k: Security parameter.
  */

  // output files
  ofstream sk_file, pk_file, context_file;
  
  //Context parameters
  long r = HEADKM_KEY_GENERATOR_CPP_R; // Typically, r = 1 for ordinary homomorphic computation

  // modChain parameters
  long c = HEADKM_KEY_GENERATOR_CPP_C;//c is the number of columns in our key-switching matrices. recomended c=2 or c=3

  // keygen parameters
  long w = HEADKM_KEY_GENERATOR_CPP_W;//the Hamming weight of a secret key (w = 64 recommended)

  // encryptedArray construction parameters
  ZZX G; //univariate polynomials over ZZ 

  // m construction parameters
  long d = HEADKM_KEY_GENERATOR_CPP_D;//The arguments p, d, determine the plaintext space F_(p^d)
  long s = HEADKM_KEY_GENERATOR_CPP_S;// the argument s bounds from below the number of plaintext slots that we want to support

  long m = FindM(k, L, c, p, d, s, 0); //Todo: remove magic number

  cout << "Initializing context... ";
  FHEcontext context(m, p, r);
  cout << " Done " << endl;

  cout << "Constructing modChain... ";
  buildModChain(context, L, c);
  cout << context ;
  cout << " Done " << endl;


  cout << "Working on keyGen... ";
  FHESecKey secret_key(context);
  const FHEPubKey& public_key = secret_key;
  secret_key.GenSecKey(w); // generate a secret key with Hamming weight w
  addSome1DMatrices(secret_key); // compute key-switching matrices that we need
  cout << " Done " << endl;

  
  cout << "Storing secret key... ";
  sk_file.open (PATHFILE_SECRETKEY);
  sk_file << secret_key;
  sk_file.close(); 
  cout << "Done " << endl;

  cout << "Storing public key... ";
  // Storing in binary mode does not reduce file size if writing the same way. check for special way to write blocks.
  // TODO: check http://www.cplusplus.com/doc/tutorial/files/ -> binary files
  //pk_file.open ("key.pub", ios::out | ios::binary);
  pk_file.open (PATHFILE_PUBLICKEY);
  pk_file << public_key;
  pk_file.close();
  cout << "Done " << endl;

  cout << "Storing context... ";
  context_file.open (PATHFILE_CONTEXT);
  writeContextBase(context_file, context);
  context_file << context << endl;
  context_file.close(); 
  cout << "Done " << endl;

  //cout << "pk: "<< public_key.getPubEncrKey() << endl;
  //cout<< public_key.getPubEncrKey().size()<<endl;

 }

int main(int argc, char **argv){

  long p, L, k;

  if(AUTO_DEFAULT_MODE==1)
  {
    p = AUTO_P;
    L = AUTO_L;
    k = AUTO_K;
  }else{
    cout << "Define the parameters for key generation:" << endl;
    cout << "p: Prime number. Define the modulus of the operations." << endl;
    cout << "L: Number of ciphertext supported (levels)." << endl;
    cout << "k: Security parameter." << endl;
    cout << "P? ";
    cin >> p;
    cout << "L? ";
    cin >> L;
    cout << "K? ";
    cin >> k;
    cout << endl;
  }
  generate_keys(p, L, k);

}