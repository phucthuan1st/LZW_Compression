#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <bitset>
#include <iomanip>
#include <string>


#define BITS 10

using namespace std;

//Hàm chuẩn hóa đường dẫn đến file
string Convert_path(const char* str) {
   
    string temp = str;
    int n = temp.length();
    
    for (int i = 0; i < n; i++)
    {
        if (temp[i] == '\\') {
            temp.erase(i, 1);
            temp.insert(i, "//");
            i++;
            n++;
        }
    }
    
    return temp;
}

//Hàm tách đường dẫn để lấy tên file input
string SplitFileName(const char* str) {
    
    string temp = str;
    
    for (int i = temp.length() - 1; i >= 0; i--)
    {
        if (temp[i] == '\\')
        {
            return temp.substr(i + 1);
        }
    }
}

//Hàm xuất từ điển và trả về dãy mã hóa
vector<int> encodingMode(string s,string outputInfo)
{
    //Nếu mode là -d hoặc -ind thì tạo từ điển rồi xuất từ điển ra màn hình
    if (outputInfo == "-d" || outputInfo == "-ind") 
    {  cout << "\nAddition\n";
       
        //Khai báo bảng table lưu từ điển
        map<string, int> table;

        //Mã hóa và lưu từ điển
        for (int i = 0; i <= 255; i++) {
            string ch = "";
            ch += char(i);
            table[ch] = i;
        }

        string p = "", c = "";
        int code = 256;

        p += s[0];
      
        //Vector lưu dãy mã hóa 
        vector<int> coding;

        for (int i = 0; i < s.length(); i++)
        {
            if (i != s.length() - 1)
            {
                c += s[i + 1];
            }

            if (table.find(p + c) != table.end())
            {
                p = p + c;
            }
            else {
                coding.push_back(table[p]);
                table[p + c] = code;
                cout << p + c << ":" << table[p + c] << endl;
                code++;
                p = c;
            }
            c = "";
        }
        coding.push_back(table[p]);

        return coding;
    }
    //Nếu mode là -i thì không cần xuất từ điển ra màn hình
    else if (outputInfo == "-i")
    {
        //Khai báo bảng table lưu từ điển
        map<string, int> table;
        for (int i = 0; i <= 255; i++) {
            string ch = "";
            ch += char(i);
            table[ch] = i;
        }
        string p = "", c = "";
        p += s[0];
        int code = 256;

        //Vector lưu dãy mã hóa 
        vector<int> coding;

        //Mã hóa và lưu từ điển
        for (int i = 0; i < s.length(); i++)
        {
            if (i != s.length() - 1)
            {
                c += s[i + 1];
            }

            if (table.find(p + c) != table.end())
            {
                p = p + c;
            }
            else {
                coding.push_back(table[p]);
                table[p + c] = code;
                code++;
                p = c;
            }
            c = "";
        }
        coding.push_back(table[p]);

        return coding;
    }  
}

//Hàm chuyển số thập phân sang nhị phân theo số bit quy định
string DecToBin(int code, int number_of_bit) 
{
    string result;
    //Chuyển thập phân sang nhị phân
    while (code > 0) {
        result = to_string(code % 2) + result;
        code /= 2;
    }

    //Nếu chuỗi nhị phân chưa đủ số bit quy định thì phải bù bit
    if (result.size() < number_of_bit) {
        int old_size = result.size();
        for (int i = 0; i < number_of_bit - old_size; i++) {
            result = '0' + result;
        }
    }

    return result;
}

//Hàm mã hoá dãy mã thành nhị phân
string encode(vector<int> compress_code)
{
    string bin_code;

    for (int i = 0; i < compress_code.size(); i++) {
        bin_code += DecToBin(compress_code[i], BITS);
    }
    
    return bin_code;
}

