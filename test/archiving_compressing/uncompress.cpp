#include <iostream>
#include <fstream>
#include <streambuf>

#include <chrono>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "mementar/core/archiving_compressing/compressing/LzUncompress.h"
#include "mementar/core/archiving_compressing/compressing/Huffman.h"

using namespace std::chrono;

enum code_t
{
  lz77,
  huffman,
  other_code
};

int main (int argc, char* argv[])
{
  std::string input_file = "";
  std::string output_file = "";
  code_t code_type = other_code;

  for(int i = 1; i < argc; i++)
  {
    if(std::string(argv[i]) == "-i")
    {
      if(i+1 < argc)
        input_file = std::string(argv[++i]);
      else
      {
        std::cout << "expected argument after the -i option" << std::endl;
        return -1;
      }
    }
    else if(std::string(argv[i]) == "-o")
    {
      if(i+1 < argc)
        output_file = std::string(argv[++i]);
      else
      {
        std::cout << "expected argument after the -o option" << std::endl;
        return -1;
      }
    }
    else if(std::string(argv[i]) == "-lz")
      code_type = lz77;
    else if(std::string(argv[i]) == "-hu")
      code_type = huffman;
    else if(std::string(argv[i]) == "-help")
    {
      std::cout << "Available options are :" << std::endl;
      std::cout << "\t-i : input file" << std::endl;
      std::cout << "\t-o : output file" << std::endl;
      std::cout << "\t-lz : use the lz77 coding" << std::endl;
      std::cout << "\t-hu : use Huffman's coding" << std::endl;
      return 0;
    }
  }

  if(input_file == "")
  {
    std::cout << "specify the input file with -i" << std::endl;
    return -1;
  }
  if(output_file == "")
  {
    std::cout << "specify the output file with -o" << std::endl;
    return -1;
  }
  if(code_type == other_code)
  {
    std::cout << "specify the encoding used with -lz or -hu" << std::endl;
    return -1;
  }

  std::string out;

  ///////////////////////////////////////////////////////////////////
  high_resolution_clock::time_point t1 = high_resolution_clock::now();

  if(code_type == lz77)
  {
    mementar::LzUncompress lz;
    std::vector<char> data;
    if(lz.readBinaryFile(data, input_file))
      lz.uncompress(data, out);
  }
  else if(code_type == huffman)
  {
    mementar::Huffman huff;
    std::vector<char> data;
    if(huff.readBinaryFile(data, input_file))
    {
      size_t tree_size = huff.setTree(data);
      data = std::vector<char>(data.begin() + tree_size, data.end());
      huff.getFile(data, out);
    }
  }

  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  ///////////////////////////////////////////////////////////////////

  std::ofstream myfile;
  myfile.open (output_file);
  myfile << out;
  myfile.close();

  //std::cout << out << std::endl;

  std::cout << "time = " << time_span.count() << "s" << std::endl;

  return 0;
}

//’
