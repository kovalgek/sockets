//
//  main.cpp
//  SurveyServer
//
//  Created by Anton Kovalchuk on 10.12.17.
//  Copyright © 2017 Anton Kovalchuk. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <pthread.h>
#include "PracticalSocket.h"
#include "SurveyCommon.hpp"

static std::vector<Question> qList; // List of survey questions
static pthread_mutex_t lock; // Mutex to Protect critical sections
static std::vector<std::vector<int> > rCount; // Response tallies for each question

/** Thread main function to administer a survey over the given socket */
static void *conductSurvey(void *arg);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: SurveyServer <Survey File>" << std::endl;
        return 1;
    }
    
    std::ifstream input(argv[1]); // Read survey from given file
    if (!input || !readSurvey(input, qList) )
    {
        std::cerr << "Can't read survey from file: " << argv[1] << std::endl;
        return 1;
    }
    
    // Initialize response tally for each question/response.
    for (unsigned int i = 0; i < qList.size(); i++)
    {
        rCount.push_back(std::vector<int>(qList[i].rList.size(), 0));
    }
    pthread_mutex_init(&lock, NULL); // Initialize mutex
    
    try {
        // Make a socket to listen for SurveyClient connections.
        TCPServerSocket servSock(SURVEY_PORT);
        
        for (;;) // Repeatedly accept connections and administer the survey.
        {
            TCPSocket *sock = servSock.accept();
            
            pthread_t newThread; // Give survey in a separate thread
            if (pthread_create(&newThread, NULL, conductSurvey, sock) != 0)
            {
                std::cerr << "Can't create new thread" << std::endl;
                delete sock;
            }
        }
    }
    catch (SocketException &e)
    {
        std::cerr << e.what() << std::endl; // Report errors to the console.
    }
    
    return 0;
}


static void *conductSurvey(void *arg)
{
    TCPSocket *sock = (TCPSocket *)arg; // Argument is really a socket
    try
    {
        sendInt(sock, (uint32_t)qList.size()); // Tell client no of questions
        
        for (unsigned int q = 0; q < qList.size(); q++)
        {
            // For each question, send the question text and list of responses
            sendString(sock, qList[q].qText);
            sendInt(sock, (uint32_t)qList[q].rList.size());
            for (unsigned int r = 0; r < qList[q].rList.size(); r++)
            {
                sendString(sock, qList[q].rList[r]);
            }
            
            // Get the client's response and count it if it's in range
            unsigned int response = recvInt(sock);
            if (response < rCount[q].size())
            {
                pthread_mutex_lock(&lock); // Lock the mutex
                rCount[q][response]++; // Increment count for chosen item
                pthread_mutex_unlock(&lock); // Release the lock
            }
        }
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl; // Report errors to the console.
    }
    
    delete sock; // Free the socket object (and close the connection)
    return NULL;
}
