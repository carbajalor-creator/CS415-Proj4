#include <iostream> 
#include <vector>
#include <fstream>
#include <string>


/** Thought Process:
 * --Input to Trie
 * read in input from file
 * one word per line and use a parser function to convert input stream to lower case and remove punctuation
 * parser function is going to tokenize the file so produce an array of words
 * the words will get passed to our build trie functions
 * build trie will iterate over the token array one word token at a time
 * search both tries for words if character not found (letter doesn't exist in trie or last char isn't marked eow) create a new trie node
 * if found (we have iterated over entire token and last token has eow == true) we return, no need to add a duplicate
 * --Search
 * search function will take a token and iterate over it character by character 
 * if character found and not end of token keep searching
 * if character not found return not found
 * if character found, end of token, and last character in token is marked as eow in trie return found
 * --Autocomplete 
 * search tries w/ DFS 
 * create sibling words vector that will be printed as the autocomplete words
 * have a temp word variable and build word as you traverse nodes
 * when node is marked end of word push back word to vector of sibling words
 * if node is marked eow but there is still children (ex: sun, sunny if we are at n and n's eow == true) keep searching until child ptrs are null
 * pop back when child ptrs are null (maybe treat temp word variable as a stack)
 *
 */



struct TrieNode {
    bool eow; //end of word
    char c; //value
    std::vector<TrieNode> child[26]; //alphabet array
    int ptrCount;//? do we need it? we can just multiply 26 x number of nodes
};

struct TernaryNode {
    bool eow;
    char c;
    TernaryNode* left; //points to character less than our c (sibling word)
    TernaryNode* right; //points to character greater than our c (sibling word)
    TernaryNode* equal; //points to next character in word
    //int ptrCount;//? increment by 3 when new node is created
    TernaryNode(char c) : eow(false), c(c), left(NULL), right(NULL), equal(NULL) {}
    
};

std::string parse(std::string& line) //add guarding if string is empty
{
    //convert all characters to lowercase and remove punctuation marks
    char c;
    std::string token;
    for (int i = 0; i < line.length(); i++) 
    {
        c = line[i];
        //if c has any punctuation, special characteres, dashes, or double dashes, remove them
        if (ispunct(line[i]))
        {
            line.erase(i--, 1);
            std::cout << "Removed punctuation: " << c << std::endl;
            continue;
        }
        c = tolower(line[i]);
        token += c;
    }

    return token;
}

std::vector<std::string> readFile(std::string filename){
    //read one word per line
    //call parser token = parse(line)
    //call functions to build tries and pass token to them
    std::ifstream file (filename);
    std::string line;
    std::vector<std::string> tokens;

    if(!file.is_open()) {
        std::cout << "Error: Unable to open file " << filename << std::endl;
        return {};
    }
    while (std::getline(file, line)) {
        std::string token = parse(line);
        tokens.push_back(token);
    }
    return tokens;
}

int charIdx(char &c) { 
    int charIdx = int(c) - 97; // a = 0
    return charIdx;
}

void output() {
    //print time takes to build tries (as a number of operations)
    //space occupied by tries (as a number of pointers)
    //number of operations to search in each trie (case 1: per prefix inputted, case 2: sum of operations for entire input file)
}
/*
TrieNode searchArrHelper(int idx, char c) {
    //is given an idx and returns array of node
}*/

void DFS_Collect(TernaryNode* node, std::string& prefix, std::string& suffix, std::vector<std::string>& siblingWords, int &ternOpCount) {
    if (node == NULL) { //base case 
        return;
    }
    if (node->left != NULL) {
        ternOpCount++; 
        DFS_Collect(node->left, prefix, suffix, siblingWords, ternOpCount);
    }
    suffix += node->c;
    if (node->eow == true) {
        ternOpCount++;
        siblingWords.push_back(prefix + suffix);
    }
    if (node->equal != NULL) {
        ternOpCount++;
        DFS_Collect(node->equal, prefix, suffix, siblingWords, ternOpCount);
    }
    suffix.pop_back();
    if (node->right != NULL) {
        ternOpCount++;
        DFS_Collect(node->right, prefix, suffix, siblingWords, ternOpCount);
    }
}

std::vector<std::string> findSiblings(std::string prefix, TernaryNode* &root, int &ternOpCount) {
    TernaryNode* node = root;
    std::string suffix = "";
    std::vector<std::string> siblingWords;
    
    int i = 0;
    while(node != NULL) {
        if (prefix[i] == node->c) { //if a match check where we are in the prefix
            ternOpCount++;
            if ( i < prefix.size() - 1) { //if we haven't reached the last character
                i++;
                node = node->equal;
            }
            else if ( i == prefix.size() - 1) { //if we are at the last character in prefix start collecting sibling words
                DFS_Collect(node->equal, prefix, suffix, siblingWords, ternOpCount); //pass the last nodes equal ptr to point to next char that will point to rest of sibling words 
                return siblingWords;
                
            }
        }
        else if (prefix[i] < node->c) { //if not a match and smaller than node go left
            ternOpCount++;
            node = node->left;
        }
        else if (prefix[i] > node->c) { //if not a match and greater than node go right
            ternOpCount++;
            node = node->right;
        }

    } 
    return siblingWords;
}



