#include "mementar/core/archiving_compressing/archiving/Header.h"

#include "mementar/core/archiving_compressing/binaryManagement/BitFileGenerator.h"
#include "mementar/core/archiving_compressing/binaryManagement/BitFileGetter.h"

#include <fmt/format.h>

#include <sstream>
#include <string>

namespace mementar
{

std::string Header::toString()
{
  fmt::memory_buffer out;

  fmt::format_to(out, FMT_STRING("{:<50} | {:<10} | {:<10}\n"), "file name", "size", "offset");
  fmt::format_to(out, FMT_STRING("{}\n"), std::string{50+3+10+3+10, '='});
  fmt::format_to(out, FMT_STRING("{:<50} | {:10} | {:10}\n"), description_file_.name_, description_file_.size_, description_file_.offset_);
  fmt::format_to(out, FMT_STRING("{:<50} | {:<10} | {:<10}\n"), "", "", "");
  for (const auto& file : input_files_)
  {
    fmt::format_to(out, FMT_STRING("{:<50} | {:10} | {:10}\n"), file.name_, file.size_, file.offset_);
  }

  return fmt::to_string(out);
}

size_t Header::endodedSize()
{
  size_t nb_bit = 0;
  nb_bit += 8 + description_file_.name_.size()*7 + 31 + 31;
  nb_bit += 16;
  for(const auto& file : input_files_)
  {
    nb_bit += 8 + file.name_.size()*7 + 31 + 31;
  }

  return nb_bit / 8 + 1;
}

void Header::encode(std::vector<char>& out)
{
  BitFileGenerator bit(31, 16, 8);
  bit.writeType3(description_file_.name_.size());
  for(auto c : description_file_.name_)
    bit.writeChar(c);
  bit.writeType1(description_file_.offset_);
  bit.writeType1(description_file_.size_);

  bit.writeType2(input_files_.size());
  for(const auto& file : input_files_)
  {
    bit.writeType3(file.name_.size());
    for(auto c : file.name_)
      bit.writeChar(c);
    bit.writeType1(file.offset_);
    bit.writeType1(file.size_);
  }

  std::vector<char> tmp;
  bit.get(tmp);
  out.insert(out.end(), tmp.begin(), tmp.end());
}

void Header::decode(std::vector<char>& data)
{
  BitFileGetter bit(31, 16, 8);
  bit.set(data);

  size_t file_size = bit.getType3();
  for(size_t i = 0; i < file_size; i++)
    description_file_.name_.push_back(bit.getChar());
  description_file_.offset_ = bit.getType1();
  description_file_.size_ = bit.getType1();

  size_t nb_file =  bit.getType2();
  for(size_t j = 0; j < nb_file; j++)
  {
    File_t file;
    file_size = bit.getType3();
    for(size_t i = 0; i < file_size; i++)
      file.name_.push_back(bit.getChar());
    file.offset_ = bit.getType1();
    file.size_ = bit.getType1();
    input_files_.push_back(file);
  }
}

} // namespace mementar
