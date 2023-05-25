#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <vector>

// Structure for a node in the Huffman tree
struct HuffmanNode {
    char data;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data) : data(data), left(nullptr), right(nullptr) {}
};

// Function to deserialize the Huffman codes from a file
std::unordered_map<std::string, char> deserializeHuffmanCodes(std::ifstream& inputFile) {
    std::unordered_map<std::string, char> codes;
    size_t numCodes;
    inputFile.read(reinterpret_cast<char*>(&numCodes), sizeof(numCodes));

    for (size_t i = 0; i < numCodes; i++) {
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
std::string decompressText(const std::vector<char>& compressedData, const std::unordered_map<std::string, char>& codes) {
    std::string decompressedText;
    std::string currentCode;

    for (char byte : compressedData) {
        std::bitset<8> bitBuffer(byte);

        for (int bitIndex = 7; bitIndex >= 0; bitIndex--) {
            bool bit = bitBuffer[bitIndex];
            currentCode += (bit ? '1' : '0');

            if (codes.count(currentCode) > 0) {
                decompressedText += codes.at(currentCode);
                currentCode.clear();
            }
        }
    }

    return decompressedText;
}

int main() {
    std::ifstream compressedFile("compressed.txt", std::ios::binary);
    std::ifstream codesFile("huffman_codes.txt", std::ios::binary);
    std::ofstream decompressedFile("decompressed.txt");

    if (!compressedFile) {
        std::cerr << "Error opening compressed file." << std::endl;
        return 1;
    }

    if (!codesFile) {
        std::cerr << "Error opening Huffman codes file." << std::endl;
        return 1;
    }

    if (!decompressedFile) {
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

    std::cout << "Decompression completed." << std::endl;

    return 0;
}
