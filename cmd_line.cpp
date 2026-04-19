#include<iostream>
#include<cstdlib>   // for atoi
#include<fstream>
#include "cache.h"

using namespace std;

  
int main(int argc, char*argv[]) {

  /* check if the correct number of arguments have been passed; bail otherwise */
  if (argc < 4) {
    cout << "Usage: " << endl;
    cout << "   ./cache num_entries associativity filename" << endl;
    return 0;
  }

  /* get args; all args come in as strings; need to convert to int with atoi() */
  unsigned entries = atoi(argv[1]);
  unsigned assoc = atoi(argv[2]);

  string input_filename = argv[3];

  Cache cache(num_entries, assoc);

  ifstream input(input_filename);
  ofstream output("cache_sim_output");

  if (!input.is_open()) {
    cerr << "Error: failed to open input file.\n";
    return 0;
  }

  unsigned long addr;

  while (input >> addr) {
    if (!cache.hit(output,addr))
      cache.update(output, addr);
  }

input.close();
output.close();
Cache ~cache();
  
  return 0;
}


