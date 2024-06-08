#pragma once
#include <cstdint>
#include <QByteArray>

namespace ar
{
  enum class MessageType : uint8_t
  {
    Diagnose,
    Data,
    Exit
  };

  struct Message
  {
    struct Header
    {
      MessageType type;
      uint32_t body_size;

      [[nodiscard]]
      QByteArray serialize() const noexcept
      {
        QByteArray result{};
        result.append(static_cast<char>(type));
        result.append(reinterpret_cast<const char*>(&body_size), sizeof(uint32_t));
        return result;
      }
    } header;

    QByteArray body;
  };

  template <typename T>
  static Message create_message(T&& payload) noexcept
  {
    auto serialized = payload.serialize();
    return Message{
      .header = {
        .type = T::type,
        .body_size = serialized.size()
      },
      .body = std::move(serialized)
    };
  }
}
