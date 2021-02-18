#pragma once

#include "any_subscription_callback.hpp"
#include "publisher.hpp"
#include "subscription.hpp"
#include "client.hpp"

namespace rclcpp
{

class Node
{
public:
  Node(const std::string & name)
  : name_(name)
  {}

  template<
    typename MessageT,
    typename PublisherT = rclcpp::Publisher<MessageT>>
  std::shared_ptr<PublisherT>
  create_publisher(
    const std::string & topic_name
  )
  {
    return rclcpp::create_publisher<MessageT, PublisherT, Node>(topic_name);
  }

  template<
    typename MessageT,
    typename SubscriptionT = rclcpp::Subscription<MessageT>>
  std::shared_ptr<SubscriptionT>
  create_subscription(
    const std::string & topic_name,
    AnySubscriptionCallback<MessageT> callback
  )
  {
    return rclcpp::create_subscription<MessageT, SubscriptionT, Node>(topic_name, callback);
  }

  template<
    typename ServiceT,
    typename ClientT = rclcpp::Client<ServiceT>>
  std::shared_ptr<ClientT>
  create_client(
    const std::string & topic_name
  )
  {
    return rclcpp::create_client<ServiceT, ClientT, Node>(topic_name);
  }

private:
  std::string name_;
};

}