//Ghi file nén 
void writeFile(const char* outputPath, string full_code, string filename)
{
    //Tạo file output
    filename = filename.substr(0, filename.size() - 4);
    filename += ".lwz";
   
    //Tạo đường dẫn đến file output
    string path_of_outputfile = Convert_path(outputPath);
    path_of_outputfile += "//" + filename;

    //Ghi file bằng bitset
    fstream fout(path_of_outputfile, ios::out | ios::binary);
    
    for (int i = 0; i < full_code.size(); i += 8) {
        bitset<8> bit(full_code.substr(i, 8));
        fout.write((char*)&bit, 1);
    }

    fout.close();
}

//Hàm bù bit (trường hợp số byte cần ghi bị lẻ)
void addMissBit(string& full_code)
{
    short enough_byte = full_code.size() / 8;
    short missed_bit = 0;
    size_t enought_bit = full_code.size();

    //Tính số bit thiếu
    if (full_code.size() % 8 != 0) {
        enough_byte = full_code.size() / 8 + 1;
        enought_bit = enough_byte * 8;
        missed_bit = enought_bit - full_code.size();
    }

    //Bù bit vào cuối dãy mã nhị phân 
    for (int i = 0; i < missed_bit; i++) {
        full_code += '0';
    }
}

//Hàm nén file
void compressFile(const char* inputPath, const char* outputPath, string outputInfo)
{
    //Chuẩn hóa đường dẫn đến file input
    string path_of_inputFile = Convert_path(inputPath);

    //Tách lấy tên file input
    string filename = SplitFileName(inputPath);
    
    //Đọc file input
    fstream fin(path_of_inputFile, ios::in);
    if (!fin.is_open())
    {
        cout << "Unable to open file!" << endl;
        exit(0);
    }

    string s;
    string code;
    int size = 0;
    while (getline(fin, s, '\n'))
    {
        s = s + "\n";
        size++;
        code = code + s;
        s = "";
    }
    //Xóa kí tự enter cuối cùng
    code.pop_back();

    //Tính size file input
    size = size + code.size()-1;
    size = size * 8;
    
    //Mã hóa file input
    vector<int> en = encodingMode(code,outputInfo);
    
    //Chọn mode của outputinfo
    if (outputInfo == "-d")
    {
        //mã hóa dãy mã thành chuỗi nhị phân
        string full_code = encode(en);

        //Bù bit
        addMissBit(full_code);

        //Ghi file nén
        writeFile(outputPath, full_code, filename);
    }
    else if (outputInfo == "-i"||outputInfo=="-ind")
    {
        cout << "\nInput size:" << size << endl;

        //mã hóa dãy mã thành chuỗi nhị phân
        string full_code = encode(en);

        //Bù bit
        addMissBit(full_code);

        //Xuất ra màn hình size của file input và output
        cout << "Output size:" << full_code.size() << endl;
        float save = ((size - full_code.size())*1.0 / size)*100;
        cout << "Space saved:" << setprecision(2)<< fixed<< save <<"%"<< endl;

        //Ghi file nén
        writeFile(outputPath, full_code, filename);
    }
    else {
        cout << "Output Info does not exist!" << endl;
    }

    fin.close();  
}

//Hàm chuyển nhị phân sang thập phân
int BinToDec(int binaryNumber)
{
    int p = 0;
    int decNumber = 0;

    while (binaryNumber > 0)
    {
        decNumber += (binaryNumber % 10) * pow(2, p);
        ++p;
        binaryNumber /= 10;
    }

    return decNumber;
}

//Hàm giải mã nhị phân sang thập phân
vector<int> decode(string s)
{
    vector<int> code;

    for (int i = 0; i < s.size(); i += 10)
    {
        int x = BinToDec(stoi(s.substr(i, 10)));
        code.push_back(x);
    }

    return code;
}

//Hàm ghi file giải nén
void writeFileDecode(string result, const char* outputPath, string filename)
{
    //Tạo đường dẫn đến file output
    filename = filename.substr(0, filename.size() - 4);
    filename += ".txt";
    string path_of_outputfile = Convert_path(outputPath);
    path_of_outputfile += "//" + filename;

    //Ghi file
    fstream fout(path_of_outputfile, ios::out);
    if (!fout.is_open())
        exit(0);
    fout << result;

    fout.close();
}

