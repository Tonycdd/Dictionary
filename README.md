# Dictionary
Top M words from a dictionary


# Features

  ###  Trie-based Dictionary: 
  Efficient word storage and retrieval using a Trie data structure.
 
  ###  Heap for Top M Words: 
  A custom K-ary heap structure to efficiently extract the top M words.

  ###  Score Calculation: 
  Words are scored based on their presence in the Trie and their frequency in the input text.
 
  ###  Input:
  
  The program reads a text file containing words and counts their occurrences.
 
  ###  Output:
  The top M words along with their scores are displayed.

# Code Structure

  ##  Trie.hpp:
  Contains the TrieDictionary class for managing the dictionary and word storage.
  
  ##  KHeap.hpp:
  Defines the KHeap class for managing the heap structure, used for extracting the top M words.
  
  ##  Main Program:
  The main program (main.cpp) handles user input, processes the text file, and interacts with the dictionary and heap to extract and display the top M w
