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

#include <rmf_battery/agv/SystemTraits.hpp>

#include <rmf_utils/catch.hpp>

SCENARIO("Test PowerSystem")
{
  rmf_battery::agv::SystemTraits::PowerSystem power_system(60, 12);
  REQUIRE(power_system.nominal_power() - 60 == Approx(0.0));
  REQUIRE(power_system.nominal_voltage() - 12 == Approx(0.0));
  REQUIRE(power_system.efficiency() - 1.0 == Approx(0.0));
  WHEN("Nominal power is set")
  {
    power_system.nominal_power(80);
    CHECK(power_system.nominal_power() - 80 == Approx(0.0));
    CHECK(power_system.nominal_voltage() - 12 == Approx(0.0));
    CHECK(power_system.efficiency() - 1.0 == Approx(0.0));
  }
  WHEN("Nominal voltage is set")
  {
    power_system.nominal_voltage(24);
    CHECK(power_system.nominal_voltage() - 24 == Approx(0.0));
    CHECK(power_system.nominal_power() - 60 == Approx(0.0));
    CHECK(power_system.efficiency() - 1.0 == Approx(0.0));
  }
  WHEN("Efficiency is set")
  {
    power_system.efficiency(0.80);
    CHECK(power_system.nominal_voltage() - 12 == Approx(0.0));
    CHECK(power_system.nominal_power() - 60 == Approx(0.0));
    CHECK(power_system.efficiency() - 0.80 == Approx(0.0));
  }
}