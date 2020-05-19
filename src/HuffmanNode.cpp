/**
 * CS 106B/X Huffman Encoding
 * This file implements the members of the HuffmanNode structure that you will
 * use in your Huffman encoding tree.  See HuffmanNode.h for documentation of
 * each member.
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

#include "HuffmanNode.h"
#include <iomanip>
#include "stringutils.h"

static void printHuffmanNode(ostream& out, HuffmanNode* t, bool showAddress);

HuffmanNode::HuffmanNode(char ch)
{
    this->ch = ch;
    this->zero = nullptr;
    this->one = nullptr;
}

HuffmanNode::HuffmanNode(HuffmanNode* zero, HuffmanNode* one)
{
    this->ch = '\0';
    this->zero = zero;
    this->one = one;
}

bool HuffmanNode::isLeaf() const
{
    return zero == nullptr && one == nullptr;
}

void printSideways(HuffmanNode* t, bool showAddresses, string indent)
{
    if (t != nullptr) {
        printSideways(t->one, showAddresses, indent + "  ");
        cout << indent;
        printHuffmanNode(cout, t, showAddresses);
        cout << endl;
        printSideways(t->zero, showAddresses, indent + "  ");
    }
}

void displayChar(ostream& out, char ch)
{
    out << setw(4) << stringutils::toPrintable(ch) << " (Ascii " << setw(3) << int(static_cast<unsigned char>(ch)) << ")";
}

static void printHuffmanNode(ostream& out, HuffmanNode* t, bool showAddress)
{
    if (showAddress) {
        out << "@" << t;
    }
    out << "{";
    if (t->isLeaf()) {      // leaf has character, but no children
        out << "leaf, ";
        displayChar(out, t->ch);
    } else {
        out << "interior";  // interior has children, but no character
        if (showAddress) {
            out << ", zero=" << t->zero << ", one=" << t->one;
        }
    }
    out << "}";
}
