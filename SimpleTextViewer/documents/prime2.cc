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
  int dmax{static_cast<int>(sqrt(static_cast<double>(i)))};
  for (int d{3}; d <= dmax; d += 2)
    if (i % d == 0)
      return false;
  return true;
}

int main(int argc, char** argv)
{
  int n{1000};                        // default number
  string outfilename{"outputprime2.txt"};   // default output file name
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

