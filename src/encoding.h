/**
 * CS 106B/X Huffman Encoding
 * This file declares the functions that you will need to write in this
 * assignment for your Huffman Encoder in encoding.cpp.
 *
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Julie Zelenski
 * @version 2019/5/22
 * - 19sp 106b version; slimmed bitstream, no pseudoEOF, dehydrated tree as header
 * @author Marty Stepp
 * @version 2017/11/09
 * - 106x 17au version
 * @version 2016/11/12
 * - 106x 16au version
 * @version 2013/11/14
 * - initial version
 */

#ifndef _encoding_h
#define _encoding_h

#include <iostream>
#include <string>
#include "HuffmanFile.h"
#include "HuffmanNode.h"
#include "map.h"
using namespace std;

// function prototype declarations
// (If you want to declare your own helper function prototypes, do that near
//  the top of your .cpp file, not here.  You should NOT modify this file.)

Map<char, int> buildFrequencyTable(istream& input);
HuffmanNode* buildEncodingTree(Map<char, int>& freqTable);
string flattenTreeToHeader(HuffmanNode* encodingTree);
HuffmanNode* recreateTreeFromHeader(string str);
Map<char, string> buildEncodingMap(HuffmanNode* encodingTree);
void freeTree(HuffmanNode* t);

void compress(istream& input, HuffmanOutputFile& output);
void decompress(HuffmanInputFile& input, ostream& output);

#endif // _encoding_h

/*
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 */
