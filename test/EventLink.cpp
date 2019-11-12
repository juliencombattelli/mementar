#include "mementar/core/LinkedBtree/LinkedBtree.h"


void displayLink(mementar::LinkedBtree<time_t>* t)
{
  auto first = t->getFirst();
  while(first != nullptr)
  {
    auto data = first->getData()[0];
    std::cout << data->getStamp() << " : " << *data << std::endl;
    if(data->next_)
      std::cout << "\tN = " << data->next_->getStamp() << " : " << *(data->next_) << std::endl;
    else
      std::cout << "\tN = nullptr" << std::endl;

    if(data->prev_)
      std::cout << "\tP = " << data->prev_->getStamp() << " : " << *(data->prev_) << std::endl;
    else
      std::cout << "\tP = nullptr" << std::endl;

    first = first->next_;
  }
}

int main()
{
  mementar::LinkedBtree<time_t> tree;

  for(int i = 20; i >= 0; i--)
  {
    if(i % 2)
      tree.insert(new mementar::LinkedFact<time_t>(i, "max", "hasCounted", std::to_string(i-1)));
    else
      tree.insert(new mementar::LinkedFact<time_t>(i, "max", "hasCounted", std::to_string(i)));
  }

  tree.remove(mementar::LinkedFact<time_t>(4, "max", "hasCounted", std::to_string(4)));
  tree.remove(mementar::LinkedFact<time_t>(4, "max", "hasCounted", std::to_string(4)));
  tree.remove(mementar::LinkedFact<time_t>(6, "max", "hasCounted", std::to_string(6)));
  displayLink(&tree);
  //tree.display();
  auto leaf = tree.find(1);

  std::cout << "+++++++++++++" << std::endl;
  if(leaf)
  {
    std::vector<mementar::LinkedFact<time_t>*> datas = leaf->getData();
    if(datas.size())
    {
      mementar::LinkedFact<time_t>* c = datas[0];
      while(c != nullptr)
      {
        std::cout << *c << std::endl;
        c = c->next_;
      }
    }
    else
      std::cout << "no data in the leaf" << std::endl;
  }
  else
    std::cout << "no leaf" << std::endl;

  return 0;
}
