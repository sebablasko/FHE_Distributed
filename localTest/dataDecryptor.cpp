#include <string>
#include <iostream>
#include <getopt.h>
#include "ResultDecryptor.h"

static int verbose_flag;    //TODO: set the utility of the verbose flag

int main(int argc, char **argv)
{
    int antenna_index = -1;
    char* fileProcessedData;
    
    while(1)
    {
        static struct option long_options[] = 
        {
          /* These options set a flag. */
          {"verbose", no_argument,       &verbose_flag, 1},
          /* These options don’t set a flag. We distinguish them by their indices. */
          {"antennaindex",  optional_argument, 0, 'a'},
          {"file",    required_argument, 0, 'f'},
          /* The close for Options */
          {0, 0, 0, 0}
        };
        
        /* getopt_long stores the option index here. */
        int opt = -1, option_index = 0;
        
        opt = getopt_long (argc, argv, "a:f:", long_options, &option_index);
        
        /* Detect the end of the options. */
        if (opt == -1)
          break;
        
        switch(opt)
        {
          case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
              break;
            if (verbose_flag) printf ("option %s", long_options[option_index].name);
            if (optarg)
              if (verbose_flag) printf (" with arg %s", optarg);
            if (verbose_flag) printf ("\n");
            break;    
          
          case 'a':
            antenna_index = strtod(optarg, NULL);
            if (verbose_flag) printf ("option --antenna_index with value %d\n", antenna_index);
            break;
            
          case 'f':
            fileProcessedData = optarg;
            if (verbose_flag) printf ("option --file with value %s\n", fileProcessedData);
            break;
        
          default:
            abort ();
        }
    }
    
    /* Report Verbose Mode */
    if (verbose_flag)
        puts("verbose flag is set");
    
    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        putchar('\n');
    }
    
    /* Check for all parameters */  
    if(!fileProcessedData)
    {
    printf("Error, must explicit file to decrypt\n");
    return 1;
    }
    
    /* Everything Ok, let's start the processing */
    if (verbose_flag)
    {
        printf("Launching the decrypt module with:\n");
        printf("\tantenna_index:\t%d\n",antenna_index);
        printf("\tSource File:\t%s\n",fileProcessedData);
    }

    /* Build ResultDecryptor */
    /* Build ResultDecryptor Use the standart construction, restoring the parameters already builded */
    ResultDecryptor result_decryptor;
    /* Restore the cipher file with processed data  */
    Ctxt cipher = result_decryptor.BuildCipherFromFile(fileProcessedData);

    /* Return the result */
    if(antenna_index > -1)
    {
        int result = result_decryptor.DecryptResult(cipher, antenna_index);
        std::cout << "Resultado:\t" << result << std::endl;
    }else{
        result_decryptor.DecryptResult(cipher);
    }
       
}