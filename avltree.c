/*
 * @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * @version 20/11/2018
 * CIS2520 Assignment 4
 * avltree.c
    - Implements the AVL tree paradigm
    - Deal with nodes, each of which has a key and a count
    - Reads keys in file "A4_data_f18.txt", and inserts them into the tree
    - A key is a character string (eg. "flr830"), and the count is an integer which serves as the frequency count of the key
            --> For example, if the same key has been inserted into the tree three times, there is only one node for the key and the count value of the node is 3

    - Shows a menu with the following options:
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

    NOTE: assumption that keys in the file must be separated by spaces (as seen in example file given to us)
    NOTE: assumption that character string keys from data file will not be longer than 30
    NOTE: assumption the count of a node will not exceed 9,999,999,999
    NOTE: AVL tree is sorted by the character string keys, so it's sorted alphabetically (i.e. "abc100" comes before "abc9" because '1' comes before '9')
    NOTE: when calculatng the height of the tree, I include the empty leaf nodes (i.e. the edge to the leaf node) (eg. height if 1 node = 1)

    NOTE: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/ used in conjunction with lecture and lab slides to create insertion and rotation functions
    NOTE: https://www.geeksforgeeks.org/avl-tree-set-2-deletion/ used in conjunction with lecture and lab slides to create deletion functions
 */

// =========================== INCLUDED LIBRARIES ===========================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// =========================== STRUCT DECLARATIONS ==========================
typedef struct avl_tree_struct {
    struct avl_tree_struct *left;
    struct avl_tree_struct *right;
    char key[31];
    int count;
    int height;
} Node;

// =========================== FUNCTION PROTOTYPES ==========================
Node* read_file_create_tree(char *filename);
void find_key_print_key(Node* root, char* key_name, int *key_found, int yes_print);

void print_unbalanced(Node* root);
void calc_size_and_total(Node* root, int *size, int *total);
void print_find_all(Node* root, int freq);

Node* insert(Node *root, char *key_name);
Node* remove_node(Node *root, char *key_name, int *key_found);
Node* delete_node(Node *root, char *key_name);
void free_tree(Node *root);

Node* rebalance_tree(Node *root, char *key_name);
Node* rotate_right(Node *a);
Node* rotate_left(Node *a);

void flush_input(char *input);
int isNumber(char *input);
int isWhitespace(char *input);
int max(int num1, int num2);

Node* new_node(char *key_name);
int height(Node *root);
int calc_balance(Node *n);
Node* min_node(Node *root);

