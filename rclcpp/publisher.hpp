#pragma once

#include "resolve_message_type_support_handle.hpp"
#include "type_adaptation.hpp"

namespace rclcpp
{

template<class MessageT>
class Publisher
{
public:
  Publisher(const std::string & topic_name)
  : topic_name_(topic_name),
    type_support_(resolve_message_type_support_handle<MessageT>())
  {
    static_assert(rclcpp::is_ros_compatible_type<MessageT>::value, "given message type is not compatible with ROS and cannot be used with a Publisher");
  }

  // Only if type is not an adapted type, i.e. it is a real ROS message type
  template<typename T>
  typename std::enable_if_t<
    rosidl_generator_traits::is_message<T>::value &&
    std::is_same<MessageT, T>::value
  >
  publish(const T & msg)
  {
    // here we know MessageT is already a ROS message
    // rcl_publish
  }

  // Only if it is an adapted type
  template<typename T>
  typename std::enable_if_t<
    !rosidl_generator_traits::is_message<MessageT>::value &&
    std::is_same<MessageT, T>::value
  >
  publish(const T & msg)
  {
    // here we know MessageT needs to be converted to ROS type
    this->publish(TypeAdaptor<MessageT>::convert_to_ros_message(msg));
  }

  // Only if it is an adapted type
  template<typename T>
  typename std::enable_if_t<
    !rosidl_generator_traits::is_message<MessageT>::value &&
    rosidl_generator_traits::is_message<T>::value
  >
  publish(const T & ros_msg)
  {
    static_assert(std::is_same<T, typename TypeAdaptor<MessageT>::ROSMessageType>::value, "given ros message type does not match adapted type");
    // rcl_publish
  }

private:
  std::string topic_name_;
  const rosidl_type_support_t * type_support_;
};

template<class MessageT, class PublisherT, class NodeT>
std::shared_ptr<PublisherT>
create_publisher(const std::string & topic_name)
{
  // In rclcpp other logic happens here.
  return std::make_shared<PublisherT>(topic_name);
}

}
