#pragma once

#include <memory>
#include <iostream>

#include "any_subscription_callback.hpp"
#include "resolve_message_type_support_handle.hpp"
#include "type_adaptation.hpp"

namespace rclcpp
{

template<class ServiceT>
class Service
{

using RequestT = typename ServiceT::Request;
using ResponseT = typename ServiceT::Response;

public:
  Service(
    const std::string & topic_name, 
    typename AnySubscriptionCallback<RequestT>::variant_type callback
  )
  : topic_name_(topic_name),
    request_type_support_(resolve_message_type_support_handle<RequestT>()),
    response_type_support_(resolve_message_type_support_handle<ResponseT>()),
    any_callback_(callback)
  {
    static_assert(rclcpp::is_ros_compatible_type<RequestT>::value, "given service request type is not compatible with ROS and cannot be used with a Client");
    static_assert(rclcpp::is_ros_compatible_type<ResponseT>::value, "given service response type is not compatible with ROS and cannot be used with a Client");
  }

  void
  handle_request(
    std::shared_ptr<RequestT> request
  )
  {
    std::cout << "Shared called\n";
    any_callback_.dispatch(request);
  }

  void
  handle_request(
    RequestT request
  )
  {
    std::cout << "Standard called\n";
    handle_request(std::make_shared<RequestT>(request));
  }

  void
  handle_request(
    std::unique_ptr<RequestT> request
  )
  {
    std::cout << "Unique called\n";
    std::shared_ptr<RequestT> ptr(std::move(request));
    handle_request(ptr);
  }

private:
  std::string topic_name_;
  const rosidl_type_support_t * request_type_support_;
  const rosidl_type_support_t * response_type_support_;
  AnySubscriptionCallback<typename ServiceT::Request> any_callback_;
};

template<class ServiceT, class ServerT, class NodeT>
std::shared_ptr<ServerT>
create_service(
  const std::string & topic_name, 
  typename AnySubscriptionCallback<typename ServiceT::Request>::variant_type callback
)
{
  return std::make_shared<ServerT>(topic_name, callback);
}

}