int main(int argc, char **argv) {
    // declare variables
    Node* root = NULL;
    char menu_input[3];
    char key_input[32];     //input -max 30 characters long
    char number_input[12];  //input -max 10 characters long

    int key_found = 0; //false
    int good_input = 0;//false

    int size = 0;
    int total_count = 0;

    // infinite menu loop, only ending program with '7' as input
    while(1) {
        printf("\n1. Initialization\n" \
                "2. Find\n" \
                "3. Insert\n" \
                "4. Remove\n" \
                "5. Check Height, Size, and Total Count\n" \
                "6. Find All (above a given frequency)\n" \
                "7. Exit\n" \
                "Enter a code (1 – 7) and hit Return\n" \
                "avl/> ");
        fgets(menu_input, 3, stdin);    //only need the first character of input for menu option
        flush_input(menu_input);

        // checks user input
        if(strlen(menu_input) > 1) {
            // display error msg inputting more than just the number option
            printf("\nError: invald input - You must choose one of the menu options by number: [1,7]\nPlease try again\n");

        }
        else if(menu_input[0] == '1') {         // ================================= 1 =================================
            // free tree first (in case it has already been init)
            free_tree(root);
            // initialize the tree using a text file
            root = read_file_create_tree("A4_data_f18.txt");
            // if this happens, messed up growing the tree -- shouldn't print
            if(root == NULL) {
                fprintf(stderr, "\t...still NULL here -I am root\n");
            }
            print_unbalanced(root); //will display any unbalanced nodes -- shouldn't print

        }
        else if(menu_input[0] == '2') {         // ================================= 2 =================================
            // Find key, only if tree is init
            if(root != NULL) {
                // prompt user for key
                good_input = 0;
                while(!good_input) {
                    printf("find key: ");
                    fgets(key_input, 32, stdin);
                    flush_input(key_input);

                    // checks input
                    if(strlen(key_input) > 30) {
                        printf("\nError: Invalid key - must be 30 characters or less\n"); //display err msg -too long
                    } else if(isWhitespace(key_input)) {
                        printf("\nError: Invalid key - please try again\n");              //display err msg -only whitespace chars
                    } else {
                        good_input = 1;
                    }
                }
                // get it to correct size
                strncpy(key_input, key_input, 30);
                key_input[strlen(key_input)] = '\0';

                key_found = 0; //reset
                // check for key, and print its match
                find_key_print_key(root, key_input, &key_found, 1);

                // display msg if key isn't in the AVL tree
                if(!key_found) {
                    printf("No_such_key\n");
                }
            } else {
                // display error msg for not selecting menu option 1 first
                printf("\nError: tree not initialized - You must select option 1. Initialization first\n");
            }

        }
        else if(menu_input[0] == '3') {         // ================================= 3 =================================
            // Insert key, only if tree is init
            if(root != NULL) {
                // prompt user for key
                good_input = 0;
                while(!good_input) {
                    printf("insert key: ");
                    fgets(key_input, 32, stdin);
                    flush_input(key_input);

                    // checks input
                    if(strlen(key_input) > 30) {
                        printf("\nError: Invalid key - must be 30 characters or less\n"); //display err msg -too long
                    } else if(isWhitespace(key_input)) {
                        printf("\nError: Invalid key - please try again\n");              //display err msg -only whitespace chars
                    } else {
                        good_input = 1;
                    }
                }
                // get it to correct size
                strncpy(key_input, key_input, 30);
                key_input[strlen(key_input)] = '\0';

                // insert the key appropriately
                root = insert(root, key_input);

                key_found = 0; //reset
                // print the key and its new frequency
                find_key_print_key(root, key_input, &key_found, 1);

            } else {
                // display error msg for not selecting menu option 1 first
                printf("\nError: tree not initialized - You must select option 1. Initialization first\n");
            }

        }
        else if(menu_input[0] == '4') {         // ================================= 4 =================================
            // Remove key, only if tree is init
            if(root != NULL) {
                // prompt user for key
                good_input = 0;
                while(!good_input) {
                    printf("remove key: ");
                    fgets(key_input, 32, stdin);
                    flush_input(key_input);

                    // checks input
                    if(strlen(key_input) > 30) {
                        printf("\nError: Invalid key - must be 30 characters or less\n"); //display err msg -too long
                    } else if(isWhitespace(key_input)) {
                        printf("\nError: Invalid key - please try again\n");              //display err msg -only whitespace chars
                    } else {
                        good_input = 1;
                    }
                }
                // get it to correct size
                strncpy(key_input, key_input, 30);
                key_input[strlen(key_input)] = '\0';

                key_found = 0; //reset
                // check for key, then decrement its count by 1 (remove the node if count = 1)
                remove_node(root, key_input, &key_found);

                // display msg if key wasn't in the AVL tree
                if(!key_found) {
                    printf("No_such_key\n");
                }
            } else {
                // display error msg for not selecting menu option 1 first
                printf("\nError: tree not initialized - You must select option 1. Initialization first\n");
            }

        }
        else if(menu_input[0] == '5') {         // ================================= 5 =================================
            // Check Height, Size, and Total Count, only if tree is init
            if(root != NULL) {
                // reset counters first, before calculating and printing the results
                size = 0;
                total_count = 0;
                calc_size_and_total(root, &size, &total_count);
                printf("height: %d, size: %d, total count: %d\n", height(root), size, total_count);
            } else {
                // display error msg for not selecting menu option 1 first
                printf("\nError: tree not initialized - You must select option 1. Initialization first\n");
            }

        }
        else if(menu_input[0] == '6') {         // ================================= 6 =================================
            // Find All (above a given frequency), only if tree is init
            if(root != NULL) {
                // prompt user for key
                good_input = 0;
                while(!good_input) {
                    printf("frequency: ");
                    fgets(number_input, 12, stdin);
                    flush_input(number_input);

                    // checks input
                    if(strlen(number_input) > 10) {
                        printf("\nError: Invalid frequency count - must be 10 characters or less\n"); //display err msg -too long
                    } else if(isWhitespace(number_input)) {
                        printf("\nError: Invalid frequency count - please try again\n");              //display err msg -only whitespace chars
                    } else if(isNumber(number_input)) { // checks that it's a number, >= 0
                        // check if it's 0
                        if(atoi(number_input) == 0) {
                            printf("\nError: Invalid frequency count - must be a positive number\n"); //display err msg -must be positive
                        } else {
                            good_input = 1;
                        }
                    } else {
                        printf("\nError: Invalid frequency count - please try again\n");              //display err msg
                    }
                }
                // get it to correct size
                strncpy(number_input, number_input, 10);
                number_input[strlen(number_input)] = '\0';

                // print all nodes that are >= given frequency
                print_find_all(root, atoi(number_input));

            } else {
                // display error msg for not selecting menu option 1 first
                printf("\nError: tree not initialized - You must select option 1. Initialization first\n");
            }

        }
        else if(menu_input[0] == '7') {         // ================================= 7 =================================
            printf("\nGood bye!\n");    // display closing msg
            free_tree(root);            // free before ending
            return 0;
        }
        else {                                  // =============================== ELSE ================================
            // display error msg for bad input
            printf("\nError: invald input - You must choose one of the menu options by number: [1,7]\nPlease try again\n");
        }
    } // end menu loop
} // end main

