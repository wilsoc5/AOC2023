#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <map>
#include <iterator>

using namespace std;

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v){
    bool first = true;
    for (const auto& vi : v){
        if (!first) os <<"\n";
        os <<vi ;
        first = false;
    }
    return os;
}

template<typename T, typename U>
ostream& operator<<(ostream& os, const pair<T,U>& p){
    os <<"{ " <<p.first <<", " <<p.second <<" }";
    return os;
}

template<typename T>
void print(vector<vector<T>> a){
    for (auto& b: a){
        cout <<b <<endl;
    }
}

typedef vector<string> map_t;

template <typename bit, typename eit>
map_t transpose(bit begin, eit end){
    map_t out;
    auto orig_rowsize = std::distance(begin,end);
    auto orig_colsize = begin->size();
    for (auto col = 0; col < orig_colsize; ++col){
        string s;
        for (auto row = begin; row < end; ++row){
            s.push_back((*row)[col]);
        }
        out.push_back(s);
    }
    return out;
}

map_t transpose(const map_t& m ){
    return transpose(m.cbegin(), m.cend());
}

map_t parse_input(ifstream& in){
    string line;
    map_t m;
    while(getline(in,line,',')){
        if (!line.empty()){
            if (line[0] == '\n')
                continue;
            line.erase(std::remove_if(line.begin(),
                              line.end(),
                              [](unsigned char x) { return std::isspace(x); }),
                              line.end());
            m.push_back(line);
        }
    }
    return m;
}

uint8_t hashf(string s){
    size_t output = 0;
    for (auto c : s){
        output += c;
        output *= 17;
        output = output % 256;
    }
    if (output > 255){
        cerr <<"Error with output: " <<output <<endl;
    }
    return output;
}

int main(int argc, char* argv[]){
    using std::begin, std::end;
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file> " <<endl;
        exit(1);
    }
    cout <<"Running: " ;
    for (int i = 0; i < argc; ++i){
        cout << argv[i] <<" ";
    }
    cout <<endl;
    size_t p2count = 3;
    if (argc > 2)
        p2count = stoi(argv[2]);
    std::ifstream in(argv[1]);
    auto notes = parse_input(in);
    cout <<"Read file: " <<endl;
    size_t sum = 0;
    uint8_t h = 0;
    for (auto n : notes){
       h = hashf(n); 
       cout <<n <<" | " <<(size_t)h <<endl;
       sum += (size_t)h;
    }
    //cout <<notes <<"==========\n";
    cout <<"\n==========\n";
    cout <<"P1 Sum: " <<sum<<endl;
    
    size_t p2val = 0;
    //p2val = p2 (mc, p2count); // Find limit cycle in output. then offset
    cout <<"P2 val: " <<p2val <<endl;
    return 0;
}




