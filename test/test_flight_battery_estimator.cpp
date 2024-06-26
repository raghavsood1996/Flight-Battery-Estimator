/**
* @file test_flight_battery_estimator.cpp
* @author Raghav Sood (raghav2sood@gmail.com)
* @brief Unit tests for the FlightBatteryEstimator class
**/


#include <flight_battery_estimator/flight_battery_estimator.h>
#include <flight_battery_estimator/waypoint_wind_association_functions.h>

#include <eigen3/Eigen/src/Core/Matrix.h>
#include <gtest/gtest.h>

#include <vector>
#include <cstdlib>


namespace
{
    void addWaypoint(std::vector<FlightWaypoint>& waypoints, double x, double y)
    {
        FlightWaypoint waypoint;
        waypoint.position = Eigen::Vector2d(x, y);
        waypoints.push_back(waypoint);
    }

    void addWindData(std::vector<std::shared_ptr<WindData>>& wind_speeds, double x, double y, double wind_x,
                     double wind_y)
    {
        WindData wind_speed;
        wind_speed.position = Eigen::Vector2d(x, y);
        wind_speed.wind_speed = Eigen::Vector2d(wind_x, wind_y);
        wind_speeds.push_back(std::make_shared<WindData>(wind_speed));
    }
}


TEST(FlightBatteryEstimatorTest, TestEstimatedBatteryRemainingEmptyWaypoints)
{
    FlightBatteryEstimator flight_battery_estimator{
        flight_battery_estimator::associateWindVectorsWithWaypointsUsingClosestWindVector
    };
    std::vector<FlightWaypoint> waypoints;
    const std::vector<std::shared_ptr<WindData>> wind_speeds;

    constexpr double mission_airspeed = 30.0;
    constexpr double battery_initial_charge = 100.0;
    constexpr double constant_speed_power_consumption = 500.0;

    const double battery_remaining = flight_battery_estimator.estimatedBatteryRemaining(waypoints,
        wind_speeds,
        mission_airspeed,
        battery_initial_charge,
        constant_speed_power_consumption);
    EXPECT_NEAR(battery_remaining, 100.0, 1e-3);
}

//remaining batter should be same in case of 1 waypoint no matter what wind data
TEST(FlightBatteryEstimatorTest, TestEstimatedBatteryRemaining1Waypoint)
{
    FlightBatteryEstimator flight_battery_estimator{
        flight_battery_estimator::associateWindVectorsWithWaypointsUsingClosestWindVector
    };
    std::vector<FlightWaypoint> waypoints;
    std::vector<std::shared_ptr<WindData>> wind_speeds;


    addWaypoint(waypoints, 0, 10);
    addWindData(wind_speeds, 0, 0, -1000, 0);

    // Set up other parameters
    constexpr double mission_airspeed = 30.0; //m/s
    constexpr double battery_initial_charge = 100.0; //watt hours
    constexpr double constant_speed_power_consumption = 500.0; //Watt

    // Call the estimatedBatteryRemaining function
    const double battery_remaining = flight_battery_estimator.estimatedBatteryRemaining(waypoints,
        wind_speeds,
        mission_airspeed,
        battery_initial_charge,
        constant_speed_power_consumption);

    // Check that the battery remaining is the same as the initial charge
    EXPECT_NEAR(battery_remaining, 100.0, 1e-3);
}

//if wind data is empty then effect of the wind should not be considered
TEST(FlightBatteryEstimatorTest, TestEstimatedBatteryRemainingNoWindData)
{
    FlightBatteryEstimator flight_battery_estimator{
        flight_battery_estimator::associateWindVectorsWithWaypointsUsingClosestWindVector
    };
    std::vector<FlightWaypoint> waypoints;
    const std::vector<std::shared_ptr<WindData>> wind_speeds;

    addWaypoint(waypoints, 1000, 1000);
    addWaypoint(waypoints, 5000, 5000);

    // Set up other parameters
    constexpr double mission_airspeed = 30.0;
    constexpr double battery_initial_charge = 100.0;
    constexpr double constant_speed_power_consumption = 500.0;

    // Call the estimatedBatteryRemaining function
    const double battery_remaining = flight_battery_estimator.estimatedBatteryRemaining(waypoints,
        wind_speeds,
        mission_airspeed,
        battery_initial_charge,
        constant_speed_power_consumption);

    // Check that the battery remaining is the same as the initial charge
    EXPECT_NEAR(battery_remaining, 73.810, 1e-3);
}

