#pragma once

#include <functional>
#include <memory>
#include <variant>

#include "../rosidl.hpp"
#include "type_adaptation.hpp"
#include "always_false.hpp"


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
class AnySubscriptionCallback 
{
public:

  using variant_type = VariantT;

  AnySubscriptionCallback(VariantT callback)
    : callback_(callback) {}

  void run(
    MessageT arg)
  {
    std::visit([arg](auto &&callback) {
      using T = std::decay_t<decltype(callback)>;
      if constexpr (
          std::is_same_v<std::function<void(const MessageT &)>, T>)
      {
        callback(arg);
      }
      else if constexpr (
          std::is_same_v<std::function<void(std::unique_ptr<MessageT>)>, T> ||
          std::is_same_v<std::function<void(std::shared_ptr<const MessageT>)>, T>)
      {
        auto ptr = std::make_unique<MessageT>(arg);
        callback(std::move(ptr));
      }
      else
      {
        static_assert(always_false_v<T>, "non-exhaustive visitor!");
      }
    }, callback_);
  }

private:
  VariantT callback_;
};

}
