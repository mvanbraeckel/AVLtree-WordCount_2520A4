# AVLtree-WordCount_2520A4
Data Structures course A4 - reads a file and constructs an AVL tree that counts how many times each word appears, and can further manipulate the AVL tree after this

(Data Structures course Assignment #4)

NOTE: assumed submission should be in a ".tar" file like the other assignment submissions

To compile, just type "make" to compile everything.

A4_data_f18.txt file format:
NOTE: assumption that the file being used is a text file named "A4_data_f18.txt"
NOTE: assumption that keys in the file must be separated by spaces (as seen in the example file given to us)
NOTE: leading spaces, trailing spaces, and blank lines in the text file are accounted for
NOTE: assumption that character string keys from data file will not be longer than 30 characters
NOTE: assumption the count of a node will not exceed 9,999,999,999 (it will not appear more than this many times in the file)

avltree.c   - Usage: ./avltree
NOTE: AVL tree is sorted by the character string keys, so it's sorted alphabetically (i.e. "abc100" comes before "abc9" because '1' comes before '9')
NOTE: when calculatng the height of the tree, I include the empty leaf nodes (i.e. the edge to the leaf node) (eg. height if 1 node = 1)
NOTE: (see program comments for more detailed information on things, eg. error checking)

E.g.
Input:  ./avltree
- has a menu loop that prompts the user for what they want to do and their 7 options (see below)
1. Initialization
2. Find
3. Insert
4. Remove
5. Check Height, Size, and Total Count
6. Find All (above a given frequency)
7. Exit
Enter a code (1 – 7) and hit Return
avl/>
--> 1: Initialization                       - open the file, read and insert keys into the AVL tree one word at a time (if other options selected, first display error msg)
--> 2. Find                                 - prompts for key, then searches AVL tree: display the key and its frequency count, or "No_such_key" if key is not found
--> 3. Insert                               - prompts for key, then searches for position where key should be placed:
                                                Increases the count if the key is already in AVL tree, otherwise inserts key as a new node (set the count to be 1)
                                                Prints the key with it's new count afterwards
--> 4. Remove                               - prompts for key, then searches for match:
                                                Removes the node only if node's count = 1, otherwise it just decreases the node's count by one, or displays "No_such_key" if key not found
                                                Prints the key with it's new count afterwards
--> 5. Check Height, Size, and Total Count  - computes and displays the height of AVL tree, size of AVL tree (i.e. number of nodes), and total count (i.e. sum of all counts)
--> 6. Find All (above a given frequency)   - prompts for positive integer, then displays all keys whose frequency counts are greater than or equal to the inputted integer
--> 7. Exit                                 - will only terminate program if '7' is entered
NOTE: displays error msg if an option that is not [1,7] is entered for menu selection
NOTE: input prompts to user are displayed appropriately per menu option based on the assignment handout's description and guidelines' example(s)

NOTE: any URLs in code are referencing learning resources used to assist with things we have not done yet and/or not really talked about in detail
