#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>




#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <bitset>








// Structure for a node in the Huffman tree
struct HuffmanNode
{
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int frequency)
        : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Comparison function for HuffmanNodes (used by priority_queue)
struct CompareNodes
{
    bool operator()(const HuffmanNode* node1, const HuffmanNode* node2)
    {
        return node1->frequency > node2->frequency;
    }
};

// Function to build the Huffman tree
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& frequencyTable)
{
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;

    // Create a leaf node for each character with its frequency
    for (const auto& entry : frequencyTable)
    {
        char character = entry.first;
        int frequency = entry.second;
        pq.push(new HuffmanNode(character, frequency));
    }

    // Build the tree by merging nodes
    while (pq.size() > 1)
    {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        int sumFrequency = left->frequency + right->frequency;
        HuffmanNode* parent = new HuffmanNode('$', sumFrequency);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    return pq.top();
}

// Function to generate Huffman codes
void generateHuffmanCodes(HuffmanNode* root, const std::string& code, std::unordered_map<char, std::string>& codes)
{
    if (root == nullptr)
    {
        return;
    }

    if (root->data != '$')
    {
        codes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", codes);
    generateHuffmanCodes(root->right, code + "1", codes);
}

// Function to compress the input text using Huffman codes and bitset
std::string compressText(const std::string& inputText, const std::unordered_map<char, std::string>& codes)
{
    std::string compressedText;
    std::bitset<8> bitBuffer;
    size_t bitCount = 0;

    for (char c : inputText)
    {
        std::string code = codes.at(c);

        for (char bit : code)
        {
            if (bit == '1')
            {
                bitBuffer.set(7 - bitCount);
            }

            bitCount++;

            if (bitCount == 8)
            {
                compressedText += static_cast<char>(bitBuffer.to_ulong());
                bitBuffer.reset();
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0)
    {
        bitBuffer <<= (8 - bitCount);
        compressedText += static_cast<char>(bitBuffer.to_ulong());
    }

    return compressedText;
}

// Function to serialize the Huffman codes to a file
void serializeHuffmanCodes(const std::unordered_map<char, std::string>& codes, std::ofstream& outputFile)
{
    size_t numCodes = codes.size();
    outputFile.write(reinterpret_cast<char*>(&numCodes), sizeof(numCodes));

    for (const auto& entry : codes)
    {
        char character = entry.first;
        const std::string& code = entry.second;

        outputFile.write(reinterpret_cast<const char*>(&character), sizeof(character));
        size_t codeSize = code.size();
        outputFile.write(reinterpret_cast<const char*>(&codeSize), sizeof(codeSize));
        outputFile.write(code.c_str(), codeSize);
    }
}

// Function to calculate the compression ratio
double calculateCompressionRatio(const std::string& compressedText, const std::string& inputText)
{
    double inputSize = inputText.size() * 8;      // Size of input text in bits
    double compressedSize = compressedText.size() * 8;  // Size of compressed text in bits

    return (compressedSize / inputSize) * 100.0;
}

















void error(const char * msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char * argv[])
{




    int numberOfChats;
    std::ifstream inputFile1("number of chats.txt");
    inputFile1 >> numberOfChats;
    inputFile1.close();

    // Increment the number of chats
    numberOfChats++;

    // Clear the file and store the updated value
    std::ofstream outputFile1("number of chats.txt");
    outputFile1 << numberOfChats;
    outputFile1.close();



    std::string historyName = "chatHistory" + std::to_string(numberOfChats) + ".txt";

    std::ofstream inputFile2(historyName);
    inputFile2 << "Start of the chat\n\n";
    inputFile2.close();



    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[100000], buffer2[100000];
    std::string serverName;
    std::string clientName;

    if(argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi (argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        error("Error opening socket");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL)
    {
        fprintf(stderr,"Error, no such host");
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server -> h_addr, (char *)&serv_addr.sin_addr.s_addr, server -> h_length);

    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Socket connection failed.\n");
    else
    {
        printf("Socket connection has been established! What is your name?: \n");




        std::getline(std::cin, clientName);

        std::ofstream clearFile("client name.txt", std::ios::trunc);
        clearFile.close();


        std::ofstream outputFile("client name.txt");


        if (outputFile.is_open())
        {

            outputFile << clientName;


            outputFile.close();

            std::cout << "Your name has been stored successfully. Chat can take place now!\n\n\n" << std::endl;
        }
        else
        {
            std::cout << "Unable to open the file.\n\n" << std::endl;
        }



    }


    bool fileRead = false;


    while(1)
    {

        if(!fileRead)
        {
            std::ifstream inputFile("server name.txt");



            if (inputFile.is_open())
            {

                std::getline(inputFile, serverName);


                inputFile.close();

                std::cout <<"Hello "<<clientName<< "! You can now chat with your friend " << serverName <<"! :):\n\n"<< std::endl;
            }
            else
            {
                std::cout << "Unable to know your friend's name!" << std::endl;
            }
            fileRead = true;
        }


        bzero(buffer, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);





        std::ofstream outputFile("input.txt"); // Open the output file

        if (!outputFile)
        {
            std::cerr << "Error opening the file." << std::endl;
            return 1;
        }




        outputFile << buffer; // Write input to the file

        outputFile.close();












        std::ifstream inputFile("input.txt");


        std::ofstream length("length.txt");


        std::ofstream compressedFile("compressed.txt");
        std::ofstream codesFile("huffman_codes.txt");

        if (!inputFile)
        {
            std::cerr << "Error opening input file." << std::endl;
            return 1;
        }

        if (!compressedFile)
        {
            std::cerr << "Error creating compressed file." << std::endl;
            return 1;
        }

        if (!codesFile)
        {
            std::cerr << "Error creating Huffman codes file." << std::endl;
            return 1;
        }

        // Read the input text from the file
        std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();





        length << (int)inputText.size();
        length.close();


        // Count the frequency of each character in the input text
        std::unordered_map<char, int> frequencyTable;
        for (char c : inputText)
        {
            frequencyTable[c]++;
        }

        // Build the Huffman tree
        HuffmanNode* root = buildHuffmanTree(frequencyTable);

        // Generate Huffman codes
        std::unordered_map<char, std::string> codes;
        generateHuffmanCodes(root, "", codes);

        // Serialize the Huffman codes to the codes file
        serializeHuffmanCodes(codes, codesFile);
        codesFile.close();

        // Compress the input text using Huffman codes and bitset
        std::string compressedText = compressText(inputText, codes);

        // Write the compressed text to the compressed file
        compressedFile << compressedText;
        compressedFile.close();

        // Calculate the compression ratio
        double compressionRatio = calculateCompressionRatio(compressedText, inputText);

        std::cout << "\n\nCompression completed." << std::endl;
        std::cout << "Compression ratio: " << compressionRatio << "% \n\n" << std::endl;

        // Cleanup: Delete the Huffman tree nodes
        delete root;

        for(int i = 0; i < compressedText.size(); i++)
            buffer2[i] = compressedText[i];

        buffer2[compressedText.size()] = '\0';













        n = write (sockfd, buffer2, strlen(buffer2));

        if(n < 0)
        {
            error("Error writing the file.\n");
        }

        bzero(buffer, sizeof(buffer));
        bzero(buffer2, sizeof(buffer2));

        n = read(sockfd, buffer, sizeof(buffer));

        if(n < 0)
            error("Error on reading.\n");

        //printf("Server: %s", buffer);
        std::cout<< serverName;
        printf(": %s", buffer);





        std::ofstream historyFile(historyName, std::ios_base::app);

    if (historyFile.is_open()) {
        // Write the desired output to the file
        historyFile << serverName << ": " << buffer << "\n";

        // Close the file after writing
        historyFile.close();
    } else {
        std::cout << "Error opening the file!" << std::endl;
    }




        int i = strncmp("Bye", buffer, 3);

        if(i == 0)
            break;

    }

    close(sockfd);
    return 0;
}

