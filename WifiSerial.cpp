#include <asio.hpp>
#include "WifiSerial.h"

WiFiSerial::WiFiSerial(const std::string &host, uint16_t port)
    : io_context_(), socket_(io_context_), host_(host), port_(port)
{
    connect();
    io_thread_ = std::thread([this]()
                             { io_context_.run(); });
}

bool WiFiSerial::hasData()
{
    std::lock_guard<std::mutex> lock(read_mutex_);
    return !read_queue_.empty() && !read_queue_.front().empty();
}

bool WiFiSerial::readUint32(uint32_t &value)
{
    auto bytes = readBytes(4);
    if (bytes.size() < 4)
        return false;
    value = static_cast<uint32_t>(bytes[0] |
                                  (bytes[1] << 8) |
                                  (bytes[2] << 16) |
                                  (bytes[3] << 24));
    return true;
}

bool WiFiSerial::readUint16(uint16_t &value)
{
    auto bytes = readBytes(2);
    if (bytes.size() < 2)
        return false;
    value = static_cast<uint16_t>(bytes[0] | (bytes[1] << 8));
    return true;
}

std::vector<uint8_t> WiFiSerial::readBytes(size_t n)
{
    std::lock_guard<std::mutex> lock(read_mutex_);
    std::vector<uint8_t> result;
    if (read_queue_.empty())
        return result;

    auto &front = read_queue_.front();
    n = std::min(n, front.size());
    result.insert(result.end(), front.begin(), front.begin() + n);
    front.erase(front.begin(), front.begin() + n);
    if (front.empty())
        read_queue_.pop();
    return result;
}

std::vector<uint8_t> WiFiSerial::peek()
{
    std::lock_guard<std::mutex> lock(read_mutex_);
    if (read_queue_.empty())
        return {};
    return read_queue_.front();
}

void WiFiSerial::write(const std::vector<uint8_t> &data)
{
    asio::post(io_context_, [this, data]()
               {
        bool writing = !write_queue_.empty();
        write_queue_.push(data);
        if(!writing) do_write(); });
}

WiFiSerial::~WiFiSerial()
{
    io_context_.stop();
    if (io_thread_.joinable())
        io_thread_.join();
}

bool WiFiSerial::isConnected() const
{
    return connected_.load();
}

// Чтение null-terminated строки в буфер (как в WinAPI)
bool WiFiSerial::readNullTerminatedString(char *buffer, size_t maxLen)
{
    if (maxLen == 0)
        return false;

    std::lock_guard<std::mutex> lock(read_mutex_);
    if (read_queue_.empty())
        return false;

    auto &front = read_queue_.front();
    auto it = std::find(front.begin(), front.end(), 0); // ищем null-терминатор
    size_t copyLen = (it != front.end()) ? std::min<size_t>(it - front.begin(), maxLen - 1)
                                         : std::min(front.size(), maxLen - 1);

    std::memcpy(buffer, front.data(), copyLen);
    buffer[copyLen] = '\0';

    size_t eraseLen = (it != front.end()) ? copyLen + 1 : copyLen;
    front.erase(front.begin(), front.begin() + eraseLen);
    if (front.empty())
        read_queue_.pop();

    return true;
}

void WiFiSerial::connect()
{
    asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(host_, std::to_string(port_));

    asio::async_connect(socket_, endpoints,
                        [this](std::error_code ec, asio::ip::tcp::endpoint)
                        {
                            if (!ec)
                            {
                                connected_ = true;
                                start_read();
                            }
                            else
                            {
                                connected_ = false;
                                std::cerr << "Connect failed: " << ec.message() << "\n";
                            }
                        });
}

void WiFiSerial::start_read()
{
    socket_.async_read_some(asio::buffer(read_buffer_),
                            [this](std::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    {
                                        std::lock_guard<std::mutex> lock(read_mutex_);
                                        read_queue_.push(std::vector<uint8_t>(
                                            read_buffer_.data(),
                                            read_buffer_.data() + length));
                                    }
                                    start_read();
                                }
                                else
                                {
                                    connected_ = false;
                                    std::cerr << "Read error: " << ec.message() << "\n";
                                }
                            });
}

void WiFiSerial::do_write()
{
    asio::async_write(socket_, asio::buffer(write_queue_.front()),
                      [this](std::error_code ec, std::size_t /*length*/)
                      {
                          if (!ec)
                          {
                              write_queue_.pop();
                              if (!write_queue_.empty())
                                  do_write();
                          }
                          else
                          {
                              connected_ = false;
                              std::cerr << "Write error: " << ec.message() << "\n";
                          }
                      });
}