#pragma once
#include <adkostatt/Endianness.hpp>
#include <cstdint>
#include <cstring>
namespace adkostatt {
namespace Proxy {
static constexpr size_t IPONLY = 0;
template <typename Parent, size_t DSize = IPONLY> class Socks4 {
private:
  static constexpr Endianness::BE<uint16_t> VERCMD{0b0000010000000001};
  char handshakei[8];
  char handshakeo[9 + DSize];
  Parent parent;

public:
  inline int Write(char *buffer, int size) {
    return this->parent.Write(buffer, size);
  }
  inline int Read(char *buffer, int size) {
    return this->parent.Read(buffer, size);
  }
  inline bool Open() {
    if (this->parent.Write(handshakeo, sizeof(handshakeo)) <= 0) [[unlikely]]
      return false;
    // Probably better change <= 0 to != 8. But server may send only first 2
    // bytes
    if (this->parent.Read(handshakei, 8) <= 0) [[unlikely]]
      return false;
    return handshakei[1] == 0x5a;
  }
  inline bool Close() { return this->parent.Close(); }
  Socks4(Parent parent, Endianness::BE<uint32_t> targetIp,
         Endianness::BE<uint16_t> targetPort)
      : parent(parent) {
    *reinterpret_cast<uint16_t *>(handshakeo) = VERCMD;
    *reinterpret_cast<uint16_t *>(handshakeo + 2) = targetPort;
    *reinterpret_cast<uint32_t *>(handshakeo + 4) = targetIp;
  }
  Socks4(Parent parent, const char (&domain)[DSize],
         Endianness::BE<uint16_t> targetPort)
    requires(DSize > 0)
      : Socks4(parent, 1u, targetPort) {
    handshakeo[8] = 0;
    std::memcpy(handshakeo + 9, domain, DSize);
  }
};
template <typename Parent, size_t N>
Socks4(Parent, const char (&)[N], Endianness::BE<uint16_t>)
    -> Socks4<Parent, N>;
} // namespace Proxy
} // namespace adkostatt
