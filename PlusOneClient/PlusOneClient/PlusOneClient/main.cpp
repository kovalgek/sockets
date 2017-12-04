//
//  main.cpp
//  PlusOneClient
//
//  Created by Anton Kovalchuk on 04.12.17.
//  Copyright © 2017 Anton Kovalchuk. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include "PracticalSocket.h"

using namespace std;

int main(int argc, const char * argv[])
{
    
    if (argc != 3) // Check number of parameters
    {
        cerr << "Usage: PlusOneClient <server host> <starting value>" << endl;
        return 1;
    }
    
    try
    {
        TCPSocket socket(argv[1], 9431);
        
        uint32_t val = atoi(argv[2]);
        val = htonl(val);
        socket.send(&val, sizeof(val));
        
        if (socket.recvFully(&val, sizeof(val)) == sizeof(val))
        {
            val = ntohl(val);
            cout << "Server Response: " << val << endl;
        }
    }
    catch (SocketException &e)
    {
        cerr << e.what() << endl;
    }
    
    return 0;
}
