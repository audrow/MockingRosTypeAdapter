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

/* (audrow)
 * This actually seems to use the type rosidl_service_type_support_t, 
 * and there is a similar type for messages - using rosidl_type_support_t
 * for simplicity and because its used in messages
 */
template<class ServiceT>
const rosidl_type_support_t *
get_service_type_support_handle();
}

namespace rosidl_generator_traits
{
// Pub/Sub
template<typename T>
struct is_message : std::false_type {};

// Services
template<typename T>
struct is_service : std::false_type {};

template<typename T>
struct is_service_request : std::false_type {};

template<typename T>
struct is_service_response : std::false_type {};
}