#pragma once

#include "../../rosidl.hpp"

namespace example_interfaces
{

namespace srv
{

namespace detail
{
  struct DoubleInt_Request {
    int num;
  };
  struct DoubleInt_Response {
    int result;
  };
}

struct DoubleInt
{
  using Request = detail::DoubleInt_Request;
  using Response = detail::DoubleInt_Response;
};

}  // namespace srv

}  // namespace example_interfaces

namespace rosidl_typesupport_cpp
{
template<>
const rosidl_type_support_t *
get_service_type_support_handle<example_interfaces::srv::DoubleInt>()
{
  static auto this_type_support = rosidl_type_support_t {nullptr};
  return &this_type_support;
}
}

namespace rosidl_generator_traits
{
template<>
struct is_service<example_interfaces::srv::DoubleInt> : std::true_type {};
}