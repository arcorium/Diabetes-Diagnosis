#pragma once

#include <cmath>
#include <span>

namespace ar
{
  /**
   * calculate the bmi
   * @param weight user weight on kg
   * @param heigth user weight on meter
   * @return body mass index result
   */
  static constexpr double calculate_bmi(float weight, float heigth) noexcept
  {
    return static_cast<double>(weight) / std::pow(heigth, 2.f);
  }

  template <typename T>
  struct ValueWeight
  {
    T value;
    double weight;
  };

  static constexpr double calculate_dpf(std::span<ValueWeight<uint8_t>> value_weights) noexcept
  {
    double result = 0.0;

    for (const auto& [val, weight] : value_weights)
      result += val * weight;

    return result;
  }
}
