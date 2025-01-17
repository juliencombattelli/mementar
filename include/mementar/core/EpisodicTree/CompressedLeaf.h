#ifndef MEMENTAR_COMPRESSEDLEAF_H
#define MEMENTAR_COMPRESSEDLEAF_H

#include <string>
#include <ctime>

#include "mementar/core/LinkedBtree/LinkedBtree.h"
#include "mementar/core/Data/LinkedFact.h"

namespace mementar
{

class CompressedLeaf
{
public:
  CompressedLeaf(Btree<time_t, LinkedFact<time_t>>* tree, const std::string& directory);
  CompressedLeaf(const time_t& key, const std::string& directory);

  std::string getDirectory() { return directory_; }
  time_t getKey() { return key_; }

  LinkedBtree<time_t>* getTree();
private:
  time_t key_;
  std::string directory_;

  std::string treeToString(Btree<time_t, LinkedFact<time_t>>* tree);
};

} // namespace mementar

#endif // MEMENTAR_COMPRESSEDLEAF_H
