#include <sstream>
#include <experimental/filesystem>

#include "mementar/EpisodicTree/CompressedLeafNode.h"

namespace mementar
{

CompressedLeafNode::CompressedLeafNode(std::string directory, size_t order)
{
  directory_ = directory;
  order_ = order;
  last_tree_nb_leafs_ = 0;

  loadStoredData();

  running_ = true;
  session_cleaner_ = std::move(std::thread(&CompressedLeafNode::clean, this));
}

CompressedLeafNode::~CompressedLeafNode()
{
  running_ = false;
  session_cleaner_.join();

  for(auto tree : btree_childs_)
  {
    compressed_childs_.push_back(CompressedLeaf(tree, directory_));
    delete tree;
  }

  for(size_t i = 0; i < compressed_sessions_tree_.size(); i++)
  {
    if(compressed_sessions_tree_[i] != nullptr)
    {
      compressed_childs_[i] = std::move(CompressedLeaf(compressed_sessions_tree_[i], directory_));
      delete compressed_sessions_tree_[i];
    }
  }
}

void CompressedLeafNode::insert(const time_t& key, const Fact& data)
{
  if(keys_.size() == 0)
  {
    createNewTreeChild(key);
    last_tree_nb_leafs_ = btree_childs_[0]->insert(key, data);
  }
  else
  {
    if(key < keys_[0])
    {
      std::cout << "[ERROR] try to insert fact in past that do not exist" << std::endl;
      return;
    }

    size_t index = getKeyIndex(key);

    if(index < compressed_childs_.size())
    {
      if((index == compressed_childs_.size() - 1) && (keys_.size() == compressed_childs_.size()))
      {
        createNewTreeChild(key);
        last_tree_nb_leafs_ = btree_childs_[0]->insert(key, data);
      }
      else
      {
        createSession(index);
        compressed_sessions_tree_[index]->insert(key, data);
      }
    }
    else if(useNewTree())
    {
      createNewTreeChild(key);
      last_tree_nb_leafs_ = btree_childs_[btree_childs_.size() - 1]->insert(key, data);

      //verify if a chld need to be compressed
      if(btree_childs_.size() > 2)
        compressFirst();
    }
    else if(index - keys_.size() + 1 == 0) // if insert in more recent tree
      last_tree_nb_leafs_ = btree_childs_[index - compressed_childs_.size()]->insert(key,data);
    else
      btree_childs_[index - compressed_childs_.size()]->insert(key,data);
  }
}

void CompressedLeafNode::remove(const time_t& key, const Fact& data)
{
  int index = getKeyIndex(key);

  if(index >= 0)
  {
    if((size_t)index < compressed_childs_.size())
    {
      createSession(index);
      compressed_sessions_tree_[index]->remove(key, data);
    }
    else
      btree_childs_[index - compressed_childs_.size()]->remove(key, data);
  }
}

BtreeLeaf<time_t, Fact>* CompressedLeafNode::find(const time_t& key)
{
  int index = getKeyIndex(key);

  if(index >= 0)
  {
    if((size_t)index < compressed_childs_.size())
    {
      createSession(index);
      return compressed_sessions_tree_[index]->find(key);
    }
    else
      return btree_childs_[index - compressed_childs_.size()]->find(key);
  }
  else
    return nullptr;
}

BtreeLeaf<time_t, Fact>* CompressedLeafNode::findNear(const time_t& key)
{
  int index = getKeyIndex(key);

  if(index >= 0)
  {
    if((size_t)index < compressed_childs_.size())
    {
      createSession(index);
      return compressed_sessions_tree_[index]->findNear(key);
    }
    else
      return btree_childs_[index - compressed_childs_.size()]->findNear(key);
  }
  else
    return nullptr;
}

BtreeLeaf<time_t, Fact>* CompressedLeafNode::getFirst()
{
  if(compressed_childs_.size())
  {
    createSession(0);
    return compressed_sessions_tree_[0]->getFirst();
  }
  else if(btree_childs_.size())
    return btree_childs_[0]->getFirst();
  else
    return nullptr;
}

void CompressedLeafNode::display(time_t key)
{
  int index = getKeyIndex(key);

  if(index >= 0)
  {
    if((size_t)index < compressed_childs_.size())
      std::cout << compressed_childs_[index].getDirectoty() << std::endl;
    else
      return btree_childs_[index - compressed_childs_.size()]->display();
  }
}

void CompressedLeafNode::createNewTreeChild(const time_t& key)
{
  btree_childs_.push_back(new Btree<time_t,Fact>(order_));
  keys_.push_back(key);
}

bool CompressedLeafNode::useNewTree()
{
  if(last_tree_nb_leafs_ >= 100000)
    return true;
  else
    return false;
}

int CompressedLeafNode::getKeyIndex(const time_t& key)
{
  int index = keys_.size() - 1;
  for(size_t i = 0; i < keys_.size(); i++)
  {
    if(key < keys_[i])
    {
      index = i - 1;
      break;
    }
  }
  return index;
}

void CompressedLeafNode::loadStoredData()
{
  for(const auto& entry : std::experimental::filesystem::directory_iterator(directory_))
  {
    std::string complete_dir = entry.path();
    std::string dir = complete_dir.substr(directory_.size());
    if(dir[0] == '/')
      dir.erase(dir.begin());
    size_t dot_pose = dir.find(".");
    if(dot_pose != std::string::npos)
    {
      std::string ext = dir.substr(dot_pose + 1);
      std::string str_key = dir.substr(0, dot_pose);
      if(ext == "mlz")
      {
        time_t key;
        std::istringstream iss(str_key);
        iss >> key;
        insert(key, CompressedLeaf(key, complete_dir));
        std::cout << complete_dir << " have been loaded" << std::endl;
      }
    }
  }
}

void CompressedLeafNode::insert(const time_t& key, const CompressedLeaf& leaf)
{
  if((keys_.size() == 0) || (key > keys_[keys_.size() - 1]))
  {
    keys_.push_back(key);
    compressed_childs_.push_back(leaf);
    compressed_sessions_tree_.push_back(nullptr);
    compressed_sessions_timeout_.push_back(0);
  }
  else
  {
    for(size_t i = 0; i < keys_.size(); i++)
    {
      if(key < keys_[i])
      {
        keys_.insert(keys_.begin() + i, key);
        compressed_childs_.insert(compressed_childs_.begin() + i, leaf);
        break;
      }
    }
  }
}

void CompressedLeafNode::compressFirst()
{
  if(btree_childs_.size() == 0)
    return;

  CompressedLeaf tmp(btree_childs_[0], directory_);

  // put mutex here
  compressed_childs_.push_back(tmp);
  compressed_sessions_tree_.push_back(nullptr);
  compressed_sessions_timeout_.push_back(0);

  btree_childs_.erase(btree_childs_.begin());
  //release shared mutex here
}

void CompressedLeafNode::createSession(size_t index)
{
  if(compressed_sessions_tree_[index] == nullptr)
  {
    compressed_sessions_tree_[index] = compressed_childs_[index].getTree();
    compressed_sessions_timeout_[index] = std::time(0);
  }
}

void CompressedLeafNode::clean()
{
  size_t rate = 1000000 / 1000; // clean rate / look up rate;
  size_t cpt = rate;
  while(running_ == true)
  {
    if(cpt-- == 0)
    {
      cpt = rate;
      time_t now = std::time(0);
      for(size_t i = 0; i < compressed_sessions_timeout_.size(); i++)
      {
        if((compressed_sessions_tree_[i] != nullptr) &&
          (std::difftime(now, compressed_sessions_timeout_[i]) > 30)) // session expire after 30s
        {
          delete compressed_sessions_tree_[i];
          compressed_sessions_tree_[i] = nullptr;
        }

      }
    }
  }
}

} // mementar
