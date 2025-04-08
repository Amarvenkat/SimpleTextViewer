// finding all prime numbers up to some n, version 1
// author:  Holger Arndt
// version: 17.12.2019

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

bool isprime(int i)
{
  if (i % 2 == 0)
    return false;
  for (int d{3}; d <= sqrt(static_cast<double>(i)); d += 2)
    if (i % d == 0)
      return false;
  return true;
}

int main(int argc, char** argv)
{
  int n{1000};                        // default number
  string outfilename{"outputprime1.txt"};   // default output file name
  ofstream outfile;
  if (argc >= 2)
    n = atoi(argv[1]);
  if (argc >= 3)
    outfilename = argv[2];
  outfile.open(outfilename);
  if (n >= 2)
    outfile << 2 << endl;

  for (int i{3}; i <= n; i += 2)
    if (isprime(i))
      outfile << i << endl;

  outfile.close();
}

