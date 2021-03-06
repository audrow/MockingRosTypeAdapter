#pragma once

#include <iostream>
#include <future>

#include "resolve_message_type_support_handle.hpp"
#include "type_adaptation.hpp"

namespace rclcpp
{

template<class ServiceT>
class Client
{
public:
  Client(const std::string & topic_name)
  : topic_name_(topic_name),
    request_type_support_(resolve_message_type_support_handle<typename ServiceT::Request>()),
    response_type_support_(resolve_message_type_support_handle<typename ServiceT::Response>())
  {
    static_assert(rclcpp::is_ros_compatible_type<typename ServiceT::Request>::value, "given service request type is not compatible with ROS and cannot be used with a Client");
    static_assert(rclcpp::is_ros_compatible_type<typename ServiceT::Response>::value, "given service response type is not compatible with ROS and cannot be used with a Client");
  }

  // Only if type is not an adapted type, i.e. it is a real ROS service type
  template<typename T>
  typename std::enable_if_t<
    rosidl_generator_traits::is_message<typename ServiceT::Request>::value 
    && std::is_same<typename ServiceT::Request, T>::value,
    std::future<typename ServiceT::Response>
  >
  async_send_request(const T & request)
  {
    std::cout << "Sending ROS message (not adapted case)" << std::endl;
    // here we know ServiceT is already a ROS service
    return std::async(get_response<typename ServiceT::Response>);
  }

  // Only if it is an adapted type
  template<typename T>
  typename std::enable_if_t<
    !rosidl_generator_traits::is_message<typename ServiceT::Request>::value
    && std::is_same<typename ServiceT::Request, T>::value,
    std::future<typename ServiceT::Response>
  >
  async_send_request(const T & request)
  {
    // here we know ServiceT needs to be converted to ROS type
    std::cout << "Called for adapted type -> "; 
    return this->async_send_request(TypeAdaptor<T>::convert_to_ros_message(request));
  }

  // Only if it is an adapted type
  template<typename T>
  typename std::enable_if_t<
    !rosidl_generator_traits::is_message<typename ServiceT::Request>::value &&
    rosidl_generator_traits::is_message<T>::value,
    std::future<typename ServiceT::Response>
  >
  async_send_request(const T & request)
  {
    std::cout << "Sending ROS message (adapted case)" << std::endl;
    static_assert(std::is_same<T, typename TypeAdaptor<typename ServiceT::Request>::ROSMessageType>::value, "given ros service type does not match adapted type");
    return std::async(get_response<typename ServiceT::Response>);
  }


private:

  // When the response is NOT type adapted
  template<typename ResponseT = typename ServiceT::Response>
  typename std::enable_if_t<
    rosidl_generator_traits::is_message<ResponseT>::value,
    ResponseT
  >
  static 
  get_response() {
    return ResponseT();
  }

  // When the response is type adapted
  template<typename ResponseT = typename ServiceT::Response>
  typename std::enable_if_t<
    !rosidl_generator_traits::is_message<ResponseT>::value,
    ResponseT
  >
  static 
  get_response() {
    auto msg = typename TypeAdaptor<ResponseT>::ROSMessageType();
    auto out = TypeAdaptor<ResponseT>::convert_from_ros_message(msg);
    return out;
  }

  std::string topic_name_;
  const rosidl_type_support_t * request_type_support_;
  const rosidl_type_support_t * response_type_support_;
};

template<class ServiceT, class ClientT, class NodeT>
std::shared_ptr<ClientT>
create_client(const std::string & topic_name)
{
  // In rclcpp other logic happens here.
  return std::make_shared<ClientT>(topic_name);
}

}