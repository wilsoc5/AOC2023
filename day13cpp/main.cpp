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
        if (!first) os <<", ";
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

struct note_t{
    vector<string> lines;
    bool ishoriz;
    int location = -1;
    vector<int> possible_col_locations;
    vector<int> possible_row_locations;
    int value = 0;
};
typedef vector<note_t> notes_t;

note_t operator+(const note_t& lhs, const note_t& rhs){
    note_t n;
    n.value = lhs.value + rhs.value;
    return n;
}

ostream& operator<<(ostream& os, const note_t& n){
    for (auto& l : n.lines){
        os <<l<<endl ;
    }
    os <<" ishoriz: " <<boolalpha <<n.ishoriz <<" \t" <<n.location <<endl;
    os <<" prl: " <<n.possible_row_locations <<endl;
    os <<" pcl: " <<n.possible_col_locations <<endl;
    os <<" value: " <<n.value <<endl;
    return os;
}

bool check_match(string line, int col_split){
    auto s1 = line.substr(0,col_split);
    auto s2 = line.substr(col_split);
    auto s1i = s1.rbegin();
    auto s2i = s2.begin();
    bool match=true;
    while (!((s1i == s1.rend() || s2i == s2.end()))){
        if (*s1i++ != *s2i++){
            match = false;
            break;
        }
    }
    return match;
}

bool check_match_rows(vector<string> lines, int row_split){
    auto it = lines.begin();
    std::advance(it, row_split);
    auto rit = std::make_reverse_iterator(it);
    bool match = true;
    while (!((it == lines.end() || rit == lines.rend()))){
        if (*it++ != *rit++){
            match = false;
            break;
        }
    }
    return match;
}
bool check_rows(note_t& n){
    for (auto it = std::begin(n.possible_row_locations); it != std::end(n.possible_row_locations); ){
        bool match = check_match_rows(n.lines,*it);
        if (match) ++it;
        else it = n.possible_row_locations.erase(it);
    }
}

notes_t parse_input(ifstream& in){
    string line;
    notes_t ns;
    note_t n;
    auto finish = [&](){
            check_rows(n);
            if (n.possible_col_locations.size() > 0){
                n.ishoriz = false;
                if (!(n.possible_col_locations.empty())){
                    n.location = n.possible_col_locations.front();
                    n.value = n.location;
                }
            } else {
                n.ishoriz = true;
                if (!(n.possible_row_locations.empty())){
                    n.location = n.possible_row_locations.front();
                    n.value = n.location * 100;
                }
            }
            ns.push_back(n);
            n = note_t{} ;
    };
    bool run_last = false;
    while(getline(in,line)){
        if (!line.empty()){
            if (!n.lines.empty()){
                if (n.lines.back() == line){
                    n.possible_row_locations.push_back(int(n.lines.size()));
                }
            }
            n.lines.push_back(line);
            if (n.lines.size() == 1){
                for (int i = 1; i < line.size() - 1 ; ++i){
                    bool match = check_match(line,i);
                    if (match)
                        n.possible_col_locations.push_back(i);
                }
            } else {
                for (auto it = std::begin(n.possible_col_locations); it != std::end(n.possible_col_locations); ){
                    bool match = check_match(line,*it);
                    if (match) ++it;
                    else it = n.possible_col_locations.erase(it);
                }
            }
            run_last = true;
        } else {
            finish();
            run_last = false;
        }
    }
    if(run_last){
        finish();
    }
    
    cout <<in.good() <<endl;
    return ns;
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
    std::ifstream in(argv[1]);
    auto notes = parse_input(in);
    cout <<"Read file: " <<endl;
    cout <<notes;
    
    
    note_t n;
    auto sum = std::accumulate(std::begin(notes), std::end(notes),n);
    cout <<"Sum: " <<sum <<endl;

    return 0;
}




