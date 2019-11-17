#include <vector>
#include <regex>
#include <sstream>

#include "mementar/cpp_compat/filesystem.hpp"

#include "mementar/core/EpisodicTree/ArchivedLeaf.h"

#include "mementar/core/archiving_compressing/archiving/Archive.h"
#include "mementar/core/archiving_compressing/archiving/Header.h"
#include "mementar/core/archiving_compressing/compressing/LzUncompress.h"

namespace mementar
{

ArchivedLeaf::ArchivedLeaf(CompressedLeafNode* tree, size_t nb, const std::string& directory)
{
  if(tree == nullptr)
    return;

  key_ = tree->getFirst()->getKey();
  directory_ = directory + '/' + std::to_string(key_);

  std::vector<time_t> keys;
  std::vector<Context> contexts;
  std::vector<std::string> input_files;

  for(size_t i = 0; i < nb; i++)
  {
    if(i >= tree->compressed_childs_.size())
      break;

    keys.push_back(tree->keys_[i]);
    contexts.push_back(tree->contexts_[i]);
    input_files.push_back(tree->compressed_childs_[i].getDirectory() + ".mlz");
  }

  std::string context = Context::ContextsToString(contexts);

  Archive arch(context, input_files);

  std::vector<char> data;
  arch.load(data);

  arch.saveToFile(data, directory_);

  for(size_t i = 0; i < nb; i++)
  {
    if(i >= tree->compressed_childs_.size())
      break;

    filesystem::remove(tree->compressed_childs_[i].getDirectory() + ".mlz");
  }
}

ArchivedLeaf::ArchivedLeaf(const time_t& key, const std::string& directory)
{
  key_ = key;
  size_t dot_pose = directory.find(".");
  directory_ = directory.substr(0, dot_pose);
}

LinkedBtree<time_t>* ArchivedLeaf::getTree(size_t i)
{
  mementar::Archive arch;
  std::cout << "ArchivedLeaf::getTree READ BINARY FILE " << directory_ << ".mar" << std::endl;
  arch.readBinaryFile(directory_ + ".mar");
  mementar::Header header = arch.getHeader();

  std::string comp = arch.extractFile(i, header);

  std::string out;
  LzUncompress lz;
  std::vector<char> comp_data(comp.begin(), comp.end());
  lz.uncompress(comp_data, out);
  LinkedBtree<time_t>* tree = new LinkedBtree<time_t>();

  std::regex regex("\\[(\\d+)\\](\\w+)\\|(\\w+)\\|(\\w+)");
  std::smatch match;

  std::istringstream iss(out);
  std::string line;
  while(std::getline(iss, line))
  {
    if(std::regex_match(line, match, regex))
    {
      time_t key;
      std::istringstream iss(match[1].str());
      iss >> key;
      LinkedFact<time_t>* fact = new LinkedFact<time_t>(key, match[2].str(), match[3].str(), match[4].str());

      tree->insert(fact);
    }
  }

  return tree;
}

std::vector<Context> ArchivedLeaf::getContexts()
{
  mementar::Archive arch;
  std::cout << "ArchivedLeaf::getContexts READ BINARY FILE " << directory_ << ".mar" << std::endl;
  arch.readBinaryFile(directory_ + ".mar");
  mementar::Header header = arch.getHeader();

  std::string out = arch.extractDescription(header);

  return Context::StringToContext(out);
}

} // namespace mementar
