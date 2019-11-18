#ifndef MEMENTAR_OCCASIONSMANAGER_H
#define MEMENTAR_OCCASIONSMANAGER_H

#include <mutex>
#include <queue>
#include <atomic>

#include <ros/ros.h>

#include "mementar/MementarOccasionSubscription.h"
#include "mementar/MementarOcassionUnsubscription.h"

#include "mementar/core/Occasions/Subscription.h"
#include "mementar/core/Data/Fact.h"

namespace mementar
{

class OccasionsManager
{
public:
  OccasionsManager(ros::NodeHandle* n, std::string name = "");

  void run();

  void add(const Fact<time_t>* fact);

  void stop() {run_ = false; }
  inline bool isRunning() {return run_; }

private:
  ros::NodeHandle* n_;
  Subscription subscription_;
  std::atomic<bool> run_;

  ros::Publisher pub_;
  ros::ServiceServer sub_service_;
  ros::ServiceServer unsub_service_;

  std::mutex mutex_;

  bool queue_choice_;
  std::queue<const Fact<time_t>*> fifo_1;
  std::queue<const Fact<time_t>*> fifo_2;

  bool SubscribeCallback(mementar::MementarOccasionSubscription::Request &req,
                         mementar::MementarOccasionSubscription::Response &res);
  bool UnsubscribeCallback(mementar::MementarOcassionUnsubscription::Request &req,
                           mementar::MementarOcassionUnsubscription::Response &res);

  const Fact<time_t>* get();
  bool empty();
};

} // namespace mementar

#endif // MEMENTAR_OCCASIONSMANAGER_H