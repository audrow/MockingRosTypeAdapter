#pragma once

#include <functional>
#include <memory>
#include <variant>

#include "../rosidl.hpp"
#include "type_adaptation.hpp"

namespace rclcpp
{

namespace detail {

template<
  typename MessageT,
  bool is_adapted_type = !rosidl_generator_traits::is_message<MessageT>::value
>
struct AnySubscriptionCallbackHelper;

template<typename MessageT>
struct AnySubscriptionCallbackHelper<MessageT, true>
{
  using variant_type = std::variant<
    std::function<void (const MessageT &)>,
    std::function<void (std::unique_ptr<MessageT>)>,
    std::function<void (std::shared_ptr<const MessageT>)>,
    std::function<void (const typename TypeAdaptor<MessageT>::ROSMessageType &)>,
    std::function<void (std::unique_ptr<typename TypeAdaptor<MessageT>::ROSMessageType>)>,
    std::function<void (std::shared_ptr<const typename TypeAdaptor<MessageT>::ROSMessageType>)>
  >;
};

template<typename MessageT>
struct AnySubscriptionCallbackHelper<MessageT, false>
{
  using variant_type = std::variant<
    std::function<void (const MessageT &)>,
    std::function<void (std::unique_ptr<MessageT>)>,
    std::function<void (std::shared_ptr<const MessageT>)>
  >;
};

}

template<
  typename MessageT,
  typename VariantT = typename detail::AnySubscriptionCallbackHelper<MessageT>::variant_type
>
class AnySubscriptionCallback : public VariantT
{
public:
  using VariantT::VariantT;
};

}
