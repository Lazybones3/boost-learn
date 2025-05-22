#include "MyClient.h"

int main() {
    try {
        MyClient client("127.0.0.1", 10086);
        client.connect();
        for (int i = 0; i < 100; ++i) {
            std::string msg = "hello-" + std::to_string(i + 1);
            client.writeMessage(msg);
            const std::string &reply = client.readMessage();
            std::cout << "Client received: " << reply << std::endl;
        }
    } catch(std::exception & e){
        std::cerr << "Exception: " << e.what() <<  std::endl;
    }
    return 0;
}