#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include <map>
#include <memory>
#include <string>
#include <vector>

struct Subscription;

struct Message
{
  using key_t = std::string;
  using val_t = std::string;
  using map_t = std::map<key_t,val_t>;
public:
  Message() = default;
  Message(std::string const&);
  void set(key_t const&,val_t const&);
  val_t get(key_t const&) const;
  std::string str() const;
private:
  map_t m_props;
};
using MessagePtr = std::shared_ptr<Message>;

struct MsgQueue
{
  void insert(MessagePtr m);
  void remove(std::string const& source);
  MessagePtr top();
private:
  std::vector<MessagePtr> m_queue;
};
#endif /* MESSAGEQUEUE_H */
