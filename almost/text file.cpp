/*#include <iostream>
#include <fstream>

// Assuming you have the clientName and decompressedText variables defined
int main()
{


std::string historyName = "chatHistory8.txt"; // File name to store the history
int i = 1;
while (i++ < 8) {
    // Open the file in append mode to retain existing text
    std::ofstream historyFile(historyName, std::ios_base::app);

    std::string decompressedText;
    std::cin >> decompressedText;
    if (historyFile.is_open()) {
        // Write the desired output to the file
        historyFile << "clientName "<< ": " << decompressedText << "\n";

        // Close the file after writing
        historyFile.close();
    } else {
        std::cout << "Error opening the file!" << std::endl;
    }

   } // Rest of your loop code...
}
*/



#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>

// Assuming you have the clientName and decompressedText variables defined

int main()
{

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter; // Converter for UTF-8 encoding

std::string historyName = "chatHistory8.txt"; // File name to store the history
int i = 0;
while (i++ < 8) {
    // Open the file in append mode to retain existing text with UTF-8 encoding
    std::wofstream historyFile(historyName, std::ios_base::app | std::ios_base::binary);


    std::string decompressedText;
    std::cin >> decompressedText;

    if (historyFile.is_open()) {
        // Convert the clientName and decompressedText to wstring for UTF-8 encoding
        std::wstring wideClientName = converter.from_bytes("clientName");
        std::wstring wideDecompressedText = converter.from_bytes(decompressedText);

        // Write the desired output to the file
        historyFile << wideClientName << L": " << wideDecompressedText << L"\n";

        // Close the file after writing
        historyFile.close();
    } else {
        std::cout << "Error opening the file!" << std::endl;
    }

    // Rest of your loop code...
}
}
