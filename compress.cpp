#include "HCTree.hpp"
#include "Helper.hpp"
#include <iostream>
#include <vector>
using namespace std;

void compressFile(const char* sourcePath, const char* destinationPath) {
    FancyInputStream sourceStream(sourcePath);
    if (!sourceStream.good()) {
        cerr << "Error: Unable to read from the source file." << endl;
        return;
    }

  
    if (sourceStream.filesize() == 0) {
        FancyOutputStream destinationStream(destinationPath);
        cerr << "The source file is empty. No data to encode." << endl;
        return;
    }

 
    vector<int> byteFrequencies(256, 0);
    int currentByte;
    while ((currentByte = sourceStream.read_byte()) != -1) {
        byteFrequencies[currentByte]++;
    }

    
    HCTree huffmanTree;
    huffmanTree.build(byteFrequencies);

 
    FancyOutputStream destinationStream(destinationPath);
    for (int frequency : byteFrequencies) {
        destinationStream.write_byte(frequency >> 16);
        destinationStream.write_byte(frequency >> 8);
        destinationStream.write_byte(frequency);
    }

  
    sourceStream.reset();
    while ((currentByte = sourceStream.read_byte()) != -1) {
        huffmanTree.encode(static_cast<unsigned char>(currentByte), destinationStream);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <source file> <destination file>" << endl;
        return 2;
    }

    compressFile(argv[1], argv[2]);
    return 0;
}
