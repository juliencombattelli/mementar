#ifndef MEMENTAR_BTREELEAFNODE_H
#define MEMENTAR_BTREELEAFNODE_H

#include "mementar/Btree/BtreeNode.h"
#include "mementar/Btree/BtreeLeaf.h"

namespace mementar
{

template<typename Tkey, typename Tdata>
class BtreeLeafNode : public BtreeNode<Tkey,Tdata>
{
public:
  BtreeLeafNode(size_t order = 10) : BtreeNode<Tkey,Tdata>(order)
  {}

  ~BtreeLeafNode() {}

  virtual bool isLeafNode() { return true; }

  BtreeLeaf<Tkey,Tdata>* insert(const Tkey& key, const Tdata& data);
  virtual bool needBalancing();
  void split();

  virtual void display(size_t depth = 0);
private:
  std::vector<BtreeLeaf<Tkey,Tdata>*> leafs_;
};

template<typename Tkey, typename Tdata>
BtreeLeaf<Tkey,Tdata>* BtreeLeafNode<Tkey,Tdata>::insert(const Tkey& key, const Tdata& data)
{
  BtreeLeaf<Tkey,Tdata>* res = nullptr;
  BtreeLeaf<Tkey,Tdata>* last = nullptr;
  if(leafs_.size())
    last = leafs_[leafs_.size() - 1];

  if(leafs_.size() == 0)
  {
    this->keys_.push_back(key);
    res = new BtreeLeaf<Tkey,Tdata>(key, data);
    leafs_.push_back(res);
    res->setMother(this);
  }
  else
  {
    if(this->keys_[this->keys_.size() - 1] == key)
    {
      last->push_back(data);
    }
    else if(key > this->keys_[this->keys_.size() - 1])
    {
      this->keys_.push_back(key);
      res = new BtreeLeaf<Tkey,Tdata>(key, data);
      leafs_.push_back(res);
      last->next_ = res;
      res->prev_ = last;
      res->setMother(this);
    }
    else
    {
      for(size_t i = 0; i < this->keys_.size(); i++)
      {
        if(this->keys_[i] >= key)
        {
          if(this->keys_[i] == key)
          {
            leafs_[i]->push_back(data);
          }
          else
          {
            res = new BtreeLeaf<Tkey,Tdata>(key, data);
            // here last is the next node of res
            last = leafs_[i];
            res->next_ = last;
            res->prev_ = last->prev_;
            if(res->prev_)
              res->prev_->next_ = res;
            if(res->next_)
              res->next_->prev_ = res;
            this->keys_.insert(this->keys_.begin() + i, key);
            this->leafs_.insert(this->leafs_.begin() + i, res);
            res->setMother(this);
          }
          break;
        }
      }
    }
  }

  if(needBalancing())
  {
    std::cout << "needBalancing" << std::endl;
    split();
  }

  return res;
}

template<typename Tkey, typename Tdata>
bool BtreeLeafNode<Tkey,Tdata>::needBalancing()
{
  return (leafs_.size() > this->order_);
}

template<typename Tkey, typename Tdata>
void BtreeLeafNode<Tkey,Tdata>::split()
{
  BtreeLeafNode<Tkey,Tdata>* new_node = new BtreeLeafNode<Tkey,Tdata>(this->order_);

  for(size_t i = 0; i < this->order_/2; i++)
  {
    new_node->leafs_.insert(new_node->leafs_.begin(), leafs_[leafs_.size() - 1]);
    leafs_.erase(leafs_.begin() + leafs_.size() - 1);
    new_node->leafs_[i]->setMother(new_node);
  }

  for(size_t i = 0; i < this->order_/2; i++)
  {
    new_node->keys_.insert(new_node->keys_.begin(), this->keys_[this->keys_.size() - 1]);
    this->keys_.erase(this->keys_.begin() + this->keys_.size() - 1);
  }

  if(this->mother_ != nullptr)
  {
    this->mother_->insert(new_node, new_node->keys_[0]);
  }
  else
  {
    BtreeNode<Tkey,Tdata>* new_mother = new BtreeNode<Tkey,Tdata>(this->order_);
    new_mother->insert(this, new_node->keys_[0]);
    new_mother->insert(new_node, new_node->keys_[0]);
  }
}

template<typename Tkey, typename Tdata>
void BtreeLeafNode<Tkey,Tdata>::display(size_t depth)
{
  for(size_t i = 0; i < this->keys_.size(); i++)
  {
    for(size_t j = 0; j < depth; j++)
      std::cout << "\t";
    std::vector<Tdata> datas = leafs_[i]->getData();
    std::cout << this->keys_[i] << " => ";
    for(auto data : datas)
      std::cout << data << " : ";
    std::cout << std::endl;
  }
}

} // namespace mementar

#endif // MEMENTAR_BTREELEAFNODE_H