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
 * - initial custom version, to replace the bitstream in Stanford CPP library
 */

#ifndef _hufffile_h
#define _hufffile_h

#include <iostream>
using namespace std;

/*
 * Classes: HuffmanInputFile, HuffmanOutputFile
 * --------------------------------------------
 * These two classes are used to read/write Huffman-encoded binary files.
 * The contents of a Huffman file starts with the header (read/writtten
 * via the readHeader/writeHeader functions) and the subsequent data is a
 * sequence of bits (read/written via the readBit/writeBit functions).
 */

class HuffmanInputFile
{
  public:

    /*
     * Constructor
     */
    HuffmanInputFile(istream& str);

    /*
     * Member function: readHeader
     * Usage: str = in.readHeader();
     * -----------------------------
     * Reads the header from the file and returns it as a string.
     * This is the header string that was earlier written to the file
     * by a call to HuffmanOutputFile::writeHeader.
     * Throws an exception on error.
     */
    string readHeader();
	
	/*
     * Member function: readBit
     * Usage: bit = in.readBit();
	 * --------------------------
     * Reads a single bit from the input file and returns 0 or 1 depending on
     * the bit value.  If all bits have been read from the file, returns EOF (-1).
     * Throws an exception on error.
	 */
	int readBit();

	
    /*
     * Member function: displayFile
     * Usage: in.displayFile();
     * ------------------------
     * Displays a representation of the file contents suitable for use
     * as a debugging aid.
     * Throws an exception on error.
     */
    void displayFile();

  private:
    streampos finalTell;
    int bitPos, nFinalBits;
    istream& in;
};


class HuffmanOutputFile
{
  public:

    /*
     * Constructor/destructor
     */
    HuffmanOutputFile(ostream &str);
    ~HuffmanOutputFile();

    /*
     * Member function: writeHeader
     * Usage: out.writeHeader(str);
     * ----------------------------
     * Writes the given header string to the file.
     * Throws an exception on error.
     */
    void writeHeader(string str);

    /*
     * Member function: writeBit
     * Usage: out.writeBit(1);
     * -----------------------
     * Writes a single bit to the file. Value of bit must be 0 or 1.
     * Throws an exception on error.
     */
    void writeBit(int bit);

  private:
    char curByte;
    int bitPos;
    ostream& out;
};

#endif
