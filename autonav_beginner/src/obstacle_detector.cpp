#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"





// #include <memory>

// #include "rclcpp/rclcpp.hpp"
// #include "std_msgs/msg/string.hpp"
// using std::placeholders::_1;

// class MinimalSubscriber : public rclcpp::Node
// {
//   public:
//     MinimalSubscriber()
//     : Node("minimal_subscriber")
//     {
//       subscription_ = this->create_subscription<std_msgs::msg::String>(
//       "topic", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
//     }

//   private:
//     void topic_callback(const std_msgs::msg::String & msg) const
//     {
//       RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
//     }
//     rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
// };

// int main(int argc, char * argv[])
// {
//   rclcpp::init(argc, argv);
//   rclcpp::spin(std::make_shared<MinimalSubscriber>());
//   rclcpp::shutdown();
//   return 0;





using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */
float random_num(){
    return (std::rand()%39 +2)/10.0; // Generate a random number between 0.2 and 4.0
}
class obstacle_detector : public rclcpp::Node
{
  public:
    obstacle_detector()
    : Node("obstacle_detector"), count_(0)
    {
      subscription_ = this->create_subscription<std_msgs::msg::String>("/lidar/ranges", 10, std::bind(&obstacle_detector::topic_callback, this, std::placeholders::_1));
      publisher_ = this->create_publisher<std_msgs::msg::String>("/obstacle/status", 10);
      
    }

  private:
    void topic_callback(const std_msgs::msg::String & msg)const
    {
      std::stringstream ss(msg.data);
      std::string token;
      float min_range = 10.0f; // Initialize with a large value
      while (std::getline(ss, token, ',')) {
        float range = std::stof(token);
        if (range < min_range) {
          min_range = range;
        }
      }
      auto status_message = std_msgs::msg::String();
      if (min_range < 0.5) {
        status_message.data = "DANGER";
        RCLCPP_WARN(this->get_logger(), "Obstacle detected! Minimum range: %f", min_range);
      }else if (min_range >=0.5 && min_range < 1.5) {
        status_message.data = "CAUTION";
        
      } else {
        status_message.data = "CLEAR";
        
      }


      publisher_->publish(status_message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<obstacle_detector>());
  rclcpp::shutdown();
  return 0;
}