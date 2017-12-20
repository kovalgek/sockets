//
//  main.cpp
//  AdminClient2
//
//  Created by Anton Kovalchuk on 20.12.17.
//  Copyright © 2017 Anton Kovalchuk. All rights reserved.
//

#include <iostream>
#include "PracticalSocket.h"
#include "SurveyCommon.hpp"

int main(int argc, char *argv[])
{
    try
    {
        // Connect to the server's administrative interface.
        TCPSocket sock("localhost", SURVEY_PORT + 1);
        
        // Read the server's report a block at a time.
        char buffer[1025];
        size_t len;
        while ((len = sock.recv(buffer, sizeof(buffer) - 1)) != 0)
        {
            buffer[len] = '\0'; // Null terminate the sequence
            std::cout << buffer; // And print it like as a string
        }
    }
    catch(SocketException &e)
    {
        std::cerr << e.what() << std::endl; // Report errors to the console.
        exit(1);
    }
    
    return 0;
}
