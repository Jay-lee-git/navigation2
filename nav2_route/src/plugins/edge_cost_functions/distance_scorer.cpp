// Copyright (c) 2023, Samsung Research America
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>
#include <string>

#include "nav2_route/plugins/edge_cost_functions/distance_scorer.hpp"

namespace nav2_route
{

void DistanceScorer::configure(
  const rclcpp_lifecycle::LifecycleNode::SharedPtr & node,
  const std::string & name)
{
  name_ = name;

  // Find the tag at high the speed limit information is stored
  nav2_util::declare_parameter_if_not_declared(
    node, getName() + ".speed_tag", rclcpp::ParameterValue("speed_limit"));
  speed_tag_ = node->get_parameter(getName() + ".speed_tag").as_string();

  // Find the proportional weight to apply, if multiple cost functions
  nav2_util::declare_parameter_if_not_declared(
    node, getName() + ".weight", rclcpp::ParameterValue(1.0));
  weight_ = static_cast<float>(node->get_parameter(getName() + ".weight").as_double());
}

float DistanceScorer::score(const EdgePtr edge)
{
  // Get the speed limit, if set for an edge
  float speed_val = 1.0f;
  speed_val = edge->metadata.getValue<float>(speed_tag_, speed_val);
  return weight_ * hypotf(
    edge->end->coords.x - edge->start->coords.x,
    edge->end->coords.y - edge->start->coords.y) / speed_val;
}

std::string DistanceScorer::getName()
{
  return name_;
}

}  // namespace nav2_route

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(nav2_route::DistanceScorer, nav2_route::EdgeCostFunction)