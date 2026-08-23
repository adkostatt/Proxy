#include <adkostatt/Http.hpp>
#include <adkostatt/HttpWrapper.hpp>

namespace adkostatt {
namespace Proxy {
template <typename Parent, size_t Size = 62> class HttpTunnel {
private:
  char handshakeo[Size];
  char handshakei[Size];
  int hoSize;
  Parent parent;

public:
  inline int Write(char *buffer, size_t size) {
    return this->parent.Write(buffer, size);
  }
  inline int Read(char *buffer, size_t size) {
    return this->parent.Read(buffer, size);
  }
  inline bool Open() {
    if (!this->parent.Open()) [[unlikely]]
      return false;
    if (this->parent.Write(handshakeo, hoSize) <= 0) [[unlikely]]
      return false;
    int read = this->parent.Read(handshakei, sizeof(handshakei));
    if (read <= 0) [[unlikely]]
      return false;
    Http::HttpVersion version;
    int32_t status;
    Http::ParseResponseStart(handshakei, handshakei + read, &version, &status);
    return Http::SkipHeaders(handshakei, handshakei + read) &&
           status == Http::StatusCode::Ok;
  }
  inline bool Close() { return this->parent.Close(); }
  // example.com:443 15
  HttpTunnel(Parent parent, const char *target, const int size)
      : parent(parent) {
    Http::Generator generator(handshakeo, sizeof(handshakeo));
    generator.GenerateRequest(
        {target, size, Http::MethodType::Connect, Http::HttpVersion::v1});
    generator.GenerateHeader({"Host", target, 4, size});
    hoSize = generator.Finalise() - handshakeo;
    if (hoSize > sizeof(handshakeo)) [[unlikely]]
      hoSize = 0;
  }
};
} // namespace Proxy
} // namespace adkostatt
