#ifndef TCP_CLIENT
#define TCP_CLIENT
#include<boost/asio.hpp>
#include<queue>

namespace MOVF{
    using MessageHandler = std::function<void(std::string)>;

    class TcpCLient{
    public :
        TcpCLient(const std::string &address, int port );
        void Run();
        void Stop();
        void Post(const std::string &message);
    private:
        void asyncRead();
        void onRead(boost::system::error_code er,size_t bytes_transfered);
        void asyncWrite();
        void onWrite(boost::system::error_code er,size_t bytes_transfered);
    public :
        MessageHandler OnMessage;
    private:
        boost::asio::io_context ioContext {};
        boost::asio::ip::tcp::socket socket;

        boost::asio::ip::tcp::resolver::results_type endpoints;
        boost::asio::streambuf streambuf{65536};
        std::queue<std::string> outgoingMessage {};
    };
}
#endif