TEST(FlightBatteryEstimatorTest, TestEstimatedBatteryRemainingCase1)
{
    FlightBatteryEstimator flight_battery_estimator{
        flight_battery_estimator::associateWindVectorsWithWaypointsUsingClosestWindVector
    };
    std::vector<FlightWaypoint> waypoints;
    std::vector<std::shared_ptr<WindData>> wind_speeds;


    addWaypoint(waypoints, 0, 1000);
    addWaypoint(waypoints, 0, 2000);

    addWindData(wind_speeds, 0, 0, 0, 0);


    constexpr double mission_airspeed = 30.0;
    constexpr double battery_initial_charge = 100.0;
    constexpr double constant_speed_power_consumption = 500.0;

    const double battery_remaining = flight_battery_estimator.estimatedBatteryRemaining(waypoints,
        wind_speeds,
        mission_airspeed,
        battery_initial_charge,
        constant_speed_power_consumption);
    EXPECT_NEAR(battery_remaining, 95.3704, 1e-3);
}

//create another test case
TEST(FlightBatteryEstimatorTest, TestEstimatedBatteryRemainingCase2)
{
    FlightBatteryEstimator flight_battery_estimator{
        flight_battery_estimator::associateWindVectorsWithWaypointsUsingClosestWindVector
    };

    std::vector<FlightWaypoint> waypoints;
    std::vector<std::shared_ptr<WindData>> wind_speeds;

    addWaypoint(waypoints, 0, 0);
    addWaypoint(waypoints, 1000, 1000);
    addWaypoint(waypoints, 2000, 2000);

    addWindData(wind_speeds, 250, 250, 10, 10);
    addWindData(wind_speeds, 1800, 1800, -5, -5);

    // Set up other parameters
    constexpr double mission_airspeed = 30.0;
    constexpr double battery_initial_charge = 100.0;
    constexpr double constant_speed_power_consumption = 500.0;

    // Call the estimatedBatteryRemaining function
    const double battery_remaining = flight_battery_estimator.estimatedBatteryRemaining(waypoints,
        wind_speeds,
        mission_airspeed,
        battery_initial_charge,
        constant_speed_power_consumption);


    EXPECT_NEAR(battery_remaining, 87.279, 1e-3);
}


TEST(FlightBatteryEstimatorTest, TestEstimatedBatteryRemainingCase3)
{
    FlightBatteryEstimator flight_battery_estimator{
        flight_battery_estimator::associateWindVectorsWithWaypointsUsingClosestWindVector
    };

    std::vector<FlightWaypoint> waypoints;
    std::vector<std::shared_ptr<WindData>> wind_speeds;

    addWaypoint(waypoints, 0, 1000);
    addWaypoint(waypoints, 0, 2000);
    addWaypoint(waypoints, 1500, 2000);
    addWaypoint(waypoints, 1500, 3000);

    addWindData(wind_speeds, 0, 1000, 5, 0);
    addWindData(wind_speeds, 1600, 1900, 0, 5);

    // Set up other parameters
    constexpr double mission_airspeed = 30.0;
    constexpr double battery_initial_charge = 100.0;
    constexpr double constant_speed_power_consumption = 500.0;

    // Call the estimatedBatteryRemaining function
    const double battery_remaining = flight_battery_estimator.estimatedBatteryRemaining(waypoints,
        wind_speeds,
        mission_airspeed,
        battery_initial_charge,
        constant_speed_power_consumption);


    EXPECT_NEAR(battery_remaining, 84.192, 1e-3);
}
