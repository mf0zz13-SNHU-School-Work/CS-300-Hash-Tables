//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Michael Foster
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 5-2 Binary Search Tree
//============================================================================

#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

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

// Internal structure for tree node
struct Node {
	Bid bid;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a bid
	Node(Bid aBid) :
		Node() {
		bid = aBid;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define
 */
class BinarySearchTree {

private:
	Node* root;

	void addNode(Node* node, Bid bid);
	void inOrder(Node* node);
	void postOrder(Node* node);
	void preOrder(Node* node);
	Node* removeNode(Node* node, string bidId);
	void destroyNode(Node* node);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void InOrder();
	void PostOrder();
	void PreOrder();
	void Insert(Bid bid);
	void Remove(string bidId);
	Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
	// Calling helper method to recursively delete all nodes in search tree
	destroyNode(root);
	root = nullptr;
}

// Helper method to postorder traverse and delete nodes.
void BinarySearchTree::destroyNode(Node* node) {
	if (node == nullptr)
		return;

	destroyNode(node->left); // recurseve call to left node
	destroyNode(node->right); // recurseve call to right node
	delete node;
}


/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	// Calling inOrder method passing root as start point
	inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
	// Calling postOrder method passing root as start point
	postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
	// Calling preOrder method passing root as start point
	preOrder(root);
}



/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
	// If root is null then a new node is created and root pointer is set to new node
	if (root == nullptr)
		root = new Node(bid);
	else
		addNode(root, bid); // Calling addNode function to add node to tree

}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
	// Calling removeNode, passing root and bidId
	removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
	// Pointer to current node
	Node* currNode = root;

	while (currNode != nullptr) {
		Bid currBid = currNode->bid; // Setting currBid to bid of the node for the iteration
		if (currBid.bidId == bidId) // If bidId is a match the bid is returned
			return currBid;
		else {
			if (bidId < currBid.bidId) // If the bidId being searched is less than the current bidId the the next node checked is set to left node
				currNode = currNode->left;
			else // If the bidId being searched is not less than the current bidId the the next node checked is set to right node
				currNode = currNode->right;
		}
	}
	Bid bid;
	return bid; // Defalut constructed bid returned if bidId not found
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
	// Pointer to keep track of the current node
	Node* currNode = node;

	while (currNode != nullptr) { // While loop to iterate until bid is palced in tree
		if (bid.bidId < currNode->bid.bidId) { // If the bidId is less than the bidId of current node then the nodes left pointer is examined 
			if (currNode->left == nullptr) { // If left pointer is null then a new node is created with the bid to be added and placed there
				currNode->left = new Node(bid);
				break;
			}
			else // If left pointer is not null then it is selected as the new current node
				currNode = currNode->left;
		}
		else { // If the bidId is not less than the bidId of the current node then the nodes right pointer is examined
			if (currNode->right == nullptr) { // If right pointer is null then a new node is created with the bid to be added and placed there
				currNode->right = new Node(bid);
				break;
			}
			else // If right pointer is not null then it is slected as the new current node
				currNode = currNode->right;
		}
	}
}
void BinarySearchTree::inOrder(Node* node) {
	// Quick exit if node passed is null
	if (node == nullptr)
		return;

	// Recursive call to left node
	inOrder(node->left);

	// Output bid information 
	Bid b = node->bid;
	cout << b.bidId << ": " << b.title << " | " << b.amount << " | " << b.fund << endl;

	// Recursive call to right node
	inOrder(node->right);


}
void BinarySearchTree::postOrder(Node* node) {
	// Quick exit if node passed is null
	if (node == nullptr)
		return;

	// Recursive call to left node then right node
	postOrder(node->left);
	postOrder(node->right);

	// Output bid information 
	Bid b = node->bid;
	cout << b.bidId << ": " << b.title << " | " << b.amount << " | " << b.fund << endl;

}

void BinarySearchTree::preOrder(Node* node) {
	// Quick exit if node passed is null   
	if (node == nullptr)
		return;

	// Output bid information
	Bid b = node->bid;
	cout << b.bidId << ": " << b.title << " | " << b.amount << " | " << b.fund << endl;

	// Recursive call to left node then right node
	preOrder(node->left);
	preOrder(node->right);
}

/**
 * Remove a bid from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string bidId) {

	Node* currNode = node; // Pointer to current node
	Node* parentNode = nullptr; // Pointer to parent node

	// Quick exit if node passed is null
	if (currNode == nullptr)
		return node;

	while (currNode != nullptr) {
		if (currNode->bid.bidId == bidId) {
			if (currNode->left == nullptr && currNode->right == nullptr) { // If the node does not have children
				if (parentNode == nullptr) { // If the parent node is null then the current node is the root and the root pointer is set to null
					root = nullptr;
					delete currNode;
				}
				else if (parentNode->left == currNode) { // If the current node is the left node of its parent the parents left pointer is set to null
					parentNode->left = nullptr;
					delete currNode;
				}
				else if (parentNode->right == currNode) { // If the current node is the right node of its parent the parents right pointer is set to null
					parentNode->right = nullptr;
					delete currNode;
				}
			}
			else if (currNode->right == nullptr) { // If the node does not have a right child
				if (parentNode == nullptr) { // If the parent node is null then the current node is the root and the root is set to the current nodes left child
					root = currNode->left;
					delete currNode;
				}
				else if (parentNode->left == currNode) { // If the current node is the left node of its parent the parents left pointer is set to current nodes left pointer
					parentNode->left = currNode->left;
					delete currNode;
				}
				else {
					parentNode->right = currNode->left;
					delete currNode;
				}
			}
			else if (currNode->left == nullptr) {
				if (parentNode == nullptr) {
					root = currNode->right;
					delete currNode;
				}
				else if (parentNode->right == currNode) {
					parentNode->right = currNode->right;
					delete currNode;
				}
				else {
					parentNode->left = currNode->right;
					delete currNode;
				}
			}
			else {
				Node* successor = currNode->right;
				while (successor->left != nullptr)
					successor = successor->left;
				Bid successorBid = successor->bid;
				currNode->bid = successor->bid;
				currNode->right = removeNode(currNode->right, successorBid.bidId);
			}
			return currNode;
		}
		else if (bidId < currNode->bid.bidId) {
			parentNode = currNode;
			currNode = currNode->left;
		}
		else {
			parentNode = currNode;
			currNode = currNode->right;
		}
	}
	return nullptr;

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
void loadBids(string csvPath, BinarySearchTree* bst) {
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
			bst->Insert(bid);
		}
	}
	catch (csv::Error& e) {
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

	// Define a binary search tree to hold all bids
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Bid bid;

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
			loadBids(csvPath, bst);

			//cout << bst->Size() << " bids read" << endl;

			// Calculate elapsed time and display result
			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			break;

		case 2:
			bst->InOrder();
			break;

		case 3:
			ticks = clock();

			bid = bst->Search(bidKey);

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

			if (!bid.bidId.empty()) {
				displayBid(bid);
			}
			else {
				cout << "Bid Id " << bidKey << " not found." << endl;
			}

			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

			break;

		case 4:
			bst->Remove(bidKey);
			break;
		}
	}

	cout << "Good bye." << endl;

	return 0;
}
