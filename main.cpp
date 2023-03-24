/*
for UDP config
mbed_app.json

{
    "target_overrides": {
        "*": {
            "platform.stdio-convert-newlines": true,
            "target.network-default-interface-type": "ETHERNET"
        }
    }
}
*/

#include "PinNameAliases.h"
#include "PwmOut.h"
#include "String.hpp"
#include "Twist.hpp"
#include "mbed.h"
#include "ros2mbed.h"
#include "stream_storage_internal.h"
#include "subscription.hpp"

#if USER_TRANSPORT_TYPE == 0 || USER_TRANSPORT_TYPE == 1
#include "EthernetInterface.h"
#endif 

#include <string>

PwmOut motorRF(D11);
PwmOut motorRB(D10);
PwmOut motorLF(D9);
PwmOut motorLB(D6);
PwmOut motorW1(D5);
PwmOut motorW2(D3);

class SamplePub : public ros2::Node
{
public:
    SamplePub()
    : Node("sample_node"), count_(0)
    {
        pub_ = this->create_publisher<std_msgs::String>("string_pub");

        sub_R = this->create_subscription<geometry_msgs::Twist>(
            "/cmd_vel/move/R",
            [&](std::shared_ptr<geometry_msgs::Twist> msg)
            {
                motorRB = 100 * msg->linear.y;
                motorRF = 100 * msg->linear.y;
                motorW1 = 100 * msg->linear.y;
            });

        sub_L = this->create_subscription<geometry_msgs::Twist>(
            "/cmd_vel/move/L",
            [&](std::shared_ptr<geometry_msgs::Twist> msg)
            {
                motorLB = 100 * msg->linear.y;
                motorLF = 100 * msg->linear.y;
            });
        
        sub_wheel = this->create_subscription<geometry_msgs::Twist>(
            "/cmd_vel/wheel", 
        [&](std::shared_ptr<geometry_msgs::Twist> msg)
        {
            motorW2 = 1000 * msg->angular.y;
        });
        


        timer_ = this->create_wall_timer(
            500ms,
            [&](){
                auto msg = std::make_shared<std_msgs::String>();
                msg->data = "Hello ros2mbed! " + std::to_string(count_++);
                pub_->publish(msg);
            });

        
    }

private:
    int count_;
    ros2::Publisher<std_msgs::String>::SharedPtr pub_;
    ros2::Subscription<geometry_msgs::Twist>::SharedPtr sub_R;
    ros2::Subscription<geometry_msgs::Twist>::SharedPtr sub_L;
    ros2::Subscription<geometry_msgs::Twist>::SharedPtr sub_wheel;
    
    ros2::TimerBase::SharedPtr timer_;
};


int main()
{

#if USER_TRANSPORT_TYPE == 0 || USER_TRANSPORT_TYPE == 1
    EthernetInterface net;
    ros2::init(&net, "192.168.2.101", 2018);
#endif 

#if USER_TRANSPORT_TYPE == 2
    BufferedSerial pc(USBTX, USBRX, 115200);
    ros2::init(&pc);
#endif

    SamplePub sample_node;
    ros2::spin(&sample_node);
}