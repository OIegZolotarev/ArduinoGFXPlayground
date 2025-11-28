
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <iostream>
#include <cstring> // для memcpy

#include <asio.hpp>
#include <atomic>

class WiFiSerial
{

public:
    WiFiSerial(const std::string &host, uint16_t port);
    bool isConnected() const;
    ~WiFiSerial();

    void write(const std::vector<uint8_t> &data);
    bool hasData();

    std::vector<uint8_t> peek();

    std::vector<uint8_t> readBytes(size_t n);

    bool readUint16(uint16_t &value);
    bool readUint32(uint32_t &value);

    // Чтение null-terminated строки в буфер (как в WinAPI)
    bool readNullTerminatedString(char *buffer, size_t maxLen);
	
    void reconnect();

private:
    void connect();
	
	void remakeSocket();



    void start_read();
    void do_write();

    asio::io_context io_context_;
    asio::ip::tcp::socket socket_;
    std::thread io_thread_;
    std::string host_;
    uint16_t port_;

    std::array<uint8_t, 1024> read_buffer_;
    std::queue<std::vector<uint8_t>> read_queue_;
    std::mutex read_mutex_;

    std::atomic<bool> connected_{false};
    std::atomic<bool> connecting_{ false };

    std::queue<std::vector<uint8_t>> write_queue_;
    asio::executor_work_guard<asio::io_context::executor_type> work_guard_;

};
