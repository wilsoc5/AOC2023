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

struct lens_t {
    string label;
    int val;
    char op;
};

bool operator==(const lens_t& a, const lens_t& b){
    return a.label == b.label;
}
ostream& operator<<(ostream& os, lens_t l){
    return os <<"{'" <<l.label <<"', " <<l.val <<", '" <<l.op<<"'}";
}

typedef std::map<uint8_t,vector<lens_t>> hashmap_t;

ostream& operator<<(ostream& os, const hashmap_t& hm){
    for (auto it = hm.begin(); it != hm.end(); ++it){
        os <<"Box [" <<(int)it->first <<"]: "; 
        const auto& v = it->second;
        for (const auto& l : v){
            os <<" {"<<l.label <<" " <<l.val <<"}";
        }
        os <<endl;
    }
    return os;
}

lens_t convert(string line){
    auto loc = line.find('=');
    if (loc != string::npos){
        auto label = line.substr(0,loc);
        auto val = stoi(line.substr(loc + 1));
        return {label, val, '='};
    }
    loc = line.find('-');
    if (loc != string::npos){
        auto label = line.substr(0,loc);
        return {label, 0, '-'};
    }
    cerr <<"Could not convert '" <<line <<"' to lens_t." <<endl;
    return {};
}

hashmap_t binit(map_t instr){
    hashmap_t hm;
    for (int i = 0; i < instr.size(); ++i){
        auto line = instr[i];
        lens_t l = convert(line);
        auto hashval = hashf(l.label); 
        auto loc = line.find('=');
        if (loc != string::npos){
            //Put into bin.
            auto& v = hm[hashval];
            auto it = find(v.begin(), v.end(), l);
            if (it == v.end()){
                cout <<"Box ["<<(int)hashval <<"]: Adding "<< l <<endl;
                hm[hashval].push_back(l);
            } else {
                cout <<"Box ["<<(int)hashval <<"]: Updating "<< l <<endl;
                *it = l;
            }
        } else {
            loc = line.find('-');
            if (loc != string::npos){
                cout <<"Box ["<<(int)hashval <<"]: Removing" << l <<endl;
                auto& v = hm[hashval];
                auto it = v.erase(std::remove(v.begin(), v.end(), l), v.end());
            }
        }
    }
    return hm;
}

size_t sumHM(hashmap_t hm){
    size_t sum = 0;
    for (auto it = hm.begin(); it != hm.end(); ++it){
        size_t boxval = (int)it->first + 1; 
        size_t slot_val = 1;
        size_t power = 0;
        const auto& v = it->second;
        for (const auto& l : v){
            size_t focal_length = l.val;
            power = boxval * slot_val * focal_length;
            ++slot_val;
            sum += power;
        }
    }
    return sum;
}

size_t p2(map_t instr, vector<uint8_t> hashes){
    hashmap_t hmap = binit(instr);
    cout <<"\n=======================\n";
    cout <<hmap;
    size_t sum = sumHM(hmap);
    return sum;
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
    vector<uint8_t> hashes;
    for (auto n : notes){
       h = hashf(n); 
       cout <<n <<" | " <<(size_t)h <<endl;
       sum += (size_t)h;
       hashes.push_back(h);
    }
    //cout <<notes <<"==========\n";
    cout <<"\n==========\n";
    cout <<"P1 Sum: " <<sum<<endl;
    
    size_t p2val = p2 (notes, hashes); // Find limit cycle in output. then offset
    cout <<"P2 val: " <<p2val <<endl;
    return 0;
}




