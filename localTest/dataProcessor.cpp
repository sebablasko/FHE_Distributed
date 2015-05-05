#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include "Processor.h"

using namespace std;

static int verbose_flag;    //TODO: set the utility of the verbose flag

int main(int argc, char **argv)
{
  /* Variables for data input processing */
  int mnc = -1, lac = -1, cid = -1, antenna_index;
  char* fileCipherData;
   
  while(1)
  {
    static struct option long_options[] = 
    {
      /* These options set a flag. */
      {"verbose", no_argument,       &verbose_flag, 1},
      /* These options don’t set a flag. We distinguish them by their indices. */
      {"mnc",  required_argument, 0, 'm'},
      {"lac",  required_argument, 0, 'l'},
      {"cid",    required_argument, 0, 'c'},
      {"file",    required_argument, 0, 'f'},
      /* The close for Options */
      {0, 0, 0, 0}
    };
    
    /* getopt_long stores the option index here. */
    int opt = -1, option_index = 0;
    
    opt = getopt_long (argc, argv, "m:l:c:f:", long_options, &option_index);
    
    /* Detect the end of the options. */
    if (opt == -1)
      break;
    
    switch(opt)
    {
      
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0)
          break;
        printf ("option %s", long_options[option_index].name);
        if (optarg)
          printf (" with arg %s", optarg);
        printf ("\n");
        break;    
      
      case 'm':
        mnc = strtod(optarg, NULL);
        /* printf ("option --mnc with value %d\n", mnc); */
        break;
    
      case 'l':
        lac = strtod(optarg, NULL);
        /* printf ("option --lac with value %d\n", lac); */
        break;
    
      case 'c':
        cid = strtod(optarg, NULL);
        /* printf ("option --cid with value %d\n", cid); */
        break;
        
      case 'f':
        fileCipherData = optarg;
        /* printf ("option --file with value %s\n", fileCipherData); */
        break;
    
      default:
        abort ();
    }
    
  }
  /* Instead of reporting ‘--verbose.’
   and ‘--brief’ as they are encountered,
   we report the final status resulting from them. */
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
  if(mnc == -1 || lac == -1 || cid == -1)
  {
    printf("Error, must explicit mnc, lac and cid code\n");
    return 1;
  }
  else if(!fileCipherData)
  {
    printf("Error, must explicit file to process\n");
    return 1;
  }
  
  /* Everything Ok, let's start the processing */
  printf("Launching the process module with:\n");
  printf("\tmnc:\t%d\n",mnc);
  printf("\tlac:\t%d\n",lac);
  printf("\tcid:\t%d\n",cid);
  
  Processor processor(fileCipherData);
  processor.Process(mnc, lac, cid, antenna_index);
  std::cout << "antenna_index value: " << antenna_index << std::endl;
  
  return 0;
}