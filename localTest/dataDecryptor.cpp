#include <string>
#include <iostream>
#include "ResultDecryptor.h"

int main()
{
	//int antenna_index, result;
	//antenna_index = 17;
    ResultDecryptor result_decryptor;

    Ctxt cipher = result_decryptor.BuildCipherFromFile();
    //result = result_decryptor.DecryptResult(cipher, antenna_index);
    //std::cout << "Resultado:" << result << std::endl;
    result_decryptor.DecryptResult(cipher);
}