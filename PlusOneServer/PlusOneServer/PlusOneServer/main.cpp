//
//  main.cpp
//  PlusOneServer
//
//  Created by Anton Kovalchuk on 04.12.17.
//  Copyright © 2017 Anton Kovalchuk. All rights reserved.
//

#include <iostream>
#include "PracticalSocket.h"

using namespace std;

int main(int argc, const char * argv[])
{
    try {
        
        TCPServerSocket serverSocket(9431);
        
        for(;;)
        {
            TCPSocket *socket = serverSocket.accept();
            uint32_t val;
            if (socket->recvFully(&val, sizeof(val)) == sizeof(val))
            {
                val = ntohl(val);
                ++val;
                val = htonl(val);
                socket->send(&val, sizeof(val));
            }
            delete socket;
        }
    }
    catch (SocketException &e)
    {
        cerr << e.what() << endl;
    }
    return 0;
}
