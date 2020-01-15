#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

// A Tree node
struct Node {
    char ch;
    Node *left, *right;
};

Node *createNode(char ch, Node *left, Node *right) {
    Node *node = new Node();

    node->ch = ch;
    node->left = left;
    node->right = right;

    return node;
}

unordered_map<char, string> getTableCode(string fileName) {
    unordered_map<char, string> code;
    string line;
    char codeChar;
    string codeString;
    int lineSize;
    int pos;
    ifstream fileToRead(fileName);

    if (fileToRead.is_open()) {
        while (getline(fileToRead, line)) {
            lineSize = line.size();
            if (lineSize > 0) {
                pos = line.find(' ');
                switch(pos){
                    case 1:
                        codeChar = line.at(0);
                        break;
                    case 2:
                        codeChar = '\n';
                        break;
                    case 0:
                        codeChar = ' ';
                        pos++;
                        break;
                    default:
                        codeChar = line.at(0);
                        break;
                }
                codeString = line.substr(pos + 1);
                code[codeChar] = codeString;
            }
        }
        fileToRead.close();
        cout << "Code table readed" << endl;
//        for (auto& it: code) {
//            cout << it.first << ": "<< it.second << endl;
//        }
    } else {
        cout << "Something wrong with files";
    }
    return code;
}

void saveDecodedChar(string file, char decodedChar) {
    fstream decodeFile;
    decodeFile.open(file, ios::out|ios::app);

    if (decodeFile.is_open()) {
        decodeFile << decodedChar;
        decodeFile.close();
    } else {
        cout << "Something wrong with files";
    }
}

void clearDecodedFile(string file) {
    ofstream clearFile(file);
    if(clearFile.is_open()) {
        cout << "Cleared";
    }
    clearFile.close();
}

void decode(Node *root, int &index, string str, string file) {
    if (root == nullptr) {
        return;
    }

    // znaleziono lisc w drzewie
    if (root->left == nullptr && root->right == nullptr) {
        saveDecodedChar(file, root->ch);
        return;
    }

    index++;

    if (str[index] == '0')
        decode(root->left, index, str, file);
    else
        decode(root->right, index, str, file);
}

string getCodedText(string codedFile, Node *root, string decodedFileName) {
    string line;
    ifstream fileToRead(codedFile);
    if (fileToRead.is_open()) {
        while (getline(fileToRead, line)) {
            int index = -1;
            while (index < (int) line.size() - 2) {
                decode(root, index, line, decodedFileName);
            }
        }
    } else {
        cout << "Something goes wrong with reading coded text";
    }
    return line;
}

void runHuffman(string code, string table, string decoded) {
    unordered_map<char, string> huffmanCode = getTableCode(table);

    priority_queue<Node *, vector<Node *>> pq;

    pq.push(createNode('\0', nullptr, nullptr));

    Node *root;

    for (auto pair: huffmanCode) {
        root = pq.top();
        for (char c : pair.second) {
            if (c == '1') {
                if (root->right == nullptr) {
                    root->right = createNode('\0', nullptr, nullptr);
                }
                root = root->right;
            } else {
                if (root->left == nullptr) {
                    root->left = createNode('\0', nullptr, nullptr);
                }
                root = root->left;
            }
        }
        root->ch = pair.first;
    }

    root = pq.top();
    clearDecodedFile(decoded);
    string str = getCodedText(code, root, decoded);
}

int main() {
    runHuffman(
            "C:\\Users\\abc\\workspace\\algorithms\\Lab5-Huffman\\code.txt",
            "C:\\Users\\abc\\workspace\\algorithms\\Lab5-Huffman\\codeTable.txt",
            "C:\\Users\\abc\\workspace\\algorithms\\Lab5-Huffman\\decoded.txt");
}