#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>

using namespace std;

map<std::string, int> word;
static int table_size = 255;

void InitializeWord() {
    for (int i = 0; i < 256; i++) {
        string ch = "";
        ch += (char)i;
        word[ch] = i;
    }
}

vector<short> encode(string source) {
    vector<short> compress_code;
    string p;
    p = source[0];
    int i = 0;
    string c;


    while (i < source.length()) {
        c = source[i + 1];
        if (word.find(p + c) != word.end()) {
            p += c;
        }
        else {
            compress_code.push_back(word[p]);
            word[p + c] = ++table_size;
            p = c;
        }
        i++;
    }

    return compress_code;
}

string decode(vector<short> compress_code) {
    string result = "";


    return result;
}

int main() {
    InitializeWord();
    vector<short> compress_code = encode("WYS*WYGWYS*WYSWYSG");
    for (int i = 0; i < compress_code.size(); i++) {
        cout << compress_code[i] << " ";
    }
    cout << "\n=====================\n";
    map<string, int>::iterator it;
    for (it = word.begin(); it != word.end(); it++) {
        if (it->second > 255)
            cout << it->first.c_str() << ": " << it->second << endl;
    }
    return 0;
}