//Hàm giải mã thập phân thành chuỗi kí tự để ghi vào file output và xuất ra màn hình định dạng theo mode outputinfo
void decodingMode(const char* outputPath,string filename,vector<int> op,int sizeinput ,string outputInfo)
{
    //Bảng lưu từ điển
    map<int, string> table;

    //Tạo từ điển và lưu từ điển
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[i] = ch;
    }

    int old = op[0], n;
    int count = 256;
    string s = table[old];
    string c = "";
    string result;

    c += s[0];
    result += s;

    for (int i = 0; i < op.size() - 1; i++)
    {
        n = op[i + 1];

        if (table.find(n) == table.end()) {
            s = table[old];
            s = s + c;
        }
        else {
            s = table[n];
        }
        result += s;
        c = "";
        c += s[0];
        table[count] = table[old] + c;
        count++;
        old = n;
    }
    cout << endl;

    //Nếu mode outputinfo là -d hoặc -ind thì xuất từ điển ra màn hình
    if (outputInfo == "-d" || outputInfo == "-ind")
    {
        cout << "Addition\n";
        map<int, string>::iterator it;

        for (it = table.begin(); it != table.end(); it++)
        {
            if (it->first > 255)
            {
                cout << it->first << ":" << it->second << endl;
            }
        }
    }
    //Nếu mode outputinfo là -i hoặc -ind thì xuất kích thước của file input,output
     if (outputInfo == "-i" || outputInfo == "-ind")
    {
         //Đếm số dòng enter ghi ra file để tính size
        int countEnter = 0;

        for (int i = 0; i < result.size(); i++)
        {
            if (result[i] == '\n')
            {
                countEnter++;
            }
        }
        
        cout << "\nInput Size:" << sizeinput << endl;
        cout << "Output Size:" << result.size() * 8 + countEnter * 8 << endl;
        float spaceIncreased = ((result.size() * 8 + countEnter * 8 - sizeinput) * 1.0 / sizeinput) * 100;
        cout << "Space Increased:" << setprecision(2) << fixed << spaceIncreased << "%" << endl;
    }

     //Ghi file giải  nén
     writeFileDecode(result, outputPath, filename);
}

//Hàm giải nén
void decompress(const char* inputPath, const char* outputPath, string outputInfo)
{
    //Chuẩn hóa đường dẫn đến file input
    string path_of_inputFile = Convert_path(inputPath);

    //Tách đường dẫn để lấy tên file input
    string filename = SplitFileName(inputPath);

    //Đọc file input sử dụng bitset
    fstream file(path_of_inputFile, ios::in | ios::binary);
   
    string result = "";
    bitset<8> bit;
    while (file.read((char*)&bit, 1)) {
        result += bit.to_string();
    }

    //Tính size của file input
    int sizeinput = result.size();
    //Tính số bit thừa trong file
    int sizebyte = result.size() / BITS;
    int sizebits = sizebyte * BITS;

    //Cắt những bit thừa ra khỏi dãy mã nhị phân  
    result = result.substr(0, sizebits);
    
    //Chuyển dãy mã nhị phân thành dãy mã thập phân
    vector<int> code = decode(result);

    //Giải nén dãy mã thập phân rồi ghi vào file output 
    decodingMode(outputPath, filename, code, sizeinput, outputInfo);
}

//Hàm chạy các tham số từ commandline
void runCommandline(const char* argv[])
{
    string action = argv[1];
    const char* inputPath = argv[2];
    const char* outputPath = argv[3];
    string outputInfo = argv[4];

    //Nếu action=="-e" thì ta thực hiện việc nén file
    if (action == "-e")
    {
        compressFile(inputPath, outputPath, outputInfo);
    }
    //Nếu action=="-d" thì ta thực hiện việc giải nén file
    else if (action == "-d")
    {
        decompress(inputPath, outputPath, outputInfo);
    }
    else {
        cout << "Action does not exist!" << endl;
    }
}

//Hàm main
int main(int argc, const char* argv[])
{
    //Không đủ tham số đầu vào
    if (argc != 5)
        cout << " No Enough agrc" << endl;
    else {
        runCommandline(argv);
    }
    return 1;
}
