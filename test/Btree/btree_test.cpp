#include <iostream>
#include <chrono>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "mementar/core/Btree/BtreeLeaf.h"
#include "mementar/core/Btree/Btree.h"

#include "mementar/core/Data/StampedData.h"

using namespace std::chrono;

class stampedInt : public mementar::StampedData<int>
{
public:
  stampedInt(int stamp, int data) : StampedData(stamp)
  {
    data_ = data;
  }
  int data_;

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
  mementar::BtreeLeaf<int, stampedInt> leaf1(new stampedInt(0, 2));
  mementar::BtreeLeaf<int, stampedInt> leaf2(new stampedInt(1, 3));

  if(leaf1.operator<(&leaf2))
    std::cout << "inf" << std::endl;
  else
    std::cout << "sup" << std::endl;

  std::vector<size_t> sizes = {1234};
  std::vector<double> times;
  for(auto nb : sizes)
  {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    mementar::Btree<int, stampedInt> tree(3);
    for(size_t i = 0; i < nb; i++)
      tree.insert(new stampedInt(i, i));

    //tree.display();

    auto res = tree.find(10);
    if(res) std::cout << res->getKey() << std::endl;
    res = tree.find(28);
    if(res) std::cout << res->getKey() << std::endl;
    res = tree.find(74);
    if(res) std::cout << res->getKey() << std::endl;
    res = tree.find(53);
    if(res) std::cout << res->getKey() << std::endl;
    res = tree.findNear(49);
    if(res) std::cout << res->getKey() << std::endl;
    res = tree.getFirst();
    if(res) std::cout << res->getKey() << std::endl;

    tree.remove(stampedInt(40,40));
    tree.insert(new stampedInt(40,41));
    tree.display();

    std::cout << "estimation = " << tree.estimateMinLeaves() << std::endl;
    std::cout << "level = " << tree.estimateMaxLevel(nb) << ">=" << tree.getCurrentLevel() << std::endl;

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "took " << time_span.count() << std::endl;
    times.push_back(time_span.count());

    for(size_t i = 0; i < times.size(); i++)
      std::cout << sizes[i] << ";";
    std::cout << std::endl;
    for(size_t i = 0; i < times.size(); i++)
      std::cout << times[i] << ";";
    std::cout << std::endl;
  }

  return 0;
}