void printSiblings(std::string prefix, TernaryNode* &root, int &ternOpCount) {
    //dfs search on tries
    //create sibling words vector
    //create word variable and treat it like a stack
    //if node.eow == true push back to sibling words
    //if node ptrs all point to null popback explore next branch
    std::vector<std::string>ternSiblings;
    std::vector<std::string>arrSiblings;
    ternSiblings = findSiblings(prefix, root, ternOpCount);
    for (int i = 0; i < ternSiblings.size(); i++) {
        std::cout << ternSiblings[i] << std::endl;
    }
}
/*
void searchArrTrie(std::string prefix, bool findSiblings) {
    //call charIdx to build list of indexes from prefix
    std::vector<int> prefixIndices;
    for (char c : prefix) {
        prefixIndices.push_back(charIdx(c));
    }
    for ( int i : prefixIndices) {
        //use idx to search in tree if trie[i] is present keep searching
        //if trie[i] == prefix[i] continue if not return not found 
        //if last idx and eow == false word not found
        //if last idx and eow == true word found
        //recursively searches deeper? or have a function that is given an array and searches for a letter and returns array
    }

}
*/
TernaryNode* insertTernaryTrie(TernaryNode* node, std::string word, int i, int &ternOpCount) {
    if (node == NULL) {
        node = new TernaryNode(word[i]);
        ternOpCount++;
    }
    if (word[i] == node->c) {
        ternOpCount++;
        if ( i < word.size() - 1) {
            node->equal = insertTernaryTrie(node->equal, word, i + 1, ternOpCount);
            ternOpCount++;
        }
        else if ( i == word.size() - 1) { 
            node->eow = true;
            ternOpCount++;
            return node; //are we at the end of the word 
        }
    }
    else if ( word[i] > node->c) {
        ternOpCount++; //update for the comparison
        node->right = insertTernaryTrie(node->right, word, i, ternOpCount);
        ternOpCount++; //for the assignment
    }
    else if (word[i] < node->c) {
        ternOpCount++; //update for the comparison
        node->left = insertTernaryTrie(node->left, word, i, ternOpCount);
        ternOpCount++; //update for the assignment
    }
    return node;

}

TernaryNode* insert(TernaryNode* root, std::string word, int &ternOpCount) { //this should probably be changed to take both array and ternary tries
    return insertTernaryTrie(root, word, 0, ternOpCount);
}

bool searchTernaryTrie(std::string &prefix, TernaryNode *node) { 
    //iterate through linked lists
    //if prefix[i] == node.c explore equal
    //if prefix[i] > node.c explore node.right
    //if prefix[i] < node.c explore node.left
    //if == explore node.equal
    //if prefix[i]==node->c && i = prefix.size() && node->eow == true return found 
    int i = 0;
    while(node != NULL) {
        if (prefix[i] == node->c) {
            if ( i < prefix.size() - 1) {
                i++;
                node = node->equal;
            }
            else if ( i == prefix.size() - 1) { 
                return node->eow? true : false; //are we at the end of the word 
            }
        }
        else if (prefix[i] < node->c) {
            node = node->left;
        }
        else if (prefix[i] > node->c) {
            node = node->right;
        }

    }
    return false;
}
/* 
void buildArrTrie(std::string token, TrieNode &root) 
{
    TrieNode* currentNode = &root;

    //we iterate across the chars inside of each string
    for (int i = 0; i < token.length(); i++)
    {
        // is this seat taken?
        if (currentNode->child[token[i] - 'a'] == NULL)
        {
            //no? then dont mind if i do
            TrieNode *newNode = new TrieNode();
            std::cout << "character: " << token[i] << std::endl;

            //im gonna leave my jacket here to remind me, that this seat is mine
            currentNode->child[token[i] - 'a'] = newNode;
            
        }

        //oh darn the seat is taken
        //let me just move to the next row of seats
        currentNode = currentNode->child[token[i] - 'a'];

    } 
    //all done
    currentNode->eow = true;   
}
*/
void buildTernaryTrie(std::string token, TernaryNode* &root, int &ternOpCount) {
    //call search to make sure it's not already there
    //if not there create new node and insert
    //if end of token is reached and c is found set c.eow = true
    if (token.empty()) return;
    if (!searchTernaryTrie(token, root)) {
        root = insert(root, token, ternOpCount);
    }
    else {
        std::cout << token << " already in trie" << std::endl;
    }

}


int main(int argc, char* argv[]) {
    int ternOpCount = 0;
    TrieNode arrRoot;
    TernaryNode* root = NULL;
    std::string filename = argv[1];
    std::string prefix;
    std::vector<std::string> tokens = readFile(filename);
    std::vector<std::string> ternSiblingWords;
    for (int i = 0; i < tokens.size(); i++) {
        buildTernaryTrie(tokens[i], root, ternOpCount);
        //also add build array trie here
    }
    std::cout<< "Number of operations for ternary trie: " << ternOpCount <<std::endl;
    std::cout << "Enter a prefix: ";
    std::cin >> prefix;

    ternSiblingWords = findSiblings(prefix, root, ternOpCount);
    printSiblings(prefix, root, ternOpCount);

    return 0;
}