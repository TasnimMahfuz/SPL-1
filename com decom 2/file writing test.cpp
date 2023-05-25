#include <iostream>
#include <fstream>

int main() {
    std::ofstream file("chat history.txt", std::ios::app); // Open the file in append mode

    if (!file) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    std::string input;

    std::cout << "Enter a string (empty string to exit): ";
    std::getline(std::cin, input);

    while (!input.empty()) {
        file << input << std::endl; // Write the string to the file, followed by a newline
        std::cout << "String written to file." << std::endl;

        std::cout << "Enter a string (empty string to exit): ";
        std::getline(std::cin, input);
    }

    file.close(); // Close the file
    std::cout << "File closed." << std::endl;

    return 0;
}

