#include "HCTree.hpp"
#include <stack>
#include <queue>
#include <vector>
#include "Helper.hpp"
#include <iostream>


void deleteHuffmanTreeNodes(HCNode* currentNode) {
    if (currentNode == nullptr) {
        return;
    }
    deleteHuffmanTreeNodes(currentNode->c0);
    deleteHuffmanTreeNodes(currentNode->c1);
    delete currentNode;
}


HCTree::~HCTree() {
    deleteHuffmanTreeNodes(root);
}


void HCTree::build(const vector<int>& byteFrequencies) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> priorityQueue;


    for (size_t index = 0; index < byteFrequencies.size(); ++index) {
        if (byteFrequencies[index] > 0) {
            HCNode* leafNode = new HCNode(byteFrequencies[index], static_cast<unsigned char>(index));
            priorityQueue.push(leafNode);
            leaves[index] = leafNode;
        }
    }

   
    while (priorityQueue.size() > 1) {
        HCNode* leftChild = priorityQueue.top();
        priorityQueue.pop();
        HCNode* rightChild = priorityQueue.top();
        priorityQueue.pop();

        HCNode* parentNode = new HCNode(leftChild->count + rightChild->count, '$');
        parentNode->c0 = leftChild;
        parentNode->c1 = rightChild;
        leftChild->p = parentNode;
        rightChild->p = parentNode;

        priorityQueue.push(parentNode);
    }

    root = priorityQueue.empty() ? nullptr : priorityQueue.top();
}


void HCTree::encode(unsigned char symbol, FancyOutputStream& outputStream) const {
    HCNode* currentNode = leaves[symbol];
    stack<int> encodingBits;

    while (currentNode != root) {
        encodingBits.push(currentNode == currentNode->p->c1);
        currentNode = currentNode->p;
    }

    while (!encodingBits.empty()) {
        outputStream.write_bit(encodingBits.top());
        encodingBits.pop();
    }
}


unsigned char HCTree::decode(FancyInputStream& inputStream) const {
    HCNode* currentNode = root;
    while (currentNode->c0 || currentNode->c1) {
        int bit = inputStream.read_bit();
        if (bit == -1) break; // Handle EOF or read error
        currentNode = (bit == 0) ? currentNode->c0 : currentNode->c1;
    }
    return currentNode ? currentNode->symbol : '\0'; // Return '\0' for error or EOF
}