// =============================== FUNCTIONS ================================

/**
 * Reads the data for a text file, storing each word in an AVL tree that keeps count
 * @param filename -the name of the text file being read
 * @return -the created AVL tree
 */
Node* read_file_create_tree(char *filename) {
    // create and AVL tree (the root node)
    Node* root = NULL;
    char word[31] = ""; //30 char max

    FILE *fp = fopen(filename, "r");
    // checks if fopen messed up
    if(fp == NULL) {
        fprintf(stderr, "ERROR: File could not be opened\n");

    } else {
        // read one word at a time until the end of the file
        while(!feof(fp)) {
            fscanf(fp, " %s ", word);
            root = insert(root, word);
        }
    }
    fclose(fp);
    return root;
}

/**
 * Searches for a given key within the AVL tree and then prints it
 * @param root -the node being searched
 * @param key_name -the key being searched for
 * @param key_found -pass boolean as int=0: set to 1 if match is found, otherwise remains 0
 * @param yes_print -pass boolean as int: determines whether the found key is printed or not (prints on true)
 */
void find_key_print_key(Node* root, char* key_name, int *key_found, int yes_print) {
    if(root == NULL) {  //reached a leaf's left or right NULL child
        return;
    }
    // check if it's a match
    if(strcmp(key_name, root->key) == 0) {
        *key_found = 1; //found match, set true
        // print the key and it's count, if that's what's desired
        if(yes_print) {
            printf("key: %s, frequency: %d\n", root->key, root->count);
        }
        return;
    }
    // continue checking the left and right subtrees
    find_key_print_key(root->left, key_name, key_found, yes_print);
    find_key_print_key(root->right, key_name, key_found, yes_print);
}

/**
 * Prints any unbalanced nodes in the AVL tree
 * @param root -the AVL tree node being looked at
 */
void print_unbalanced(Node* root) {
    if(root == NULL) {      //recursive base case
        return;
    }
    // print the node if it's unbalanced
    int balance = calc_balance(root);
    if(balance < -1 || balance > 1) {
        printf("\tkey: %10s, count: %10d, balance: %3d\n", root->key, root->count, balance);
    }
    // continue checking left and right subtrees
    print_unbalanced(root->left);
    print_unbalanced(root->right);
}

