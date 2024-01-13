# Huffman Encoding Program

This C++ program performs Huffman encoding, a widely-used algorithm for lossless data compression. Huffman encoding is based on building a binary tree called the Huffman tree, where each character is represented by a unique binary code.

## Overview

The program consists of the following components:

1. **Node Class (`node`):**
   - Represents a node in the Huffman tree.
   - Contains frequency, left, and right pointers.

2. **BinaryTree Class (`BinaryTree`):**
   - Manages the construction and manipulation of the Huffman tree.
   - Provides functionality for encoding and decoding.

3. **MinHeap Class (`minheap`):**
   - Implements a min heap to efficiently manage nodes based on their frequency.

4. **Encoding Functions:**
   - `unique_characters`: Finds all unique characters in a file.
   - `calculate_freq`: Calculates the frequencies of unique characters.
   - `Encode`: Encodes a file using Huffman encoding.

5. **Decoding Function:**
   - `Decode`: Decodes a file using a Huffman tree.

## How to Run

1. Compile and run the program.
2. Enter the name of the file you want to compress when prompted.
3. Provide the name for the compressed version with a `.bin` extension.
4. The program will display a success message upon successful compression.
5. Enter the name of the decompressed version with a `.txt` extension.
6. The program will display a success message upon successful decompression.

## Huffman Tree Storage

The program stores the Huffman tree structure in a file named `tree.txt`. This file is used for tree restoration during decompression.

## File Extensions

Make sure to provide file extensions while entering filenames (e.g., `.txt`, `.bin`).

## Note

This program uses a simple command-line interface and assumes that the data won't include the `|` character.

Feel free to explore and customize the program based on your requirements and additional features.

