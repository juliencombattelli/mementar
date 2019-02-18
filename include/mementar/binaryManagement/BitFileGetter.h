#ifndef BITFILEGETTER_H
#define BITFILEGETTER_H

#include <vector>
#include <string>

class BitFileGetter
{
public:
  BitFileGetter(uint32_t size_1 = 0, uint32_t size_2 = 0, uint32_t size_3 = 0, uint32_t size_4 = 0);

  void setSize1(uint32_t size_1) { type_1_size_ = size_1; }
  void setSize2(uint32_t size_2) { type_2_size_ = size_2; }
  void setSize3(uint32_t size_3) { type_3_size_ = size_3; }
  void setSize4(uint32_t size_4) { type_4_size_ = size_4; }

  uint32_t getType1();
  uint32_t getType2();
  uint32_t getType3();
  uint32_t getType4();
  char getChar();

  bool getBit();

  bool end(size_t offset = 0);

  void set(std::vector<char>& data) { data_ = data; }

private:
  std::vector<char> data_;
  size_t major_index_;
  uint8_t minor_index_;

  uint32_t type_1_size_;
  uint32_t type_2_size_;
  uint32_t type_3_size_;
  uint32_t type_4_size_;
};

#endif // BITFILEGETTER_H