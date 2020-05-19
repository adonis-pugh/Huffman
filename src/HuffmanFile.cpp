/**
 * CS 106B/X Huffman Encoding
 *
 * This file declares the classes used to read/write compressed
 * Huffman-encoded files.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Julie Zelenski
 * @version 2019/5/22
 * - 19sp 106b version; slimmed bitstream, no pseudoEOF, dehydrated tree as header
 */
#include "HuffmanFile.h"
#include <fstream>
#include <sstream>
#include "error.h"
#include <cstring>

static const int NUM_BITS_IN_BYTE = 8;
static const char HUFF_TAG[2] = {'H','F'};

HuffmanInputFile::HuffmanInputFile(istream &str): in(str)
{
    const ios::off_type tSize = sizeof(HUFF_TAG);
    char tag[tSize];

    in.seekg(0, std::ios::beg);
    in.read(tag, tSize);
    if (memcmp(tag, HUFF_TAG, tSize) != 0) {
        error("File does not begin with proper Huffman tag.");
    }

    in.seekg(-tSize, ios::end);
    in.read(tag, tSize);
    if (memcmp(tag, HUFF_TAG, tSize) != 0) {
        error("File does not close with proper Huffman tag.");
    }

    in.seekg(-tSize-2, ios::end);
    finalTell = in.tellg();
    in.seekg(-tSize-1, ios::end);
    nFinalBits = in.get(); // this byte reports nbits in last data byte

    in.seekg(tSize, ios::beg);
    bitPos = 0;
}

string HuffmanInputFile::readHeader()
{
    unsigned int headerLen = 0;
    in >> headerLen;
    char* buf = new char[headerLen];
    in.read(buf, headerLen);
    if (!in) error("Error reading header from HuffmanInputFile");
    string s = string(buf, headerLen);
    delete[] buf;
    return s;
}

int HuffmanInputFile::readBit()
{
    // if just finished reading all bits from current byte
    if (bitPos == NUM_BITS_IN_BYTE) {
        in.get();  // advance stream to next byte
        bitPos = 0; // restart at first bit of new byte
	}
    // special case to read bits from last byte (may be partially filled)
    if (in.tellg() > finalTell || (in.tellg() == finalTell && bitPos >= nFinalBits)) {
        return EOF;
    }
    int bit = ((in.peek() & (1 << bitPos)) != 0);
    bitPos++; // increment bit position for next read
    return bit;
}

// Debugging output, displays bits as ASCII 0/1 characters
void HuffmanInputFile::displayFile()
{
    cout << readHeader() << endl;
    int bit, count = 0;
    while ((bit = readBit()) != EOF) {
        cout << bit;
        ++count;
        if (count % 64 == 0) cout << endl;
        else if (count % 8 == 0) cout << ' ';
    }
    cout << endl;
}

HuffmanOutputFile::HuffmanOutputFile(ostream &str): out(str)
{
    curByte = 0;
    bitPos = 0;
}

HuffmanOutputFile::~HuffmanOutputFile()
{
    out.put(char(bitPos));  // number of bits written in final data byte
    out.write(HUFF_TAG, sizeof(HUFF_TAG)); // closing tag
    out.flush();
}

void HuffmanOutputFile::writeHeader(string header)
{
    out.write(HUFF_TAG, sizeof(HUFF_TAG));
    out << header.size();
    out.write(header.c_str(), header.size());
}

void HuffmanOutputFile::writeBit(int bit)
{
    if (bit != 0 && bit != 1) {
        ostringstream msg;
        msg << "Invalid call HuffmanOutputFile::writeBit(" << bit << "). Bit argument must be either 0 or 1.";
        error(msg.str());
    }
        // if just filled curByte
    if (bitPos == NUM_BITS_IN_BYTE) {
		curByte = 0; // zero out byte for next writes
        bitPos = 0;	// start writing to first bit of new byte
	}
		
    if (bit == 1) // only need to change if bit needs to be 1 (byte starts already zeroed)
        curByte |= (1 << bitPos);
		
    if (bitPos == 0 || bit) { // only write if first bit in byte or changing 0 to 1
        if (bitPos != 0) out.seekp(-1, ios::cur); // back up to overwite if pos > 0
        out.put(curByte);
	}
	    
    bitPos++; // increment bit position for next write
}
