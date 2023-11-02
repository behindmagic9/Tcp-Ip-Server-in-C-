#include <iostream>
#include<thread>
#include <Network/Client/Tcp_Client.h>

using namespace MOVF;

int main(int argc , char* argv[]) {
    TcpCLient client {"locahost",1337};
    client.OnMessage = [](const std::string &message){
        std::cout << message;
    };
    std::thread t1{[&client](){
        client.Run();
    }};
    while(true){
        std::string message;
        getline(std::cin,message);
        if(message == "\\q"){
            break;
        }
        message += "\n";
        client.Post(message);

    }
    client.Stop();
    t1.join();
    return 0;
}