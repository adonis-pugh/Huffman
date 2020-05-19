/**
 * CS 106B/X Huffman Encoding
 *
 * This file declares the HuffmanNode structure that you will use in your
 * Huffman encoding tree.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Julie Zelenski
 * @version 2019/5/22
 * - 19sp 106b version; slimmed bitstream, no pseudoEOF, dehydrated tree as header
 * @author Marty Stepp
 * @version 2016/11/12
 * - 16au 106x version
 * @version 2013/11/14
 * - initial version
 */

#ifndef _huffmannode_h
#define _huffmannode_h

#include <iostream>
#include <string>
using namespace std;

/* Type: HuffmanNode
 * A node inside a Huffman encoding tree. A leaf node stores
 * a character value and has empty 0 and 1 subtrees. An interior
 * node stores an empty character variable and pointers to
 * to non-empty 0 and 1 subtrees.
 */
struct HuffmanNode {
    char ch;             // character represented if leaf node, not used for interior
    HuffmanNode* zero;   // 0 (left) subtree (nullptr if empty)
    HuffmanNode* one;    // 1 (right) subtree (nullptr if empty)

    /*
     * Constructs a new leaf node for the given character. A leaf node
     * has null child pointers.
     */
    HuffmanNode(char ch);


    /*
     * Constructs a new interior node that is the parent of the two
     * child subtrees, zero and one.
     */
    HuffmanNode(HuffmanNode* zero, HuffmanNode* one);

    /*
     * Returns true if this node is a leaf (has null children).
     */
    bool isLeaf() const;

};

/*
 * Prints an indented horizontal view of the tree of HuffmanNodes with the given
 * node as its root.
 * Can optionally show the memory addresses of each node for debugging.
 */
void printSideways(HuffmanNode* node, bool showAddresses = false, string indent = "");


void displayChar(ostream& out, char ch);


#endif // _huffmannode_h
