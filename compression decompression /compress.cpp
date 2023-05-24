#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <bitset>

// Structure for a node in the Huffman tree
struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int frequency)
        : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Comparison function for HuffmanNodes (used by priority_queue)
struct CompareNodes {
    bool operator()(const HuffmanNode* node1, const HuffmanNode* node2) {
        return node1->frequency > node2->frequency;
    }
};

// Function to build the Huffman tree
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& frequencyTable) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;

    // Create a leaf node for each character with its frequency
    for (const auto& entry : frequencyTable) {
        char character = entry.first;
        int frequency = entry.second;
        pq.push(new HuffmanNode(character, frequency));
    }

    // Build the tree by merging nodes
    while (pq.size() > 1) {
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
void generateHuffmanCodes(HuffmanNode* root, const std::string& code, std::unordered_map<char, std::string>& codes) {
    if (root == nullptr) {
        return;
    }

    if (root->data != '$') {
        codes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", codes);
    generateHuffmanCodes(root->right, code + "1", codes);
}

// Function to compress the input text using Huffman codes and bitset
std::string compressText(const std::string& inputText, const std::unordered_map<char, std::string>& codes) {
    std::string compressedText;
    std::bitset<8> bitBuffer;
    size_t bitCount = 0;

    for (char c : inputText) {
        std::string code = codes.at(c);

        for (char bit : code) {
            if (bit == '1') {
                bitBuffer.set(7 - bitCount);
            }

            bitCount++;

            if (bitCount == 8) {
                compressedText += static_cast<char>(bitBuffer.to_ulong());
                bitBuffer.reset();
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount);
        compressedText += static_cast<char>(bitBuffer.to_ulong());
    }

    return compressedText;
}

// Function to serialize the Huffman codes to a file
void serializeHuffmanCodes(const std::unordered_map<char, std::string>& codes, std::ofstream& outputFile) {
    size_t numCodes = codes.size();
    outputFile.write(reinterpret_cast<char*>(&numCodes), sizeof(numCodes));

    for (const auto& entry : codes) {
        char character = entry.first;
        const std::string& code = entry.second;

        outputFile.write(reinterpret_cast<const char*>(&character), sizeof(character));
        size_t codeSize = code.size();
        outputFile.write(reinterpret_cast<const char*>(&codeSize), sizeof(codeSize));
        outputFile.write(code.c_str(), codeSize);
    }
}

// Function to calculate the compression ratio
double calculateCompressionRatio(const std::string& compressedText, const std::string& inputText) {
    double inputSize = inputText.size() * 8;      // Size of input text in bits
    double compressedSize = compressedText.size() * 8;  // Size of compressed text in bits

    return (compressedSize / inputSize) * 100.0;
}



int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream compressedFile("compressed.txt");
    std::ofstream codesFile("huffman_codes.txt");

    if (!inputFile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    if (!compressedFile) {
        std::cerr << "Error creating compressed file." << std::endl;
        return 1;
    }

    if (!codesFile) {
        std::cerr << "Error creating Huffman codes file." << std::endl;
        return 1;
    }

    // Read the input text from the file
    std::string inputText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Count the frequency of each character in the input text
    std::unordered_map<char, int> frequencyTable;
    for (char c : inputText) {
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

    std::cout << "Compression completed." << std::endl;
    std::cout << "Compression ratio: " << compressionRatio << "%" << std::endl;

    // Cleanup: Delete the Huffman tree nodes
    delete root;

    return 0;
}

