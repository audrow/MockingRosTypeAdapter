#pragma once

namespace rclcpp
{

template<
  typename MessageT
>
constexpr
typename std::enable_if_t<
  rosidl_generator_traits::is_message<MessageT>::value,
  const rosidl_type_support_t *
>
resolve_message_type_support_handle()
{
  return rosidl_typesupport_cpp::get_message_type_support_handle<MessageT>();
}

template<
  typename AdaptedType
>
constexpr
typename std::enable_if_t<
  !rosidl_generator_traits::is_message<AdaptedType>::value &&
  TypeAdaptor<AdaptedType>::is_specialized::value,
  const rosidl_type_support_t *
>
resolve_message_type_support_handle()
{
  return rosidl_typesupport_cpp::get_message_type_support_handle<
    typename TypeAdaptor<AdaptedType>::ROSMessageType
  >();
}

template<
  typename AdaptedType
>
constexpr
typename std::enable_if_t<
  !rosidl_generator_traits::is_message<AdaptedType>::value &&
  !TypeAdaptor<AdaptedType>::is_specialized::value,
  const rosidl_type_support_t *
>
resolve_message_type_support_handle()
{
  throw std::runtime_error("this specialization of resolve_message_type_support_handle() should never be called");
}

}
