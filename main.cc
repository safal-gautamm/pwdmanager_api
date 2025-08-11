#include <drogon/drogon.h>
#include <iostream>

int main()
{
    // Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 5555);

    // Print StartUp message
    std::cout << "|-----------------------------------------------|" << std::endl;
    std::cout << "|               API is executed                 |" << std::endl;
    std::cout << "|            http://localhost:5555/             |" << std::endl;
    std::cout << "|      Press Ctrl + C to stop the server.       |" << std::endl;
    std::cout << "|-----------------------------------------------|" << std::endl;

    drogon::app().run();

    return 0;
}
