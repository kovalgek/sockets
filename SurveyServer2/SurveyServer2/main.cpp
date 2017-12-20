//
//  main.cpp
//  SurveyServer2
//
//  Created by Anton Kovalchuk on 20.12.17.
//  Copyright © 2017 Anton Kovalchuk. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <pthread.h>
#include "PracticalSocket.h"
#include "SurveyCommon.hpp"

static std::vector<Question> qList; // List of survey questions
static pthread_mutex_t lock; // Mutex to Protect critical sections
static std::vector<std::vector<int> > rCount; // Response tallies for each question
static std::vector<SocketAddress> addrList; // Address list for client history

/** Thread main function to administer a survey over the given socket */
void *conductSurvey(void *arg);

/** Thread main function to monitor an administrative connection and
 give reports over it. */
void *adminServer(void *arg);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: SurveyServer <Survey File>" << std::endl;
        return 1;
    }
    
    std::ifstream input(argv[1]); // Read survey from given file
    if (!input || !readSurvey(input, qList))
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
    
    try
    {
        pthread_t newThread;
        
        // Make a thread to provide the administrative interface.
        if (pthread_create(&newThread, NULL, adminServer, NULL) != 0)
        {
            std::cerr << "Can't create administrative thread" << std::endl;
            return 1;
        }
        
        // Make a socket to listen for SurveyClient connections.
        TCPServerSocket servSock(SURVEY_PORT);
        
        for (;;) // Repeatedly accept connections and administer the survey.
        {
            TCPSocket *sock = servSock.accept();
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

void *conductSurvey(void *arg)
{
    TCPSocket *sock = (TCPSocket *)arg; // Argument is really a socket.
    
    try
    {
        // Write out the survey in the same format as the input file.
        std::iostream &stream = sock->getStream();
        stream << qList.size() << "\n";
        
        for (unsigned int q = 0; q < qList.size(); q++)
        {
            stream << qList[q].qText << "\n";
            stream << qList[q].rList.size() << "\n";
            for (unsigned int r = 0; r < qList[q].rList.size(); r++)
            {
                stream << qList[q].rList[r] << "\n";
            }
        }
        stream.flush();
        
        // Read client responses to questions and record them
        for (unsigned int q = 0; q < qList.size(); q++)
        {
            unsigned int response;
            stream >> response;
            if (response >= 0 && response < rCount[q].size())
            {
                pthread_mutex_lock(&lock); // Lock the mutex
                rCount[q][response]++; // Increment count for chosen item
                pthread_mutex_unlock(&lock); // Release the lock
            }
        }
        
        // Log this client as completing the survey.
        pthread_mutex_lock(&lock);
        addrList.push_back(sock->getForeignAddress());
        pthread_mutex_unlock(&lock);
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl; // Report errors to the console.
    }
    
    delete sock; // Free the socket object (and close the connection)
    return NULL;
}

void *adminServer(void *arg)
{
    try
    {
        // Make a ServerSocket to listen for admin connections
        TCPServerSocket adminSock;
        adminSock.bind(SocketAddress("127.0.0.1", SURVEY_PORT + 1));
        
        for (;;) // Repeatedly accept administrative connections
        {
            TCPSocket *sock = adminSock.accept();
            std::iostream &stream = sock->getStream();
            
            try
            {
                // Copy response counts and address lists
                pthread_mutex_lock(&lock);
                std::vector<std::vector<int>> myCount = rCount;
                std::vector<SocketAddress> myList = addrList;
                pthread_mutex_unlock(&lock);
                
                for (unsigned int q = 0; q < qList.size(); q++)
                {
                    // Give a report for each question.
                    stream << "Q" << q << ": " << qList[q].qText << "\n";
                    for (unsigned int r = 0; r < qList[q].rList.size(); r++)
                    {
                        stream << std::setw(5) << myCount[q][r] << " " << qList[q].rList[r] << "\n";
                    }
                }
                
                // Report the list of client addresses.
                stream << "Client Addresses:" << std::endl;
                for (unsigned int c = 0; c < myList.size(); c++ )
                {
                    stream << " " << myList[c].getAddress() << "\n";
                }
                
                stream.flush();
            }
            catch (std::runtime_error e)
            {
                std::cerr << e.what() << std::endl;
            }
            
            delete sock; // Free the socket object (and close the connection)
        }
    }
    catch (SocketException e)
    {
        std::cerr << e.what() << std::endl; // Report errors to the console.
    }
    return NULL; // Reached only on error
}
