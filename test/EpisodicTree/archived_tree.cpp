#include <iostream>
#include <chrono>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>
#include <unistd.h>

#include "mementar/core/Btree/Btree.h"
#include "mementar/core/EpisodicTree/ArchivedLeafNode.h"

using namespace std::chrono;

int main()
{
  high_resolution_clock::time_point t1 = high_resolution_clock::now();

  mementar::ArchivedLeafNode archived_node("/home/gsarthou/Desktop/tests", 4);

  std::cout << " *************" << std::endl;
  for(size_t i = 0; i < 400000; i++)
  {
    archived_node.insert(new mementar::LinkedFact<time_t>(i, "bob", "hasValue", std::to_string(i)));
    if(i == 250000)
      archived_node.newSession();
  }
  std::cout << " *************" << std::endl;

  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  std::cout << "took " << time_span.count() << std::endl;

  std::cout << "will remove" << std::endl;
  archived_node.remove(mementar::LinkedFact<time_t>(102, "bob", "hasValue", std::to_string(102)));
  std::cout << "removed" << std::endl;
  archived_node.insert(new mementar::LinkedFact<time_t>(0, "bob", "hasValue", std::to_string(0)));
  std::cout << "inserted" << std::endl;

  std::cout << " *************" << std::endl;
  /*//archived_node.display(150000);

  std::cout << "first key = " << archived_node.getFirst()->getKey() << std::endl;

  std::cout << "find key = " << archived_node.find(50)->getKey() << std::endl;
  std::cout << "findNear key = " << archived_node.findNear(102)->getKey() << std::endl;
*/
  //mementar::CompressedLeaf<size_t> leaf(&tree, "/home/gsarthou/Desktop");

  return 0;
}
