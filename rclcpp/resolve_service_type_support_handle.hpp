#pragma once

namespace rclcpp
{

template<
  typename ServiceT
>
constexpr
typename std::enable_if_t<
  rosidl_generator_traits::is_service<ServiceT>::value,
  const rosidl_type_support_t *
>
resolve_service_type_support_handle()
{
  return rosidl_typesupport_cpp::get_service_type_support_handle<ServiceT>();
}

template<
  typename AdaptedType
>
constexpr
typename std::enable_if_t<
  !rosidl_generator_traits::is_service<AdaptedType>::value &&
  TypeAdaptor<AdaptedType>::is_specialized::value,
  const rosidl_type_support_t *
>
resolve_service_type_support_handle()
{
  return rosidl_typesupport_cpp::get_service_type_support_handle<
    typename TypeAdaptor<AdaptedType>::ROSServiceType
  >();
}

template<
  typename AdaptedType
>
constexpr
typename std::enable_if_t<
  !rosidl_generator_traits::is_service<AdaptedType>::value &&
  !TypeAdaptor<AdaptedType>::is_specialized::value,
  const rosidl_type_support_t *
>
resolve_service_type_support_handle()
{
  throw std::runtime_error("this specialization of resolve_service_type_support_handle() should never be called");
}

}