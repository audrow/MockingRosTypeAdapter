#pragma once

#include "../../rosidl.hpp"

namespace std_msgs::msg
{
struct String
{
  std::string data;
};
}

namespace rosidl_typesupport_cpp
{
template<>
const rosidl_type_support_t *
get_message_type_support_handle<std_msgs::msg::String>()
{
  static auto this_type_support = rosidl_type_support_t {nullptr};
  return &this_type_support;
}
}

namespace rosidl_generator_traits
{
template<>
struct is_message<std_msgs::msg::String> : std::true_type {};
}
