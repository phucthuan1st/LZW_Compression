#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <bitset>

using namespace std;
#define BIT_PER_CODE 10

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

string DecToBin(short code, int number_of_bit) {
    string result;
    while (code > 0) {
        result = to_string(code % 2) + result;
        code /= 2;
    }

    if (result.size() < number_of_bit) {
        short old_size = result.size();
        for (int i = 0; i < number_of_bit - old_size; i++) {
            result = '0' + result;
        }
    }

    return result;
}

void CompressFile(string filename, vector<short> compress_code) {
    
    string bin_code;

    //compress code to binary format
    for (int i = 0; i < compress_code.size(); i++) {
        bin_code += DecToBin(compress_code[i], BIT_PER_CODE);
    }

    //calculate for padding size
    short enough_byte = bin_code.size() / 8;
    short missed_bit = 0;
    size_t enought_bit = bin_code.size();
    if (bin_code.size() % 8 != 0) {
        enough_byte = bin_code.size() / 8 + 1;
        enought_bit = enough_byte * 8;
        missed_bit = enought_bit - bin_code.size();
    }
    for (int i = 0; i < missed_bit; i++) {
        bin_code += '0';
    }

    //write bitset to file
    fstream fout(filename, ios::out | ios::binary);
    for (int i = 0; i < enought_bit; i += 8) {
        bitset<8> bit(bin_code.substr(i, 8));
        fout.write((char*)&bit, 1);
    }

    fout.close();
}

vector<string> readCompressionFile(const char* filename) {
    fstream file(filename, ios::in | ios::binary);
    vector<string> result;


    return result;
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
    CompressFile("Compressed.lzw", compress_code);

    return 0;
}