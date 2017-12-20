//
//  main.cpp
//  SurveyClient2
//
//  Created by Anton Kovalchuk on 20.12.17.
//  Copyright © 2017 Anton Kovalchuk. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "PracticalSocket.h"
#include "SurveyCommon.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2) // Make sure the user gives a host
    {
        std::cerr << "Usage: SurveyClient <Survey Server Host>" << std::endl;
        return 1;
    }
    
    try
    {
        // Connect to the server.
        TCPSocket sock(argv[1], SURVEY_PORT);
        std::iostream &stream = sock.getStream();
        std::vector<Question> qList; // Read the whole survey
        readSurvey(stream, qList);
        
        for (unsigned int q = 0; q < qList.size(); q++)
        {
            // Show each the question to the user and print the list of responses.
            std::cout << "Q" << q << ": " << qList[q].qText << std::endl;
            for (unsigned int r = 0; r < qList[q].rList.size(); r++)
            {
                std::cout << std::setw(2) << r << " " << qList[q].rList[r] << std::endl;
            }
            
            // Keep prompting the user until we get a legal response.
            size_t response = qList[q].rList.size();
            while (response >= qList[q].rList.size())
            {
                std::cout << "> ";
                std::cin >> response;
            }
            
            stream << response << std::endl; // Send user response to server
            stream.flush();
        }
    }
    catch(SocketException &e)
    {
        std::cerr << e.what() << std::endl; // Report errors to the console.
        return 1;
    }
    
    return 0;
}
