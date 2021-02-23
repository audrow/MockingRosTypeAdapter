#pragma once

typedef struct rosidl_type_support_t
{
  void * impl;
} rosidl_type_support_t;

namespace rosidl_typesupport_cpp
{
template<class MessageT>
const rosidl_type_support_t *
get_message_type_support_handle();
}

namespace rosidl_generator_traits
{
template<typename T>
struct is_message : std::false_type {};
}