#include <iostream>
#include <fstream>

#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "Usage: memnomic <amount_of_RAM_to_gobble_up>\n\n\
  memnomic allocates the requested amount of memory and forces the OS\n\
  to keep all storage it uses pinned in RAM via mlockall(2).\n"

using namespace std;

size_t allocation_amount(char* amount);

int main(int argc, char *argv[]) {
   if (argc < 2) {
      cout << USAGE;
      exit(1);
   }

   size_t to_be_allocated = allocation_amount(argv[1]);

   cout << "Allocating and locking " << to_be_allocated << " bytes.\n";

   char* mem = (char*) calloc(to_be_allocated, 1);

   mlockall(MCL_CURRENT);

   while(1) {
      sleep(1);
   }
}


size_t allocation_amount(char* amount) {
   int len = strlen(amount);
   int multiplier = 1;
   switch(amount[len-1]) {
   case 'G':
      multiplier = 1024*1024*1024;
      break;
   case 'M':
      multiplier = 1024*1024;
      break;
   case 'K':
      multiplier = 1024;
      break;
   default:
      multiplier = 1;
   }

   if (multiplier > 1) {
      amount[len-1] = 0;
   }

   /* only works for sizeof(size_t) == sizeof(unsigned long) */
   if (sizeof(size_t) != sizeof(unsigned long)) {
      cerr << "size_t is not the same as unsigned long.  Please update the application and recompile.\n";
      exit(1);
   }

   char* endptr;
   size_t retval = strtoul(amount, &endptr, 0);

   return retval*multiplier;
}
