#include "HCTree.hpp"
#include "Helper.hpp"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <source file> <destination file>" << std::endl;
        return 2;
    }

    const char* sourceFilePath = argv[1];
    const char* destinationFilePath = argv[2];

    FancyInputStream encodedFileStream(sourceFilePath);
    if (!encodedFileStream.good()) {
        std::cerr << "Error: Cannot read from the source file." << std::endl;
        return 2;
    }

    FancyOutputStream decodedFileStream(destinationFilePath);
    if (!decodedFileStream.good()) {
        std::cerr << "Error: Cannot write to the destination file." << std::endl;
        return 2;
    }

    if (encodedFileStream.filesize() == 0) {
        return 0;  
    }

    HCTree huffmanTree;
    std::vector<int> frequencyTable(256, 0);
    int totalCharacters = 0;

   
    for (int i = 0; i < 256; ++i) {
        int frequency = 0;
        frequency |= encodedFileStream.read_byte() << 16;
        frequency |= encodedFileStream.read_byte() << 8;
        frequency |= encodedFileStream.read_byte();
        frequencyTable[i] = frequency;
        totalCharacters += frequency;
    }

    huffmanTree.build(frequencyTable);

 
    for (int i = 0; i < totalCharacters; ++i) {
        unsigned char decodedCharacter = huffmanTree.decode(encodedFileStream);
        decodedFileStream.write_byte(decodedCharacter);
    }

    return 0;
}
