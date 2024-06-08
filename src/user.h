#pragma once

namespace ar
{
  enum class GenderType
  {
    Male,
    Female
  };

  static constexpr std::string_view GENDER_MALE_STR = "Male";
  static constexpr std::string_view GENDER_FEMALE_STR = "Female";

  constexpr std::string_view gender_str(GenderType type) noexcept
  {
    switch (type)
    {
    case GenderType::Male:
      return GENDER_MALE_STR;
    case GenderType::Female:
      return GENDER_FEMALE_STR;
    }
    return "Not found";
  }

  struct UserDiagnosed
  {
    std::string name;
    std::string address;
    GenderType gender;
    uint8_t age;
    uint8_t pregnancies;
    uint8_t blood_pressure;
    uint8_t skin_thickness;
    uint8_t insulin;
    uint16_t glucose;
    float bmi;
    float dpf;
    bool result;
  };
}
