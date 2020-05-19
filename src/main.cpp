/**
 * CS 106B/X Huffman Encoding
 * This file contains the main program and user interface for running your
 * Huffman Encoder.  It contains a text menu to allow you to piecewise test
 * your functions to build a Huffman encoding.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Julie Zelenski
 * @version 2019/5/22
 * - 19sp 106b version; slimmed bitstream, no pseudoEOF, dehydrated tree as header
 * @author Marty Stepp
 * @version 2016/11/12
 * - 17au 106x version; menu changes based on different required functions
 * @version 2016/11/12
 * - 16au 106x version
 * @version 2013/11/21
 * - updated to fix compiler errors with opening files on older compilers
 */

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include "HuffmanNode.h"
#include "encoding.h"
#include "HuffmanFile.h"
#include "bitstream.h"
using namespace std;

const bool SHOW_TREE_ADDRESSES = false;   // set to true to debug tree pointer issues
const string COMPRESSED_FILE_NAME = ".huf";
const string DECOMPRESSED_FILE_NAME = "unhuf.";

// function prototype declarations; see definitions below for documentation
void intro();
string menu();
void test_buildFrequencyTable(Map<char, int>& freqTable);
void test_buildEncodingTree(Map<char, int>& freqTable, HuffmanNode*& encodingTree);
void test_buildEncodingMap(HuffmanNode* encodingTree);
void test_flattenTree(HuffmanNode* encodingTree);
void test_compress();
void test_decompress();
void view_huffmanFile();
void view_textFile();
void view_sideBySideComparison();
istream* openStream();



int main()
{
    intro();

    // these variables maintain state between steps 1-4
    HuffmanNode* encodingTree = nullptr;
    Map<char, int> freqTable;

    // prompt user for options repeatedly
    while (true) {
        string choice = menu();
        if (choice == "Q") {
            break;
        } else if (choice == "1") {
            test_buildFrequencyTable(freqTable);
            encodingTree = nullptr;
        } else if (choice == "2") {
            test_buildEncodingTree(freqTable, encodingTree);
        } else if (choice == "3") {
            test_buildEncodingMap(encodingTree);
        } else if (choice == "4") {
            test_flattenTree(encodingTree);
        } else if (choice == "C") {
            test_compress();
        } else if (choice == "D") {
            test_decompress();
        } else if (choice == "H") {
            view_huffmanFile();
        } else if (choice == "T") {
            view_textFile();
        } else if (choice == "S") {
            view_sideBySideComparison();
        }
    }

    cout << "Exiting." << endl;
    return 0;
}

/*
 * Sets up the output console and explains the program to the user.
 */
void intro()
{
    cout << "Welcome to CS 106B/X Shrink-It!" << endl;
    cout << "This program uses the Huffman coding algorithm for compression." << endl;
    cout << "Any type of file can be encoded using a Huffman code." << endl;
    cout << "Decompressing the result will faithfully reproduce the original." << endl;
    cout << "The compression achieved can be substantial for large files of " << endl;
    cout << "homogenous content. (Smaller, heterogenous files see less/no benefit.)" << endl;
}

/*
 * Prints a menu of choices for the user and reads/returns the user's response.
 */
string menu()
{
    cout << endl;
    cout << "MAIN MENU" << endl;
    cout << "1) test build character frequency table" << endl;
    cout << "2) test build encoding tree" << endl;
    cout << "3) test build encoding map" << endl;
    cout << "4) test flatten/recreate encoding tree header" << endl;
    cout << endl;
    cout << "C) compress file" << endl;
    cout << "D) decompress file" << endl;
    cout << endl;
    cout << "H) view contents of huffman file" << endl;
    cout << "T) view contents of text file" << endl;
    cout << "S) side-by-side file comparison" << endl;
    cout << "Q) quit" << endl;

    cout << endl;
    string choice = toUpperCase(trim(getLine("Your choice? ")));
    return choice;
}

/*
 * Tests the buildFrequencyTable function.
 * Prompts the user for a string of data or input file to read,
 * then builds a frequency map of its characters and prints that map's contents.
 * Stores the built map in the given output parameter freqTable.
 */
void test_buildFrequencyTable(Map<char, int>& freqTable)
{
    istream* input = openStream();
    cout << "Building frequency table ..." << endl;
    freqTable = buildFrequencyTable(*input);
    for (char ch : freqTable) {
        displayChar(cout, ch);
        cout << " => " << setw(7) << freqTable[ch] << endl;
    }
    delete input;
}

