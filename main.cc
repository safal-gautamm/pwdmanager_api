#include <drogon/drogon.h>
#include <iostream>

int main()
{
    // Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 5555);

    // Print nice startup message
    std::cout << "============================================\n";
    std::cout << "  Password Manager API is running!\n";
    std::cout << "  Access it at:  http://localhost:5555/\n";
    std::cout << "  Press Ctrl + C to stop the server.\n";
    std::cout << "============================================\n";

    // Run HTTP framework (blocks until exit)
    drogon::app().run();

    return 0;
}
