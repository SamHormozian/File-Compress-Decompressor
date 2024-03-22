#ifndef HCTREE_HPP
#define HCTREE_HPP
#include <queue>
#include <vector>
#include <fstream>
#include "Helper.hpp"
using namespace std;


class HCTree {
    private:
        HCNode* root;
        vector<HCNode*> leaves;

    public:
       
        HCTree() : root(nullptr) {
            leaves = vector<HCNode*>(256, nullptr);
        }

        ~HCTree();

      
        void build(const vector<int>& freqs);

        
        void encode(unsigned char symbol, FancyOutputStream & out) const;

       
        unsigned char decode(FancyInputStream & in) const;
};
#endif // HCTREE_HPP