/**
 * Calculates the size and total count of an AVL tree
 * @param root -the node being looked at
 * @param size -(passed by reference as int=0) the number of nodes in the AVL tree
 * @param total -(passed by reference as int=0) the total count of all nodes in the AVL tree
 */
void calc_size_and_total(Node* root, int *size, int *total) {
    if(root == NULL) {      //recursive base case
        return;
    }
    // accumulate
    (*size)++;
    (*total) += root->count;
    // traverse in order (after left subtree, but before right subtree)
    calc_size_and_total(root->left, size, total);
    calc_size_and_total(root->right, size, total);
}

/**
 * Prints all keys (with the count) that have a count greater than or equal to the given frequency
 * @param root -the node being looked at
 * @param freq -the given frequncy being compared
 */
void print_find_all(Node* root, int freq) {
    if(root == NULL) {      //recursive base case
        return;
    }
    // traverse in order (after left subtree, but before right subtree)
    print_find_all(root->left, freq);
    // print key and count if count is greater than or equal to the given frequency
    if(root->count >= freq) {
        printf("key: %s, frequency: %d\n", root->key, root->count);
    }
    print_find_all(root->right, freq);
}

/**
 * Adds a key to the AVL tree by inserting (if it already exists, then just increment count by 1)
 * --> It also rebalances the AVL tree where necessary using the appropriate rotation(s)
 * NOTE: if insertion causes a node in the AVL tree to become unbalanced, then the root node must be unbalanced
 * @param root -the subtree being looked at
 * @param key_name -the key being added
 * @return -the (new) balanced AVL tree
 */
Node* insert(Node *root, char *key_name) {
    // if at leaf, then add/insert node here (base case 1 -- end on empty)
    if(root == NULL) {
        return(new_node(key_name));
    }
    // binary search traverse for matching key
    if(strcmp(key_name, root->key) < 0) {
        root->left = insert(root->left, key_name);
    } else if(strcmp(key_name, root->key) > 0) {
        root->right = insert(root->right, key_name);
    } else {
        root->count++;  //accumulate count, it's a match
        return root;    // base case 2 -- end on match
    }
    // rebalance if necessary
    return rebalance_tree(root, key_name);
}

/**
 * Recursively does a binary search to find the given key, then decrements the node's count
 * @param root -the subtree being looked at
 * @param key_name -the key of the node having its count decremented
 * @param key_found -pass boolean as int=0: set to 1 if match is found, otherwise remains 0
 * @return -the (new) balanced AVL tree
 */
Node* remove_node(Node *root, char *key_name, int *key_found) {
    // recursive base case
    if(root == NULL) {
        return root;
    }
    // binary search traverse for matching key
    if(strcmp(key_name, root->key) < 0) {
        root->left = remove_node(root->left, key_name, key_found);
    } else if(strcmp(key_name, root->key) > 0) {
        root->right = remove_node(root->right, key_name, key_found);
    } else {
        *key_found = 1;     // it's a match, so remove this one
        // have to actually remove the node from AVL tree if count = 1, otherwise just decrement count by 1
        if(root->count > 1) {
            root->count--;  //decrement
            // print the key and its new frequency
            printf("key: %s, frequency: %d\n", root->key, root->count);

        } else {
            // print the msg that it has been removed from the AVL tree
            printf("key: %s, frequency: 0\n", root->key);

            // Check if node is leaf or parent with only one child (the same thing happens, one case just moves a NULL though)
            if(root->left == NULL ) {
                Node *temp = root;
                root = root->right;
                free(temp);
            } else if(root->right == NULL) {
                Node *temp = root;
                root = root->left;
                free(temp);

            } else {
                // If node is parent with two children, get the following inorder successor (i.e. smallest in the right subtree)
                // NOTE: it will have at least one NULL child, so we can delete it properly
                Node *temp = min_node(root->right);
                // Copy the inorder successor's data to this node
                strcpy(root->key, temp->key);
                root->count = temp->count;
                // Then, actually delete the inorder successor from that subtree
                root->right = delete_node(root->right, temp->key);
            }
        }
    }

    // end if tree had only one node
    if(root == NULL) {
        return root;
    }
    // rebalance if necessary
    return rebalance_tree(root, key_name);
}

