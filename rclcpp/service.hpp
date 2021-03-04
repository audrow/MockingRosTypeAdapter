#pragma once

#include <memory>

#include "any_subscription_callback.hpp"
#include "resolve_message_type_support_handle.hpp"
#include "type_adaptation.hpp"

namespace rclcpp
{

template<class ServiceT>
class Service
{
public:
  Service(
    const std::string & topic_name, 
    AnySubscriptionCallback<typename ServiceT::Request> callback
  )
  : topic_name_(topic_name),
    request_type_support_(resolve_message_type_support_handle<typename ServiceT::Request>()),
    response_type_support_(resolve_message_type_support_handle<typename ServiceT::Response>()),
    callback_(callback)
  {
    static_assert(rclcpp::is_ros_compatible_type<typename ServiceT::Request>::value, "given service request type is not compatible with ROS and cannot be used with a Client");
    static_assert(rclcpp::is_ros_compatible_type<typename ServiceT::Response>::value, "given service response type is not compatible with ROS and cannot be used with a Client");
  }
private:
  std::string topic_name_;
  const rosidl_type_support_t * request_type_support_;
  const rosidl_type_support_t * response_type_support_;
  AnySubscriptionCallback<typename ServiceT::Request> callback_;
};

template<class ServiceT, class ServerT, class NodeT>
std::shared_ptr<ServerT>
create_service(
  const std::string & topic_name, 
  AnySubscriptionCallback<typename ServiceT::Request> callback
)
{
  return std::make_shared<ServerT>(topic_name, callback);
}

}