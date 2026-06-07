#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */
float random_num(){
    return (std::rand()%39 +2)/10.0; // Generate a random number between 0.2 and 4.0
}
class lidar_publisher : public rclcpp::Node
{
  public:
    lidar_publisher()
    : Node("lidar_publisher"), count_(0)
    {
      publisher_ = this->create_publisher<std_msgs::msg::String>("/lidar/ranges", 10);
      timer_ = this->create_wall_timer(
      1000ms, std::bind(&lidar_publisher::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      auto message = std_msgs::msg::String();

      message.data = std::to_string(random_num())+","+std::to_string(random_num())+","+std::to_string(random_num()) + "," + std::to_string(random_num())+","+std::to_string(random_num());
      
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<lidar_publisher>());
  rclcpp::shutdown();
  return 0;
}