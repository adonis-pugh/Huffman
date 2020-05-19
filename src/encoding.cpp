/* HUFFMAN DATA COMPRESSOR
 * Name: Adonis Pugh
 * ------------------------
 * This function implements a version of the famous Huffman coding data compression
 * algorithm. The core of the algorithm relies on using variable-length encoding.
 * Compression is composed of five major steps: (1) constructing a map of character
 * frequencies, (2) constructing a binary tree from the frequency map, (3) converting
 * the character tree into a file header, (4) constructing an encoding map from the
 * character tree, and (5) using the encoding map to compress the file. Decompression
 * employs the reverse technique to generate the orginal file from the compressed version. */

#include "encoding.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "filelib.h"

/*****************************************
 *      PROTOTYPE / HELPER FUNCTIONS     *
 ****************************************/

void gatherLeaves(HuffmanNode* tree, Map<char, string>& map, string code);
HuffmanNode* recreateTreeFromHeaderHelper(string& str);

/*****************************************
 *              FUNCTIONS                *
 ****************************************/

/* A map is created associating characters with their frequencies. */
Map<char, int> buildFrequencyTable(istream& input)
{
    Map<char, int> freqTable;
    char ch;
    while(input.get(ch)) {
        freqTable[ch]++;
    }
    return freqTable;
}

/* A binary tree is constructed using by weighing the characters in a PriorityQueue
 * from the previously constructed frequency table. The two lowest priority items in
 * the queue are removed and combined to make a new node with the sum of their
 * priorities. This process is continued until the queue is singly occupied and the
 * tree is complete. */
HuffmanNode* buildEncodingTree(Map<char, int>& freqTable)
{
    PriorityQueue<HuffmanNode*> weights;
    for(char ch : freqTable) {
        HuffmanNode* tree = new HuffmanNode(ch);
        weights.enqueue(tree, freqTable[ch]);
    }
    while(weights.size() != 1) {
        double first = weights.peekPriority();
        HuffmanNode* tree1 = weights.dequeue();
        double second = weights.peekPriority();
        HuffmanNode* tree2 = weights.dequeue();
        HuffmanNode* newTree = new HuffmanNode(tree1, tree2);
        weights.enqueue(newTree, first + second);
    }
    return weights.dequeue();
}

/* The binary tree is converted to a header string by representing interior nodes as
 * a pair of parentheses wrapped around its two child (one and zero) nodes and
 * representing leaf nodes as a period followed by its specific character. This
 * function is implemented recursively to support repeating logic. */
string flattenTreeToHeader(HuffmanNode* t)
{
    string code = "";
    if(t == nullptr) {
        return code;
    } else if(t->isLeaf()) {
        return "." + charToString(t->ch);
    } else {
        code += "(";
        code += flattenTreeToHeader(t->zero);
        code += flattenTreeToHeader(t->one);
        code += ")";
        return code;
    }
}

/* This function calls its helper function to pass the string by reference. */
HuffmanNode* recreateTreeFromHeader(string str)
{
    return recreateTreeFromHeaderHelper(str);
}

/* The tree is regenerated from the string header by having calls of this
 * recursive function deal with a single pair of parentheses to construct
 * a HuffmanNode with its child (one and zero) nodes. Recursion allows this
 * process to convert the entire string header into a binary tree. */
HuffmanNode* recreateTreeFromHeaderHelper(string& str)
{
    HuffmanNode* tree;
    HuffmanNode* zero;
    HuffmanNode* one;
    if(startsWith(str, '.')) {
        tree = new HuffmanNode(str[1]);
        str = str.substr(2);
    } else {
        str = str.substr(1);
        zero = recreateTreeFromHeaderHelper(str);
        one = recreateTreeFromHeaderHelper(str);
        str = str.substr(1);
        tree = new HuffmanNode(zero, one);
    }
    return tree;
}

/* This function calls its helper function by passing in an empty
 * encoding map by reference. */
Map<char, string> buildEncodingMap(HuffmanNode* encodingTree)
{
    Map<char, string> encodingMap;
    gatherLeaves(encodingTree, encodingMap, "");
    return encodingMap;
}

/* Characters are mapped to binary strings by fully traversing a binary tree
 * and recording the paths of ones and zeros taken to arrive at them. */
void gatherLeaves(HuffmanNode* tree, Map<char, string>& map, string code)
{
    if(tree == nullptr) {
        return;
    } else if(tree->isLeaf()) {
        map[tree->ch] = code;
    } else {
        gatherLeaves(tree->zero, map, code + integerToString(0));
        gatherLeaves(tree->one, map, code + integerToString(1));
    }
}

/* This function deallocates the memory used by a binary tree. */
void freeTree(HuffmanNode* t)
{
    HuffmanNode* tree = t;
    if(tree != nullptr) {
        t = tree->zero;
        freeTree(t);
        t = tree->one;
        freeTree(t);
    }
    delete tree;
}

/* The compression algorithm operates in five steps: (1) constructing a map
 * of character frequencies, (2) constructing a binary tree from the frequency map,
 * (3) converting the character tree into a file header, (4) constructing an
 * encoding map from the character tree, and (5) using the encoding map to
 * compress the file. The final step in particular is the only code unique to
 * this function. Each character in the input stream is mapped to its new binary
 * equivalent and the binary representation is looped over to output each bit. */
void compress(istream& input, HuffmanOutputFile& output)
{
    Map<char, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* tree = buildEncodingTree(freqTable);
    string header = flattenTreeToHeader(tree);
    Map<char, string> binaryMap = buildEncodingMap(tree);
    freeTree(tree);
    output.writeHeader(header);
    rewindStream(input);
    string bits;
    char ch;
    while(input.get(ch)) {
        bits = binaryMap[ch];
        for(size_t i = 0; i < bits.length(); i++) {
            output.writeBit(charToInteger(bits[i]));
        }
    }
}

/* The decompression algorithm works in a reverse manner to the compression
 * algorithm. The map used by this function is a reverse mapping of the original
 * map. That is, binary strings are now associated with characters. The input
 * file is read bit-wise. Based on the prefix property of Huffman encoding,
 * characters are outputted immediately after a binary string mathes a string
 * in the map. This process is continued until the entire binary stream has
 * been read and the file decompressed. */
void decompress(HuffmanInputFile& input, ostream& output)
{
    HuffmanNode* tree = recreateTreeFromHeader(input.readHeader());
    Map<char, string> binaryMap = buildEncodingMap(tree);
    freeTree(tree);
    Map<string, char> characterMap;
    for(char ch : binaryMap) {
        string binary = binaryMap[ch];
        characterMap[binary] = ch;
    }
    int bit = 0;
    while(true) {
        string bits = "";
        while(!characterMap.containsKey(bits)) {
            bit = input.readBit();
            if(bit == EOF) return;
            bits += integerToString(bit);
        }
        output.put(characterMap[bits]);
    }
}
