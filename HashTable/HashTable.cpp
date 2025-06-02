//============================================================================
// Name        : HashTable.cpp
// Author      : Michael Foster
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Lab 4-2 Hash Table
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
    size_t Size();
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // initializing nodes with tableSize
    nodes.resize(tableSize);
    
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // updating table size with user defined parameter and initializing nodes with updated table size
    this->tableSize = size;
    nodes.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // looping through all nodes 
    for (unsigned int i = 0; i < nodes.size(); i++) {
        Node* current = &nodes[i];
        
        // iterating though and deleting chained nodes
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // returning hashed value
    return (key % tableSize);
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // setting key value
    unsigned int tempKey = hash(stoi(bid.bidId));

    // if node at key is has not been assinged a value node is placed in index
    if (nodes.at(tempKey).key == UINT_MAX) {
        Node tempNode = Node(bid, tempKey);
        nodes.at(tempKey) = tempNode;
    } 
    // if index of hash has a node then the new node is chained to current nodes at index
    else {
        Node* currentPos = &nodes.at(tempKey);
        Node* tempNode = new Node(bid, tempKey);

        while (currentPos->next != nullptr) {
            currentPos = currentPos->next;
        }

        currentPos->next = tempNode;
    }

}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // iterating through list and if a node exists at index it is printed along with chained nodes
    for (unsigned int i = 0; i < tableSize; i++) {
        if (nodes.at(i).key != UINT_MAX) {
            Node* currentPos = &nodes.at(i);
            cout << "Key ";
            do {
                cout << currentPos->key << ": " << currentPos->bid.bidId << " | " << currentPos->bid.title << " | " << currentPos->bid.amount << " | " << currentPos->bid.fund << endl;
                currentPos = currentPos->next;
            } while (currentPos != NULL);
        }
    }

}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // getting key value from hash and setting current node as pointer to index
    unsigned int tempKey = hash(stoi(bidId));
    Node* currNode = &nodes.at(tempKey);
  
    // if the current nodes bidId is equal to value to remove and no values are chained node is replaced with default empty node
    if (currNode->bid.bidId == bidId && currNode->next == nullptr) {
        currNode = new Node();
    }
    // if current node bidId is equal to value to remove and is head node then the node is removed and next node is set to head position.
    else if (currNode->bid.bidId == bidId && currNode->next != nullptr) {
        Node* temp = currNode->next;
        currNode->bid = temp->bid;
        currNode->key = temp->key;
        currNode->next = temp->next;
        delete temp;

    }
    // if current node bidId does not match and their are addtional chained values iteration begins
    else if (currNode->bid.bidId != bidId && currNode->next != nullptr) {
        while (currNode->next->bid.bidId != bidId && currNode != nullptr) {
            currNode = currNode->next;
        }
        // if next node is the node to be removed then current nodes next pointer is adjusted and node to be removed is deleted
        if (currNode->next->bid.bidId == bidId) {
            Node* temp = currNode->next;
            currNode->next = currNode->next->next;
            delete temp;
        }
        else
        {
            cout << "bidId not found" << endl;
        }
    }
    else { cout << "bidId not found" << endl; }
    

  
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;
    // getting key value from hash and setting current node to pointer of index location   
    unsigned int tempKey = hash(stoi(bidId));
    Node* currNode = &nodes.at(tempKey);

    // iterating through chained values to find bid value
    while (currNode != NULL) {
        if (currNode->bid.bidId == bidId) {
            return currNode->bid;
        }
        else { currNode = currNode->next;}
    } 

    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();
    
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
