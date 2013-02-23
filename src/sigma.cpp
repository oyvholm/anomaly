////////////////////////////////////////////////////////////////////////////////
// anomaly - Anomalous data detection
//
// Copyright 2013, Göteborg Bit Factory.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <string>
#include <sys/select.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char** argv)
{
  // Process arguments from command line.
  std::vector <double> data;
  for (int i = 1; i < argc; i++)
    data.push_back (strtod (argv[i], NULL));

  // If there was nothing on the command line, read from stdin.
  if (data.size () == 0)
  {
    // Use 'select' to determine whether there is any std::cin content buffered
    // before trying to read it, to prevent blocking.
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    fd_set fds;
    FD_ZERO (&fds);
    FD_SET (STDIN_FILENO, &fds);

    int result = select (STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    if (result && result != -1)
    {
      if (FD_ISSET (0, &fds))
      {
        std::string arg;
        while (std::cin >> arg)
          data.push_back (strtod (arg.c_str (), NULL));
      }
    }
  }

  double sigma = 0.0;
  double mean = 0.0;

  int n = data.size ();
  if (n > 1)
  {
    // Calculate mean, standard deviation.
    double sum = 0.0;
    double sum_squares = 0.0;
    std::vector <double>::iterator i;
    for (i = data.begin (); i != data.end (); ++i)
    {
      sum += *i;
      sum_squares += (*i) * (*i);
    }

    sigma = sqrt (((n * sum_squares) - (sum * sum)) / (n * (n - 1)));
    mean = sum / n;
  }
  else if (n > 0)
  {
    mean = data[0];
  }

  std::cout << mean  << "\n"
            << sigma << "\n";

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