/**
 * Recursively does a binary search to find the given key, then actually deletes the node
 * @param root -the subtree being looked at
 * @param key_name -the key being deleted
 * @return -the (new) balanced AVL tree
 */
Node* delete_node(Node *root, char *key_name) {
    // recursive base case
    if(root == NULL) {
        return root;
    }

    // binary search traverse for matching key
    if(strcmp(key_name, root->key) < 0) {
        root->left = delete_node(root->left, key_name);
    } else if(strcmp(key_name, root->key) > 0) {
        root->right = delete_node(root->right, key_name);
    } else {
        // have to actually delete the node from AVL tree
        // Check if node is leaf or parent with only one child (the same thing happens, one case just moves a NULL though)
        if(root->left == NULL ) {
            Node *temp = root;
            root = root->right;
            free(temp);
        } else if(root->right == NULL) {
            Node *temp = root;
            root = root->left;
            free(temp);

        } else {
            // If node is parent with two children, get the following inorder successor (i.e. smallest in the right subtree)
            // NOTE: it will have at least one NULL child, so we can delete it properly
            Node *temp = min_node(root->right);
            // Copy the inorder successor's data to this node
            strcpy(root->key, temp->key);
            root->count = temp->count;
            // Then, actually delete the inorder successor from that subtree
            root->right = delete_node(root->right, temp->key);
        }
    }

    // end if tree had only one node
    if(root == NULL) {
        return root;
    }
    // rebalance if necessary
    return rebalance_tree(root, key_name);
}

/**
 * Frees the entire tree (all nodes in the tree) by postorder traverse
 * @param root -the tree node to be freed
 */
