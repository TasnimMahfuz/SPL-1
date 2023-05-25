#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>




#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <vector>



void error(const char *msg)
{
    perror(msg);
    exit(1);
}















// Structure for a node in the Huffman tree
struct HuffmanNode
{
    char data;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data) : data(data), left(nullptr), right(nullptr) {}
};

// Function to deserialize the Huffman codes from a file
std::unordered_map<std::string, char> deserializeHuffmanCodes(std::ifstream& inputFile)
{
    std::unordered_map<std::string, char> codes;
    size_t numCodes;
    inputFile.read(reinterpret_cast<char*>(&numCodes), sizeof(numCodes));

    for (size_t i = 0; i < numCodes; i++)
    {
        char character;
        size_t codeSize;
        inputFile.read(reinterpret_cast<char*>(&character), sizeof(character));
        inputFile.read(reinterpret_cast<char*>(&codeSize), sizeof(codeSize));
        std::string code(codeSize, '\0');
        inputFile.read(&code[0], codeSize);
        codes[code] = character;
    }

    return codes;
}

// Function to decompress the compressed text using Huffman codes and bitset
std::string decompressText(const std::vector<char>& compressedData, const std::unordered_map<std::string, char>& codes)
{
    std::string decompressedText;
    std::string currentCode;

    for (char byte : compressedData)
    {
        std::bitset<8> bitBuffer(byte);

        for (int bitIndex = 7; bitIndex >= 0; bitIndex--)
        {
            bool bit = bitBuffer[bitIndex];
            currentCode += (bit ? '1' : '0');

            if (codes.count(currentCode) > 0)
            {
                decompressedText += codes.at(currentCode);
                currentCode.clear();
            }
        }
    }

    return decompressedText;
}
















int main(int argc, char * argv[])
{
    int numberOfChats;

    std::string clientName, serverName, terminateChat;


    if(argc < 2)
    {
        fprintf(stderr,"port number is not provided\n");
        fprintf(stderr,"program terminated\n");
    }

    int sockfd, newsockfd, portno, n;
    char buffer[100000];

    struct sockaddr_in serv_addr, cli_addr;//internet add
    //netinet file e

    socklen_t clilen;//32 bit data type

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        error("Error opening the socket\n");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));//clearing the thing

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr =INADDR_ANY;
    serv_addr.sin_port= htons(portno);//host to network short
    //address niye nilam.

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Binding failed");
    }
    else
    {
        printf("Binding successful! Let's hear your name: ");




        std::getline(std::cin, serverName);


        std::ofstream clearFile("server name.txt", std::ios::trunc);
        clearFile.close();


        std::ofstream outputFile("server name.txt");


        if (outputFile.is_open())
        {

            outputFile << serverName;


            outputFile.close();

            std::cout << "Your name has been stored successfully. Chat can take place when a client joins!\n\n\n" << std::endl;
        }
        else
        {
            std::cout << "Unable to open the file.\n\n" << std::endl;
        }

    }


    listen(sockfd, 1000);

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)& cli_addr, &clilen);

    if(newsockfd < 0)
    {

        error("Error while executing accept.\n");
    }







    bool fileRead = false;



    while(1)
    {



        std::ifstream inputFile1("number of chats.txt");
        inputFile1 >> numberOfChats;
        inputFile1.close();


        std::string historyName = "chatHistory" + std::to_string(numberOfChats) + ".txt";



        if(!fileRead)
        {
            std::ifstream inputFile("client name.txt");



            if (inputFile.is_open())
            {

                std::getline(inputFile, clientName);


                inputFile.close();

                std::cout <<"Hello "<<serverName<< "! You will shortly receive texts from your friend! :):\n\n"<< std::endl;
                std::cout<<"What would you say to end the chat, "<<serverName<<"?"<<std::endl;

                std::getline(std::cin,terminateChat);


            }
            else
            {
                std::cout << "Unable to know your friend's name!" << std::endl;
            }
            fileRead = true;
        }

        bzero(buffer, sizeof( buffer));//changed a bit.

        n = read(newsockfd, buffer, sizeof(buffer));

        if(n < 0)
            error("Error on reading\n");













        std::ifstream compressedFile("compressed.txt", std::ios::binary);

        std::ifstream length("length.txt");
        int len;
        length >> len;
        length.close();

        std::ifstream codesFile("huffman_codes.txt", std::ios::binary);
        std::ofstream decompressedFile("decompressed.txt");

        if (!compressedFile)
        {
            std::cerr << "Error opening compressed file." << std::endl;
            return 1;
        }

        if (!codesFile)
        {
            std::cerr << "Error opening Huffman codes file." << std::endl;
            return 1;
        }

        if (!decompressedFile)
        {
            std::cerr << "Error creating decompressed file." << std::endl;
            return 1;
        }

        // Read the compressed data from the file
        std::vector<char> compressedData(std::istreambuf_iterator<char>(compressedFile), {});
        compressedFile.close();

        // Deserialize the Huffman codes from the codes file
        std::unordered_map<std::string, char> codes = deserializeHuffmanCodes(codesFile);
        codesFile.close();

        // Decompress the compressed data using Huffman codes
        std::string decompressedText = decompressText(compressedData, codes);

        // Write the decompressed text to the decompressed file
        decompressedFile << decompressedText;
        decompressedFile.close();

        std::cout << "[ Decompression completed. ]" << std::endl;




        decompressedText[len] = '\0';

        char decompressedText2[len];

        for(int i = 0; i < len; i++)
            decompressedText2[i] = decompressedText[i];

        decompressedText2[len] = '\0';













        //printf("Client: %s\n", buffer);
        //std::cout<< clientName << ": "<<decompressedText<<"\n";
        std::cout<< clientName << ": "<<decompressedText2<<"\n";





        std::ofstream historyFile(historyName, std::ios_base::app);

    if (historyFile.is_open()) {
        // Write the desired output to the file
        historyFile << clientName << ": " << decompressedText << "\n";

        // Close the file after writing
        historyFile.close();
    } else {
        std::cout << "Error opening the file!" << std::endl;
    }








        bzero(buffer,sizeof(buffer));//buffer clearing

        fgets(buffer, sizeof(buffer), stdin);

        n = write(newsockfd,buffer, strlen(buffer));

        if(n < 0)
            error("Error on Writing.");


        char terChat[terminateChat.size() + 1];

        terChat[terminateChat.size()] = '\0';


        for(int j = 0; j < terminateChat.size(); j++)
        {
            terChat[j] = terminateChat[j];
        }

        //int i = strncmp("Bye", buffer, 3);
        int i = strncmp(terChat, buffer, terminateChat.size() );

        if(i == 0)
            break;
    }
    close(newsockfd);
    close(sockfd);

    return 0;
}



