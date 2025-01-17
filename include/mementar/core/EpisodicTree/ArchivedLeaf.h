#ifndef MEMENTAR_ARCHIVEDLEAF_H
#define MEMENTAR_ARCHIVEDLEAF_H

#include <ctime>
#include <string>

#include "mementar/core/EpisodicTree/CompressedLeafNode.h"

namespace mementar
{

class ArchivedLeaf
{
public:
  ArchivedLeaf(CompressedLeafNode* tree, size_t nb, const std::string& directory);
  ArchivedLeaf(const time_t& key, const std::string& directory);

  std::string getDirectory() { return directory_; }
  time_t getKey() { return key_; }

  LinkedBtree<time_t>* getTree(size_t i);
  std::vector<Context> getContexts();

private:
  time_t key_;
  std::string directory_;
};

} // namespace mementar

#endif // MEMENTAR_ARCHIVEDLEAF_H
