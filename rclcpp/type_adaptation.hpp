#pragma once

#include "../rosidl.hpp"

namespace rclcpp
{

template<typename CustomType>
struct TypeAdaptor
{
  using is_specialized = std::false_type;
};

template<typename T>
struct is_ros_compatible_type
{
  static constexpr bool value =
    rosidl_generator_traits::is_message<T>::value ||
    rosidl_generator_traits::is_service<T>::value ||
    TypeAdaptor<T>::is_specialized::value;
};

}
