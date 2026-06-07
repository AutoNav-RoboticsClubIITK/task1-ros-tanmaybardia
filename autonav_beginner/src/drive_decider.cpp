#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class DriveDecider : public rclcpp::Node {
public:
    DriveDecider() : Node("drive_decider") {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "/obstacle/status", 10, std::bind(&DriveDecider::topic_callback, this, std::placeholders::_1));
        publisher_ = this->create_publisher<std_msgs::msg::String>("/robot/action", 10);
    }

private:
    void topic_callback(const std_msgs::msg::String & msg) const {
        auto action_msg = std_msgs::msg::String();

        if (msg.data == "CLEAR") {
            action_msg.data = "MOVE FORWARD";
        } else if (msg.data == "CAUTION") {
            action_msg.data = "SLOW DOWN";
        } else if (msg.data == "DANGER") {
            action_msg.data = "STOP";
        } else {
            return; // Ignore unknown states
        }

        RCLCPP_INFO(this->get_logger(), "Action decided: %s", action_msg.data.c_str());
        publisher_->publish(action_msg);
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DriveDecider>());
    rclcpp::shutdown();
    return 0;
}