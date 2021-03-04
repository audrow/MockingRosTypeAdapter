#pragma once

#include "resolve_message_type_support_handle.hpp"
#include "type_adaptation.hpp"

namespace rclcpp
{

template<class MessageT>
class Subscription
{
public:
  Subscription(const std::string & topic_name, AnySubscriptionCallback<MessageT> callback)
  : topic_name_(topic_name),
    type_support_(resolve_message_type_support_handle<MessageT>()),
    callback_(callback)
  {
    static_assert(rclcpp::is_ros_compatible_type<MessageT>::value, "given message type is not compatible with ROS and cannot be used with a Subscription");
  }

private:
  std::string topic_name_;
  const rosidl_type_support_t * type_support_;
  AnySubscriptionCallback<MessageT> callback_;
};

template<class MessageT, class SubscriptionT, class NodeT>
std::shared_ptr<SubscriptionT>
create_subscription(const std::string & topic_name, AnySubscriptionCallback<MessageT> callback)
{
  // In rclcpp other logic happens here.
  return std::make_shared<SubscriptionT>(topic_name, callback);
}

}
