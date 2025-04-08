// finding all prime numbers up to some n, version 3
// author:  Holger Arndt
// version: 17.12.2019

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
  int n{1000};                        // default number
  string outfilename{"outputprime3.txt"};   // default output file name
  ofstream outfile;
  if (argc >= 2)
    n = atoi(argv[1]);
  if (argc >= 3)
    outfilename = argv[2];
  outfile.open(outfilename);
  if (n >= 2)
    outfile << 2 << endl;
  if (n >= 3)
    outfile << 3 << endl;
  if (n >= 5)
    outfile << 5 << endl;
  if (n >= 7)
    outfile << 7 << endl;
  if (n >= 11)
    outfile << 11 << endl;
  if (n >= 13)
    outfile << 13 << endl;
  if (n >= 17)
    outfile << 17 << endl;
  if (n >= 19)
    outfile << 19 << endl;

  int dmax, d, i;
  for (i = 23; i <= n; i += 2)
    {
      if (i % 3 == 0 || i % 5 == 0 || i % 7 == 0 || i % 11 == 0 ||
          i % 13 == 0 || i % 17 == 0 || i % 19 == 0)
        continue;
      dmax = static_cast<int>(sqrt(static_cast<double>(i)));
      for (d = 23; d <= dmax; d += 2)
        if (i % d == 0)
          break;
      if (d > dmax)
        outfile << i << endl;
    }

  outfile.close();
}

