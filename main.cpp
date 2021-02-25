#include <cstdio>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "example_interfaces/srv/doubleint.hpp"

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

void test_topics() {
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
}


template<>
struct rclcpp::TypeAdaptor<int>
{
  using is_specialized = std::true_type;
  using ROSMessageType = example_interfaces::srv::DoubleInt::Request;

  static
  ROSMessageType
  convert_to_ros_message(const int & input_msg)
  {
    return ROSMessageType{input_msg};
  }
};

struct DoubleIntAdaptor {
  using Request = int;
  using Response = example_interfaces::srv::DoubleInt::Response;

  // TODO(audrow) make this work with test services below
  // using Request = example_interfaces::srv::DoubleInt::Request;
  // using Response = int;

  // TODO(audrow) figure out how to make this work
  // using Request = int;
  // using Response = int;
};

void test_services() {
  using example_interfaces::srv::DoubleInt;

  auto node = rclcpp::Node("node");

  auto client1 = node.create_client<DoubleInt>("topic");
  auto client2 = node.create_client<DoubleIntAdaptor>("topic");

  DoubleInt::Request request_ros;
  request_ros.num = 41;
  int request_adapted = 2;

  auto future1 = client1->async_send_request(request_ros);
  auto future2 = client2->async_send_request(request_adapted);
  auto future3 = client2->async_send_request(request_ros);

  std::cout << typeid(future1.get()).name() << std::endl;
  std::cout << typeid(future2.get()).name() << std::endl;
  std::cout << typeid(future3.get()).name() << std::endl;
}

int main() {

  // Can be built and run with the following command:
  // g++ -pthread --std=c++17 -o main -g main.cpp && ./main && rm main

  test_topics();
  test_services();

  return 0;
}

/*

Next:

- extend this example for Services (Audrow)
- extend this example for Actions
- update the REP with examples of type adapter declaration (Audrow if he has time)
- start the pull request to rclcpp (William)
  - TypeAdapter for pub/sub with covert only
  - (later) with serialization (require serializer interface which might touch rmw)
  - (later) integrate with the intra process stuff

Later:

- tests for rclcpp pr
- update examples
- implement out-of-the-box support for some types, e.g. cv::Mat? or port pcl_conversions?
- create tutorial, e.g. using MyVector3 -> geometry_msgs::msg::Vector3

*/
