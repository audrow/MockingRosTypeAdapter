#pragma once

#include "resolve_service_type_support_handle.hpp"
#include "type_adaptation.hpp"

namespace rclcpp
{

template<class ServiceT>
class Client
{
public:
  Client(const std::string & topic_name)
  : topic_name_(topic_name),
    type_support_(resolve_service_type_support_handle<ServiceT>())
  {
    static_assert(rclcpp::is_ros_compatible_type<ServiceT>::value, "given service type is not compatible with ROS and cannot be used with a Client");
  }

  // Only if type is not an adapted type, i.e. it is a real ROS service type
  template<typename T>
  typename std::enable_if_t<
    rosidl_generator_traits::is_service<ServiceT>::value 
    && std::is_same<typename ServiceT::Request, T>::value
  >
  async_send_request(const std::shared_ptr<T> & request)
  {
    // here we know ServiceT is already a ROS service
    // rcl_publish
  }

  // Only if it is an adapted type
  template<typename T>
  typename std::enable_if_t<
    !rosidl_generator_traits::is_service<ServiceT>::value &&
    std::is_same<typename ServiceT::Request, T>::value
  >
  async_send_request(const T & request)
  {
    // here we know ServiceT needs to be converted to ROS type
    this->publish(TypeAdaptor<ServiceT>::convert_to_ros_service(request));
  }

  // Only if it is an adapted type
  template<typename T>
  typename std::enable_if_t<
    !rosidl_generator_traits::is_service<ServiceT>::value &&
    rosidl_generator_traits::is_service<T>::value
  >
  async_send_request(const T & request)
  {
    static_assert(std::is_same<T, typename TypeAdaptor<ServiceT>::ROSServiceType>::value, "given ros service type does not match adapted type");
    // rcl_publish
  }

private:
  std::string topic_name_;
  const rosidl_type_support_t * type_support_;
};

template<class ServiceT, class ClientT, class NodeT>
std::shared_ptr<ClientT>
create_client(const std::string & topic_name)
{
  // In rclcpp other logic happens here.
  return std::make_shared<ClientT>(topic_name);
}

}