void free_tree(Node *root) {
    if(root == NULL) {      //recursive base case
        return;
    }
    // print after traversing both left and right subtrees (post)
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// ============================ ROTATE FUNCTIONS ============================

/**
 * Rebalances the AVL tree (if necessary)
 * @param root -the subtree being looked at
 * @param key_name -the key being searched for (for insertion or deletion)
 * @return -the (new) balanced AVL tree
 */
Node* rebalance_tree(Node *root, char *key_name) {
    // Update height of this node
    root->height = 1 + max( height(root->left), height(root->right) );
    // Check if node has become unbalanced
    int balance = calc_balance(root);

    // Left-Left Case --> single right rotation
    if(balance > 1 && strcmp(key_name, root->left->key) < 0) {
        return rotate_right(root);
    }
    // Right-Right Case --> single left rotation
    if(balance < -1 && strcmp(key_name, root->right->key) > 0) {
        return rotate_left(root);
    }
    // Left-Right Case --> double left-right rotation
    if(balance > 1 && strcmp(key_name, root->left->key) > 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    // Right-Left Case --> double right-left rotation
    if(balance < -1 && strcmp(key_name, root->right->key) < 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }
    // return the (unchanged) node pointer
    return root;
}

/**
 * Right Rotation - rotates the AVL subtree right to rebalance
 * @param a -the unbalanced node
 * @return -the new root node of the subtree after the rotation
 */
Node* rotate_right(Node *a) {
    Node *b = a->left;  // the unbalanced node's left child
    Node *c = b->right; // the unbalanced node's left child's right child

    // Perform rotation -> (1) B moves to where A was, (2) A becomes the right child of B, (3) C becomes the left child of A
    b->right = a;   // (2)
    a->left = c;    // (3)

    // Update heights
    a->height = 1 + max( height(a->left), height(a->right) );
    b->height = 1 + max( height(b->left), height(b->right) );

    // new root of subtree is now B
    return b;       // (1)
}

/**
 * Left Rotation - rotates the AVL subtree left to rebalance
 * @param a -the unbalanced node
 * @return -the new root node of the subtree after the rotation
 */
Node* rotate_left(Node *a) {
    Node *b = a->right; // the unbalanced node's right child
    Node *c = b->left;  // the unbalanced node's right child's left child

    // Perform rotation -> (1) B moves to where A was, (2) A becomes the left child of B, (3) C becomes the right child of A
    b->left = a;    // (2)
    a->right = c;   // (3)

    //  Update heights
    a->height = 1 + max( height(a->left), height(a->right) );
    b->height = 1 + max( height(b->left), height(b->right) );

    // new root of subtree is now B
    return b;       // (1)
}

// ============================== MY FUNCTIONS ==============================

/**
 * Flushes all leftover data in the stream
 * @param char *input -the string that was just read from stdin
 */
void flush_input(char *input) {
    /* if the '\n' is NOT found in the word itself, flush the stream */
    if(strchr(input, '\n') == NULL) {
        while ((getchar()) != '\n');
        input[strlen(input)] = '\0';
    } else {
        input[strlen(input)-1] = '\0';
    }
}

/**
 * Checks if the string is a postive number (or zero)
 * @param char *input -the string to be checked
 * @return 1 if the string only contains digits
 */
int isNumber(char *input) {
    for(int i = 0; i < strlen(input); i++) {
        if(!isdigit(input[i])) {
            return 0; //contained a non-digit character
        }
    }
    return 1;
}

/**
 * Checks if the string is pure whitespace
 * @param char *input -the string to be checked
 * @return 1 if the string only contains whitespace characters
 */
int isWhitespace(char *input) {
    for(int i = 0; i < strlen(input); i++) {
        if(!isspace(input[i])) {
            return 0; //contained a non-whitespace character
        }
    }
    return 1;
}

/**
 * Determines the larger of two integers
 * @param num1 -the first number being compared
 * @param num2 -the second number being compared
 * @return -the greater of the two numbers
 */
int max(int num1, int num2) {
    int result;
    // check which is greater
    if(num1 > num2) {
        result = num1;
    } else {
        result = num2;
    }
    return result;
}

// ============================ HELPER FUNCTIONS ============================

/**
 * Creates a new node using a given string key
 * @param key_name -the key to be held by the new node
 * @return -the newly created node
 */
Node* new_node(char *key_name) {
    Node* new = malloc(sizeof(Node));
    strncpy(new->key, key_name, 30);
    new->key[strlen(new->key)] = '\0';

    new->count = 1; // counts are initially started at 1 (since if 0, doesn't exist in AVL tree)
    new->left = NULL;
    new->right = NULL;
    new->height = 1; // initially added as leaf (so h=1)
    return new;
}

/**
 * Retrieves the height of a subtree using the node's height attribute (but does the work of checking if node is NULL)
 * @param root -the subtree being looked at
 * @return -the height of the tree
 */
int height(Node *root) {
    if(root == NULL) {
        return 0;
    }
    return root->height;
}

/**
 * Calculates the balance factor of a node in the AVL tree
 * @param n -the node being looked at
 * @return -the balance factor of the node
 */
int calc_balance(Node *n) {
    if(n == NULL) {
        return 0;
    }
    return height(n->left) - height(n->right);
}

/**
 * Retrieves the smallest node (referenced to how AVL tree is sorted alphabetically by key) in the AVL tree (since it's an AVL tree, can be found at most bottom left node)
 * NOTE: used to get the next inorder successor of @param root
 * @param root -the AVL tree being looked at
 * @return -the node with the smallest key
 * NOTE: if root node is NULL, then it returns NULL
 */
Node* min_node(Node *root) {
    Node *curr = root; //temp node, use it to loop
    // continue to traverse left subtree(s) to find leftmost leaf
    while(curr->left != NULL) {
        curr = curr->left;
    }
    return curr;
}
