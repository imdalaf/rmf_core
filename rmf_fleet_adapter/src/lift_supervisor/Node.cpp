/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include "Node.hpp"

#include <rmf_fleet_adapter/StandardNames.hpp>
#include <rmf_traffic_ros2/StandardNames.hpp>

namespace rmf_fleet_adapter {
namespace lift_supervisor {

//==============================================================================
Node::Node()
: rclcpp::Node("rmf_lift_supervisor")
{
  const auto default_qos = rclcpp::SystemDefaultsQoS();

  _lift_request_pub = create_publisher<LiftRequest>(
    FinalLiftRequestTopicName, default_qos);

  _adapter_lift_request_sub = create_subscription<LiftRequest>(
    AdapterLiftRequestTopicName, default_qos,
    [&](LiftRequest::UniquePtr msg)
    {
      _adapter_lift_request_update(std::move(msg));
    });

  _lift_state_sub = create_subscription<LiftState>(
    LiftStateTopicName, default_qos,
    [&](LiftState::UniquePtr msg)
    {
      _lift_state_update(std::move(msg));
    });

  _emergency_notice_pub = create_publisher<EmergencyNotice>(
    rmf_traffic_ros2::EmergencyTopicName, default_qos);
}

//==============================================================================
void Node::_adapter_lift_request_update(LiftRequest::UniquePtr msg)
{
  auto& lift_requests = _schedule[msg->lift_name];
  bool is_new = true;

  for (auto const& i : lift_requests)
  {
    if ((i->destination_floor == msg->destination_floor) &&
        (i->door_state == msg->door_state))
    {
      is_new = false;
      break;
    }
  }

  if (is_new)
    lift_requests.push_back(std::move(msg));

  // For now we will simply queue the requests and forward one by one
  // TODO(MXG): Make this more intelligent by scheduling the lift
  if (!lift_requests.empty())
    _lift_request_pub->publish(*lift_requests.front());
}

//==============================================================================
void Node::_lift_state_update(LiftState::UniquePtr msg)
{
  auto& lift_requests = _schedule[msg->lift_name];

  if (!lift_requests.empty())
  {
    if ((lift_requests.front()->destination_floor == msg->current_floor) &&
        (lift_requests.front()->door_state == msg->door_state))
      lift_requests.pop_front();
  }

  if (!lift_requests.empty())
    _lift_request_pub->publish(*lift_requests.front());

  // For now, we do not need to publish this.

//  std_msgs::msg::Bool emergency_msg;
//  emergency_msg.data = false;

//  if (LiftState::MODE_FIRE == msg->current_mode
//      || LiftState::MODE_EMERGENCY == msg->current_mode)
//  {
//    emergency_msg.data = true;
//  }

//  _emergency_notice_pub->publish(emergency_msg);
}

} // namespace lift_supervisor
} // namespace rmf_fleet_adapter
