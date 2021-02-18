#include <cstdio>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

template<>
struct rclcpp::TypeAdaptor<std::string>
{
  using is_specialized = std::true_type;
  using ROSMessageType = std_msgs::msg::String;

  static
  ROSMessageType
  convert_to_ros_message(const std::string & input_msg)
  {
    return std_msgs::msg::String{input_msg};
  }
};

int main() {
  using std_msgs::msg::String;

  auto node = rclcpp::Node("node");

  auto pub1 = node.create_publisher<String>("topic");
  auto pub2 = node.create_publisher<std::string>("topic");

  std_msgs::msg::String msg;
  msg.data = "hi";
  pub1->publish(msg);

  std::string str_msg = "hi there";
  pub2->publish(msg);
  pub2->publish(str_msg);

  auto cb_a1 = [](const String & msg) {printf("%s\n", msg.data.c_str());};
  auto cb_a2 = [](std::unique_ptr<String> msg) {printf("%s\n", msg->data.c_str());};
  // TODO(wjwwood): figure out why auto doesn't work
  // auto cb_a3 = [](std::shared_ptr<const String> msg) {printf("%s\n", msg->data.c_str());};
  std::function<void (std::shared_ptr<const String>)> cb_a3 =
    [](std::shared_ptr<const String> msg) {printf("%s\n", msg->data.c_str());};

  {
    auto sub1 = node.create_subscription<String>("topic", cb_a1);
    auto sub2 = node.create_subscription<String>("topic", cb_a2);
    auto sub3 = node.create_subscription<String>("topic", cb_a3);
  }

  auto cb_b1 = [](const std::string & msg) {printf("%s\n", msg.c_str());};
  auto cb_b2 = [](std::unique_ptr<std::string> msg) {printf("%s\n", msg->c_str());};
  // TODO(wjwwood): figure out why auto doesn't work
  // auto cb_b3 = [](std::shared_ptr<const std::string> msg) {printf("%s\n", msg->c_str());};
  std::function<void (std::shared_ptr<const std::string>)> cb_b3 =
    [](std::shared_ptr<const std::string> msg) {printf("%s\n", msg->c_str());};

  {
    auto sub1 = node.create_subscription<std::string>("topic", cb_a1);
    auto sub2 = node.create_subscription<std::string>("topic", cb_a2);
    auto sub3 = node.create_subscription<std::string>("topic", cb_a3);

    auto sub4 = node.create_subscription<std::string>("topic", cb_b1);
    auto sub5 = node.create_subscription<std::string>("topic", cb_b2);
    auto sub6 = node.create_subscription<std::string>("topic", cb_b3);
  }

  return 0;
}

/*

Next:

- extend this example for Services (Audrow)
- extend this example for Actions
- reply to feedback on the REP (William)
- start the pull request to rclcpp (William)

Later:

- tests for rclcpp pr
- update examples
- implement out-of-the-box support for some types, e.g. cv::Mat? or port pcl_conversions?
- create tutorial, e.g. using MyVector3 -> geometry_msgs::msg::Vector3


*/
