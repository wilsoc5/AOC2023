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
    while(getline(in,line)){
        if (!line.empty()){
            m.push_back(line);
        }
    }
    return m;
}

template <typename BeginIterator, typename EndIterator>
string shiftIt(BeginIterator start, EndIterator stop){
    string s (start,stop);
    auto end = std::end(s);
    auto open = std::begin(s);
    while (*open != '.') ++open;

    for (auto it = open; it != end; ++it){
        if (*it == '#'){
            open = it+1;
        }
        if (*it == 'O'){
            std::swap(*open,*it);
            open++;
        }
    }
    return s;
}

string shift(string s){
    auto end = std::end(s);
    auto open = std::begin(s);
    while (*open != '.') ++open;

    for (auto it = open; it != end; ++it){
        if (*it == '#'){
            open = it+1;
        }
        if (*it == 'O'){
            std::swap(*open,*it);
            open++;
        }
    }
    return s;
}

string rshift(string s){
    string r = shiftIt(s.rbegin(), s.rend());
    std::reverse(std::begin(r), std::end(r));
    return r;
}

size_t computeSum(map_t mc, bool quiet = false){
    size_t sum = 0;
    for (int i = 0; i < mc.size(); ++i){
        auto c = std::count(std::begin(mc[i]), std::end(mc[i]), 'O');
        auto weight = mc.size() - i;
        auto val = c * weight;
        sum += val;
        if (!quiet)
            cout <<mc[i]  << " | Count " <<c << " | Weight " <<weight <<" | Val: " <<val <<" | Sum: " <<sum<<endl;
    }
    return sum;
}

map_t cycle(map_t m){
    //north:
    auto n = transpose(m);
    std::transform(std::begin(n), std::end(n),std::begin(m), shift);
    m = transpose(m);
    //cout <<"North\n" <<m <<endl;
    std::transform(std::begin(m), std::end(m),std::begin(m), shift);
    //cout <<"West\n" <<m <<endl;
    auto s = transpose(m.begin(), m.end());
    std::transform(std::begin(s), std::end(s),std::begin(m), rshift);
    m = transpose(m.begin(), m.end());
    //cout <<"South\n" <<m <<endl;
    std::transform(std::begin(m), std::end(m),std::begin(m), rshift);
    //cout <<"East\n" <<m <<endl;

    return m;
}

size_t p2(map_t map, size_t count){
    map_t n;
    size_t cycle_count = 0;
    std::vector<size_t> lc;
    while (count-- != 0){
        ++cycle_count;
        n = cycle(map);
        lc.push_back(computeSum(n, true));
        if (n == map)
            break;
        else{
            map = std::move(n);
        }
        cout <<"Cycle Count : " <<cycle_count <<"\t Sum " <<lc.back() <<endl;
    }
    cout <<"Ran " <<cycle_count <<" cycles.\n";
    return computeSum(map);
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
    cout <<notes;
    
    auto map = transpose(notes);
    map_t shiftedm;
    std::transform(std::begin(map), std::end(map),std::back_inserter(shiftedm), shift);
    cout <<"\n==========\n";
    for (int i = 0; i < map.size(); ++i){
    cout <<"\n==========\n";
    cout <<map[i] <<endl <<shiftedm[i] <<endl;
    }
    
    cout <<"\n==========\n";
    cout <<"\n==========\n";
    auto mc = transpose(shiftedm.begin(), shiftedm.end());
    auto sum = computeSum(mc);
    cout <<"P1 Sum: " <<sum<<endl;
    
    size_t p2val = 0;
    p2val = p2 (mc, p2count); // Find limit cycle in output. then offset
    cout <<"P2 val: " <<p2val <<endl;
    return 0;
}




