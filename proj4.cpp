#include <iostream> 
#include <vector>
#include <fstream>

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
    int ptrCount;//? increment by 3 when new node is created
};


std::string parse(std::string& line){
    //convert all characters to lowercase and remove punctuation marks
    char c;
    std::string token;
    for (int i = 0; i < line.length(); i++) {
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

void readFile(std::string filename){
    std::ifstream file(filename);

    // try to open the file
    if (!file.is_open()) 
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string line;

    //read one word per line
    while (std::getline(file,line))
    {
        //call parser token = parse(line)
        //call functions to build tries and pass token to them
        std::string token = parse(line);
        buildArrTrie(token, TrieNode());
        buildTernaryTrie(token, TernaryNode());

    }
    
}

int charIdx(char &c) { 
    int charIdx = int(c) - 65; // a = 0
    return charIdx;
}

void output() {
    //print time takes to build tries (as a number of operations)
    //space occupied by tries (as a number of pointers)
    //number of operations to search in each trie (case 1: per prefix inputted, case 2: sum of operations for entire input file)
}

// TrieNode searchArrHelper(int idx, char c) {
//     //is given an idx and returns array of node
//     return 0, ;
// }

void printSiblings(std::string prefix) {
    //dfs search on tries
    //create sibling words vector
    //create word variable and treat it like a stack
    //if node.eow == true push back to sibling words
    //if node ptrs all point to null popback explore next branch
}

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

//if findSiblings is true perform deeper search that will call printSiblings
void searchTernaryTrie(std::string prefix, bool findSiblings) {
    //iterate through linked lists
    //if prefix[i] == linked list element explore equal
    //if prefix[i] > node.c explore node.right
    //if prefix[i] < node.c explore node.left
    //if == explore node.equal
}

void buildArrTrie(std::string token, TrieNode root) {
    for (char c : token){ 
        //call search on c to see if node is already there if so return 
        //else create new node
        //insert new node
        //if end of token is reached and c is found set c.eow = true    
    }
    TrieNode node;
}

void buildTernaryTrie(std::string token, TernaryNode root) {
    //call search to make sure it's not already there
    //if not there create new node and insert
    //if end of token is reached and c is found set c.eow = true

}


int main() {
    TernaryNode ternRoot;
    TrieNode arrRoot;

    std::string userInput[] = {"Sunny-day", "Meow", "pecan pie", "seventy-five"};

    for (std::string& c : userInput) {
        std::string token = parse(c);
        std::cout << "Token: " << token << std::endl;
    }




    return 0;
}