#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>
#include <sstream>

class MyPublisher : public rclcpp::Node
{
public:
  MyPublisher() : Node("my_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("my_topic", 10);
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(1000),
      std::bind(&MyPublisher::publish_message, this));
    RCLCPP_INFO(this->get_logger(), "Publisher node started.");
  }

private:
  void publish_message()
  {
    auto message = std_msgs::msg::String();
    std::ostringstream oss;
    oss << "Message #" << count_ << " | Uptime: " 
        << (count_ * 1.0) << "s";
    message.data = oss.str();
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
    count_++;
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MyPublisher>());
  rclcpp::shutdown();
  return 0;
}