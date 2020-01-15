#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

void saveTableCode(unordered_map<char, string> code, string codeTableFileName);

void saveCode(string fileName, string codeFileName, unordered_map<char, string> code);

struct Node {
    char ch;
    int freq;
    Node *left, *right;
};

Node *createNode(char ch, int freq, Node *left, Node *right) {
    Node *node = new Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

struct comp {
    bool operator()(Node *l, Node *r) {
        return l->freq > r->freq;
    }
};

void encode(Node *root, string str,
            unordered_map<char, string> &huffmanCode) {
    if (root == nullptr)
        return;

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}


unordered_map<char, int> setFrequency(string fileName) {
    unordered_map<char, int> freq;
    string line;
    ifstream fileToRead(fileName);
    if (fileToRead.is_open()) {
        while (getline(fileToRead, line)) {
            for (char ch: line) {
                freq[ch]++;
            }
            freq['\n']++; //new line char
        }
        fileToRead.close();
        freq['\n']--; //removing last new line char
    } else {
        cout << "Something wrong with file";
    }

    return freq;
}

void runHuffman(string fileName, string codeTableFile, string codeFile) {
    unordered_map<char, int> freq = setFrequency(fileName);

    priority_queue<Node *, vector<Node *>, comp> pq;

    for (auto pair: freq) {
        cout << pair.first << ": " << pair.second << endl;
        pq.push(createNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1) {
        // 2 elementy z najmniejsza czestotliwoscia
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        int sum = left->freq + right->freq;
        pq.push(createNode('\0', sum, left, right));
    }

    Node *root = pq.top();

    //from node to map
    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);

    saveTableCode(huffmanCode, codeTableFile);

    saveCode(fileName, codeFile, huffmanCode);
}

void saveTableCode(unordered_map<char, string> code, string codeTableFileName) {
    ofstream codeTableFile;
    codeTableFile.open(codeTableFileName);
    for (auto pair: code) {
        if (pair.first == '\n') {
            codeTableFile << "\\n " << pair.second << '\n';
        } else {
            codeTableFile << pair.first << " " << pair.second << '\n';
        }
    }
    codeTableFile.close();
}

void saveCode(string fileName, string codeFileName, unordered_map<char, string> code) {
    string line;
    ifstream fileToRead(fileName);

    ofstream codeFile;
    codeFile.open(codeFileName);

    if (fileToRead.is_open() && codeFile.is_open()) {
        while (getline(fileToRead, line)) {
            for (char ch: line) {
                codeFile << code[ch];
            }
            codeFile << code['\n']; //new line char
            //codeFile << '\n';
        }
        codeFile.close();
        fileToRead.close();
    } else {
        cout << "Something wrong with files";
    }
}


int main() {
    string file = "C:\\Users\\abc\\workspace\\algorithms\\Lab5-Huffman\\text.txt";
    string codeTable = "C:\\Users\\abc\\workspace\\algorithms\\Lab5-Huffman\\codeTable.txt";
    string code = "C:\\Users\\abc\\workspace\\algorithms\\Lab5-Huffman\\code.txt";
    runHuffman(file, codeTable, code);

    return 0;
}