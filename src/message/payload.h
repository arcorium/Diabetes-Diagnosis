#pragma once
#include <cstdint>
#include <expected>
#include <QByteArray>

#include "protocol.h"

namespace ar
{
  struct DiagnosePayload
  {
    static constexpr MessageType type = MessageType::Diagnose;

    uint8_t age;
    uint8_t pregnancies;
    uint8_t blood_pressure;
    uint8_t skin_thickness;
    uint8_t insulin;
    uint16_t glucose;
    float bmi;
    float dpf;

    [[nodiscard]]
    QByteArray serialize() noexcept
    {
      QByteArray result{};
      result.append(pregnancies);
      result.append(reinterpret_cast<const char*>(&glucose), sizeof(uint16_t));
      result.append(blood_pressure);
      result.append(skin_thickness);
      result.append(insulin);
      result.append(reinterpret_cast<const char*>(&bmi), sizeof(float));
      result.append(reinterpret_cast<const char*>(&dpf), sizeof(float));
      result.append(age);
      return result;
    }
  };

  struct DiagnoseResponse
  {
    static constexpr size_t SIZE = sizeof(bool) + sizeof(uint32_t) + sizeof(float);
    bool predict;
    uint32_t index;
    float similarity;

    static std::expected<DiagnoseResponse, std::string_view> deserialize(QByteArray payload) noexcept
    {
      if (payload.size() < SIZE)
        return std::unexpected{"size of payload doesn't match"};

      bool predict = payload[0];
      auto index = *reinterpret_cast<uint32_t*>(&payload[1]);
      auto similarity = *reinterpret_cast<float*>(&payload[sizeof(uint32_t) + 1]);

      return DiagnoseResponse{
        .predict = predict,
        .index = index,
        .similarity = similarity
      };
    }
  };

  struct DataPayload
  {
    static constexpr MessageType type = MessageType::Data;

    [[nodiscard]]
    QByteArray serialize() noexcept
    {
      return QByteArray{};
    }
  };

  struct DataResponse
  {
    struct Data
    {
      uint8_t pregnancies;
      uint8_t blood_pressure;
      uint8_t skin_thickness;
      uint8_t age;
      bool result;
      float bmi;
      float dpf;
      uint16_t insulin;
      uint16_t glucose;
    };

    std::vector<Data> data;

    static DataResponse deserialize(QByteArray payload) noexcept
    {
      static constexpr uint8_t DATA_SIZE = 17;
      std::vector<Data> result{};

      auto block = payload.size() / DATA_SIZE;
      for (qsizetype i = 0; i < block; ++i)
      {
        auto slice = payload.sliced(i * DATA_SIZE, DATA_SIZE);

        auto pregnant = static_cast<uint8_t>(slice[0]);
        auto glucose = *reinterpret_cast<uint16_t*>(&slice[1]);
        auto blood = static_cast<uint8_t>(slice[3]);
        auto skin = static_cast<uint8_t>(slice[4]);
        auto insulin = *reinterpret_cast<uint16_t*>(&slice[5]);
        auto bmi = *reinterpret_cast<float*>(&slice[7]);
        auto dpf = *reinterpret_cast<float*>(&slice[11]);
        auto age = static_cast<uint8_t>(slice[15]);
        auto is_diseases = static_cast<bool>(slice[16]);

        Data data{
          .pregnancies = pregnant,
          .blood_pressure = blood,
          .skin_thickness = skin,
          .age = age,
          .result = is_diseases,
          .bmi = bmi,
          .dpf = dpf,
          .insulin = insulin,
          .glucose = glucose
        };
        result.push_back(std::move(data));
      }
      return DataResponse{.data = std::move(result)};
    }
  };

  struct ExitPayload
  {
    static constexpr MessageType type = MessageType::Exit;

    [[nodiscard]]
    QByteArray serialize() noexcept
    {
      return QByteArray{};
    }
  };
}
