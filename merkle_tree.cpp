/*
Implementation of the merkle tree and node classes and helper function to read from an inputed file.
*/

/*
Implementation of the insert() function:

My insert() function is only 3 lines of code, since I created two functions, deleteTree() and buildTree(), 
which would inserting a new element a lot easier. All I did was push_back the new value into the already existing 
'data', then call deleteTree() to delete the tree (and free up the memory). Then finally, I rebuild the tree 
using the buildTree() function and the updated data.

buildTree() hashes all the data and builds the tree leaves-up by concatenating lower levels.
*/

#include "merkle_tree.h"

#include <queue>
using namespace std;

/* Hash function*/
inline string fnv1a(string const & text) {
    // 64 bit params
    uint64_t constexpr fnv_prime = 1099511628211ULL;
    uint64_t constexpr fnv_offset_basis = 14695981039346656037ULL;
    
    uint64_t hash = fnv_offset_basis;
    
    for(auto c: text) {
        hash ^= c;
        hash *= fnv_prime;
    }

    return to_string(hash);
}

/* Merkle Tree method for computing the hash of the concatenated hashes of children nodes 
   Using the provided hash function, concatenates child hashes from left to right and returns the hash 
   of the concatenation */
string merkleTree::concatenateHash(const vector<Node*>& nodes){
    string merged;
    //concatenate hashes of all inputed nodes 
    for(const Node* node: nodes){
        merged += node->key;
    }

    //take hash of concatenated nodes
    string mergedHash = fnv1a(merged);

    return mergedHash;
}

//getRoot() - (not provided or part of the question, but I saw that it was used in the main file)
Node* merkleTree::getRoot(){
	return root;
}

//Node() - constructor for node
Node::Node(const string& val){
	key = val;
	Node* parent = nullptr;
	vector<Node*> *children = nullptr;
}

//merkleTree() - constructor of empty merkle tree
merkleTree::merkleTree(){
	root = nullptr;
}

//merkleTree() - constructor of a tree from data vector
merkleTree::merkleTree(const vector<int>& data){
	root = nullptr;
	this -> data = data;

	buildTree();	
}

//buildTree() - helper function to build/rebuild merkle tree
void merkleTree::buildTree(){
	vector <Node*> leaves; //bottom leaves vector

	for(int i : data){
		string h = fnv1a(to_string(i)); //hash data
		leaves.push_back(new Node(h)); //leaf stores hash
	}

	//combine leaf nodes to construct tree
	while(leaves.size() > 1){ //if leaves.size() == 1, you completed the tree (are at the top)
		vector<Node*> parents;

		for(int i = 0; i < leaves.size(); i += 4){ //4-ary merkle
			vector <Node*> children;

			for(int j = i; j < i + 4 && j < leaves.size(); ++j){
				children.push_back(leaves[j]);
			}

			//create parent node by concatenating hashes
			string parentHash = concatenateHash(children);
			Node* parentNode = new Node(parentHash);
			parentNode -> children = children;
			parents.push_back(parentNode);
		}

		leaves.swap(parents); 
	}

	//if there is one node, then size isnt > 1, so root is just the only leaf
	if(!leaves.empty()){ 
		root = leaves[0];
	}
}

//deleteTree() - (base case) deletes merkle tree recursively given root node
void merkleTree::deleteTree(){
	deleteTree(root);
	root = nullptr;
}

//deleteTree() - deletes merkle tree recursively given root node
void merkleTree::deleteTree(Node* r){
	if(r == nullptr){ //if root is pointing to nothing, done
		return;
	} else {
		for(Node* child : r -> children){
			deleteTree(child);
		}

		delete r; 
	}
}

//printTree() - visualize merkle tree and its content
void merkleTree::printTree(const Node* node, int depth){
	if(node == nullptr){
		return;
	}

	queue<const Node*> q;
	q.push(root);

	int currentLevel = 0;
	int elemsInCurrentLevel = 1;
	int elemsAtNextLevel = 0;

	while(!q.empty()){
		const Node* currentNode = q.front();

		q.pop();

		//go down a level
		if(elemsInCurrentLevel == 0){
			currentLevel++;

			elemsInCurrentLevel = elemsAtNextLevel;
			elemsAtNextLevel = 0;
		}

		for(int i = 0; i < currentLevel; i++){
			cout << " "; //indent size is based on the depth of tree (currentLevel)
		}

		cout << "Level " << currentLevel << ": " << currentNode -> key << endl;

		elemsInCurrentLevel--;

		//prepare for next level by pushing all the children to the queue
		for(auto child : currentNode -> children){
			q.push(child);

			elemsAtNextLevel++;
		}
	}
}

//verify() - checks merkle tree's accuracy
void merkleTree::verify(){
	Node* old_root = root;

	buildTree();

	buildTree();

	if(root -> key == old_root -> key){
		cout << "True";
	} else {
		cout << "False";
	}
}

//overwrite() - overrides given value with new input value and rehashes the tree
void merkleTree::overwrite(int originalValue, int newValue){
	bool exists = false;

	//Linear search (sequential search) to find index of the original value
	for(int i = 0; i < data.size(); i++){
		if(data[i] == originalValue){
			data[i] = newValue;
			exists = true;
			break; //only changing the first occurrence is sufficient
		}
	}

	if(exists){
		//rebuild tree with modified data
		deleteTree();
		buildTree();
	} else {
		return;
	}
}

//insert() - inserts new value to the end of data vector and rehashes the tree
void merkleTree::insert(int newValue){
	data.push_back(newValue);

	deleteTree();

	buildTree();
}

//printRoot() - prints the root
void merkleTree::printRoot(){
	if(root){ //print key unless tree is empty
		cout << root -> key << endl;
	} else {
		cout << "Empty tree" << endl;
	}
}

//readIntsFromFile() - reads ints from file
vector<int> readIntsFromFile(const string& filename){
	fstream input_file;
	input_file.open(filename, std::ios::in); //open and read file

	vector<int> nums;

	if(input_file.is_open()){
		int num;

		while(input_file >> num){
			nums.push_back(num);
		}
	}

	input_file.close();

	return nums;
}

