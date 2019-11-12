#ifndef MEMENTAR_API_FACT_H
#define MEMENTAR_API_FACT_H

#include <string>
#include <vector>

namespace mementar
{

class Fact
{
public:
  Fact(const std::string& fact)
  {
    std::vector<std::string> splitted = split(fact, "|");
    if(splitted.size() >= 1)
      subject_ = splitted[0];
    if(splitted.size() >= 2)
      predicat_ = splitted[1];
    if(splitted.size() >= 3)
      object_ = splitted[2];
  }

  Fact(const std::string& subject, const std::string& predicat, const std::string& object)
  {
    subject_ = subject;
    predicat_ = predicat;
    object_ = object;
  }

  std::string subject() const { return subject_; }
  std::string predicat() const { return predicat_; }
  std::string object() const { return object_; }
  std::string operator()() const {return subject_ + "|" + predicat_ + "|" + object_; }

  void subject(const std::string& subject) { subject_ = subject; }
  void predicat(const std::string& predicat) { predicat_ = predicat; }
  void object(const std::string& object) { object_ = object; }
  void operator()(const std::string& fact)
  {
    std::vector<std::string> splitted = split(fact, "|");
    if(splitted.size() >= 1)
      subject_ = splitted[0];
    if(splitted.size() >= 2)
      predicat_ = splitted[1];
    if(splitted.size() >= 3)
      object_ = splitted[2];
  }

private:
  std::string subject_;
  std::string predicat_;
  std::string object_;

  std::vector<std::string> split(const std::string& str, const std::string& delim)
  {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
      pos = str.find(delim, prev);
      if (pos == std::string::npos)
        pos = str.length();

      std::string token = str.substr(prev, pos-prev);

      if (!token.empty())
        tokens.push_back(token);
      prev = pos + delim.length();
    }
    while ((pos < str.length()) && (prev < str.length()));

    return tokens;
  }
};

} // namespace mementar

#endif // MEMENTAR_API_FACT_H