/*
 * Tests the buildEncodingTree function.
 * Accepts a frequency table map as a parameter, presumably the one generated
 * previously in step 1 by buildFrequencyTable.
 * Then prints the encoding tree in an indented sideways format.
 * Stores the built tree in the given output parameter encodingTree.
 */
void test_buildEncodingTree(Map<char, int>& freqTable, HuffmanNode*& encodingTree)
{
    if (freqTable.isEmpty()) {
        cout << "Can't build tree; character frequency table is empty or uninitialized." << endl;
    } else {
        cout << "Building encoding tree ..." << endl;
        encodingTree = buildEncodingTree(freqTable);
        printSideways(encodingTree, SHOW_TREE_ADDRESSES);
    }
}

/*
 * Tests the flattenTreeToHeader and recreateTreeFromHeader functions.
 * Accepts an encoding tree as a parameter, presumably the one generated
 * previously in step 2 by buildEncodingTree.
 * Flattens the encoding tree into its header string representation,
 * then uses that header string to create a new encoding tree. This
 * second tree should be a copy of the original encoding tree. It
 * prints the tree copy and then frees it.
 */
void test_flattenTree(HuffmanNode* encodingTree)
{
    if (encodingTree == nullptr) {
        cout << "Can't flatten tree; encoding tree is empty or uninitialized." << endl;
    } else {
        string str = flattenTreeToHeader(encodingTree);
        cout << "Encoding tree flattened to header string: " << str << endl;
        cout << "Copy of encoding tree recreated from header string: "<< endl;
        HuffmanNode *treeCopy = recreateTreeFromHeader(str);
        printSideways(treeCopy, SHOW_TREE_ADDRESSES);
        cout << "Freeing memory for copy of encoding tree." << endl;
        freeTree(treeCopy);
    }
}

/*
 * Tests the buildEncodingMap function.
 * Accepts an encoding tree as a parameter, presumably the one generated
 * previously in step 2 by buildEncodingTree.
 * Builds the encoding map of characters to binary encoding and prints it.
 */
void test_buildEncodingMap(HuffmanNode* encodingTree)
{
    if (encodingTree == nullptr) {
        cout << "Can't build map; encoding tree is null." << endl;
    } else {
        cout << "Building encoding map ..." << endl;
        Map<char, string> encodingMap = buildEncodingMap(encodingTree);
        for (char ch : encodingMap) {
            displayChar(cout, ch);
            cout << "  => " << encodingMap[ch] << endl;
        }
    }
}

/*
 * Prompts for names of files to use for compress/decompress operation.
 */
bool getInputAndOutputFiles(string& inFilename, string& outFilename, bool compressing)
{
    inFilename = promptUserForFile("Input file name: ");
    string defaultName;
    if (compressing) {
        defaultName = inFilename + COMPRESSED_FILE_NAME;
    } else {
        defaultName = DECOMPRESSED_FILE_NAME + getRoot(inFilename);
    }
    outFilename = trim(getLine("Output file name (Enter for " + defaultName + "): "));
    if (outFilename == "") {
        outFilename = defaultName;
    }
    if (inFilename == outFilename) {
        cout << "You cannot specify the same filename as both the input file" << endl;
        cout << "and the output file.  Canceling operation." << endl;
        return false;
    }
    if (fileExists(outFilename)) {
        return getYesOrNo(outFilename + " already exists. Overwrite? (y/n) ");
    }
    return true;
}

/*
 * Tests the compress function.
 * Prompts for input/output file names and opens streams on those files.
 * Then calls your compress function and displays information about size of
 * compressed output.
 */
void test_compress()
{
    string inFilename, outFilename;

    if (!getInputAndOutputFiles(inFilename, outFilename, true)) {
        return;
    }
    cout << "Reading " << fileSize(inFilename) << " input bytes." << endl;
    try {
        ifstream input(inFilename, ifstream::binary);
        ofstream outstr(outFilename, ofstream::binary);
        HuffmanOutputFile output(outstr);
        cout << "Compressing ..." << endl;
        compress(input, output);
    } catch (ErrorException& e) {
        cout << "Ooops! " << e.getMessage() << endl;
    }

    if (fileExists(outFilename)) {
        cout << "Wrote " << fileSize(outFilename) << " compressed bytes." << endl;
    } else {
        cout << "Compressed output file was not found; perhaps there was an error." << endl;
    }
}

