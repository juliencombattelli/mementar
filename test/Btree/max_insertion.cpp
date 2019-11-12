#include <iostream>
#include <chrono>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "mementar/core/Btree/BtreeLeaf.h"
#include "mementar/core/Btree/Btree.h"

#include "mementar/core/Data/StampedData.h"

using namespace std::chrono;

class stampedInt : public mementar::StampedData<size_t>
{
public:
  stampedInt(size_t stamp, int data) : StampedData(stamp)
  {
    data_ = data;
  }
  size_t data_;

  friend std::ostream& operator<<(std::ostream& os, const stampedInt& data)
  {
    os << data.data_;
    return os;
  }

  bool operator==(const stampedInt& other) const
  {
    return (data_ == other.data_);
  }
};

int main()
{
  size_t nb = 349526;

  high_resolution_clock::time_point t1 = high_resolution_clock::now();

  mementar::Btree<size_t, stampedInt> tree(5);
  for(size_t i = 0; true; i++)
    if(tree.insert(new stampedInt(i, i)) >= nb)
      break;

  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  std::cout << "took " << time_span.count() << std::endl;

  //tree.display();
  std::cout << "level = " << tree.estimateMaxLevel(nb) << ">=" << tree.getCurrentLevel() << std::endl;

  return 0;
}