/*
 * Tests the decompress function.
 * Prompts for input/output file names and opens streams on those files.
 * Then calls your decompress function and displays information about size of
 * decompressed output.
 */
void test_decompress()
{
    string inFilename, outFilename;

    if (!getInputAndOutputFiles(inFilename, outFilename, false)) {
        return;
    }
    cout << "Reading " << fileSize(inFilename) << " input bytes." << endl;
    try {
        ifstream instr(inFilename, ifstream::binary);
        HuffmanInputFile input(instr);
        ofstream output(outFilename, ofstream::binary);
        cout << "Decompressing ..." << endl;
        decompress(input, output);
    } catch (ErrorException& e) {
        cout << "Ooops! " << e.getMessage() << endl;
    }

    if (fileExists(outFilename)) {
        cout << "Wrote " << fileSize(outFilename) << " decompressed bytes." << endl;
    } else {
        cout << "Decompressed output file was not found; perhaps there was an error." << endl;
    }
}

/*
 * Huffman file viewer function.
 * Prompts the user for a file name and then prints all bits/bytes of that file.
 */
void view_huffmanFile()
{
    string filename = promptUserForFile("Name of huffman file to display: ");
    try {
        ifstream instr(filename, ifstream::binary);
        HuffmanInputFile input(instr);
        cout << endl << "Here is the binary data (" << fileSize(filename) << " bytes):" << endl;
        input.displayFile();
    } catch (ErrorException& e) {
        cout << "Ooops! " << e.getMessage() << endl;
    }
}

/*
 * Text file viewer function.
 * Prompts the user for a file name and prints all text in that file.
 */
void view_textFile()
{
    string filename = promptUserForFile("Name of text file to display: ");
    cout << endl << "Here is the text data (" << fileSize(filename) << " bytes):" << endl;
    cout << readEntireFile(filename) << endl;
}

/*
 * Side-by-side file comparison function.
 * Prompts for two file names and then checks their contents,
 * printing information about differences between the two.
 * Most of this code is written by Keith Schwarz.
 */
void view_sideBySideComparison()
{
    string filename1 = promptUserForFile("First file name: ");
    string defaultName = DECOMPRESSED_FILE_NAME + filename1;
    string filename2 = trim(getLine("Second file name (Enter for " + defaultName + "): "));
    if (filename2 == "") {
        filename2 = defaultName;
    }
    if (!fileExists(filename2)) {
        cout << "No such file '" << filename2 << "', canceling operation." << endl;
        return;
    }

    string fileText1 = readEntireFile(filename1);
    string fileText2 = readEntireFile(filename2);

    // compare the two sequences to find a mismatch
    pair<string::const_iterator, string::const_iterator> diff =
        mismatch(fileText1.begin(), fileText1.end(), fileText2.begin());
    if (diff.first != fileText1.end()) {
        ptrdiff_t offset = diff.first - fileText1.begin();
        cout << "File data first difference at byte offset " << offset << ":" << endl;
        displayChar(cout, *diff.first);
        cout << setw(16) << filename1 << endl;
        displayChar(cout, *diff.second);
        cout << setw(16) << filename2 << endl;
        int size1 = fileSize(filename1);
        int size2 = fileSize(filename2);
        if (size1 != size2) {
            cout << "File sizes differ: ";
            cout << filename1 << "=" << size1 << " bytes, " << filename2 << "=" << size2 << " bytes." << endl;
        }
    } else {
        cout << "Files " << filename1 << " and " << filename2 << " match!" << endl;
    }
}

/*
 * Prompts the user to choose between reading from a string or file.
 * If the user wants to read from a string, asks the user to type said string.
 * If the user wants to read from a file, asks the user for the file name.
 * Then opens an input stream for the appropriate type of data and returns
 * a pointer to the stream.
 * The memory for the stream must be freed by the caller.
 */
istream* openStream()
{
    while (true) {
        string choice = toLowerCase(trim(getLine("Read from a s)tring or f)ile? ")));
        if (startsWith(choice, 's')) {
            string data = getLine("Type the string to process: ");
            return new istringstream(data);
        } else if (startsWith(choice, 'f')) {
            string name = promptUserForFile("File name to process: ");
            return new ifstream(name, ifstream::binary);
        }
    }
}
