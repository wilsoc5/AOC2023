#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>

using namespace std;

struct coord_t {
    int row;
    int col;
};

struct number_t{ 
    int64_t num = -1;
    coord_t start = {-1,-1};
    coord_t end = {-1, -1};
    bool pn = false;
};
struct symbol_t {
    char sym = '0';
    coord_t pos = {-1,-1};
};

bool operator<(const symbol_t a, const symbol_t b){
    if (a.pos.row == b.pos.row) return a.pos.col < b.pos.col;
    return a.pos.row < b.pos.row;
}

std::ostream& operator<<(std::ostream& os, const coord_t c){
    os <<"{ " <<c.row <<", " <<c.col <<"}";
    return os;
}
std::ostream& operator<<(std::ostream& os, const symbol_t s){
    os <<"{ " <<s.sym <<", " <<s.pos <<"}";
    return os;
}
std::ostream& operator<<(std::ostream& os, const number_t n){
    os <<"{ " <<n.num<<", " <<n.start <<", " <<n.end <<", " << n.pn<<"}";
    return os;
}

std::vector<symbol_t> symbols;
std::vector<number_t> numbers;
std::map<symbol_t,vector<number_t>> gears;

void parse_line (string line, int row){
    auto start_it = std::find_if(line.begin(),line.end(),[](const char c) { return c != '.'; });
    auto end_it = std::find_if(start_it+1,line.end(),[](const char c) { return !isdigit(c); });
    auto start_pos = std::distance(line.begin(), start_it);
    auto stop_pos = end_it == line.end()? line.size() : std::distance(line.begin(), end_it);
    while (start_it != line.end()){
        auto substr_count = stop_pos - start_pos;
        substr_count = std::max(substr_count,1UL);
        auto sub = line.substr(start_pos, substr_count);
        //cout <<"PL: " <<sub <<endl;
        if (isdigit(sub[0])){
            number_t n;
            n.num = stoi(sub);
            n.start.row = row;
            n.start.col = start_pos;
            n.end.row = row;
            n.end.col = stop_pos-1;
            numbers.push_back(n);
            if (!isdigit(sub.back())){
                substr_count -=1;
            }
        } else {
           symbol_t s;
           s.sym = sub[0];
           s.pos.row = row;
           s.pos.col = start_pos;
           symbols.push_back(s);
           if (isdigit(sub.back())){
                number_t n;
                sub.erase(0,1);
                n.num = stoi(sub);
                n.start.row = row;
                n.start.col = start_pos+1;
                n.end.row = row;
                n.end.col = stop_pos-1;
                numbers.push_back(n);
                if (!isdigit(sub.back())){
                    substr_count -=1;
                }
           }
        }
        start_it = std::find_if(start_it+substr_count,line.end(),[](const char c) { return c != '.'; });
        end_it = std::find_if(start_it+1,line.end(),[](const char c) { return !isdigit(c); });
        start_pos = std::distance(line.begin(), start_it);
        stop_pos = end_it == line.end()? line.size() : std::distance(line.begin(), end_it);
    }
}

vector<symbol_t> find_symbols_near_row(int row){
       vector<symbol_t> valid_symbols;
       auto start = std::stable_partition(symbols.begin(), symbols.end(), [row](symbol_t s){ return s.pos.row < row - 1; });
       auto end = std::stable_partition(symbols.begin(), symbols.end(), [row](symbol_t s){ return s.pos.row <= row + 1; });
       std::copy(start,end,std::back_inserter(valid_symbols));
       return valid_symbols;
}
vector<symbol_t> find_symbols_near_cols(std::vector<symbol_t> syms, int start_col, int end_col){
       vector<symbol_t> valid_symbols;
       auto start = std::stable_partition(syms.begin(), syms.end(), [start_col](symbol_t s){ return s.pos.col < start_col - 1; });
       auto end = std::stable_partition(syms.begin(), syms.end(), [end_col](symbol_t s){ return s.pos.col <= end_col + 1; });
       std::copy(start,end,std::back_inserter(valid_symbols));
       return valid_symbols;
}

int main(int argc, char* argv[]){
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    string line;
    uint64_t line_count = 0;
    uint64_t game_number;
    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        //cout <<"========\n" <<line <<endl;
        parse_line(line, line_count);
        ++line_count;
    }
    //cout <<"Symbols: " <<endl;
    //for (const auto& s : symbols){
    //    cout <<"\t" <<s <<endl;
    //}
    //cout <<"Numbers: " <<endl;
    //for (const auto& n : numbers){
    //    cout <<"\t" <<n <<endl;
    //}

    auto s2 = find_symbols_near_row(1);
    //cout <<"find_symbols_near_row(2):\n";
    //for (const auto& s : s2){
    //    cout <<"\t" <<s <<endl;
    //}
    auto s3= find_symbols_near_cols(s2, 0, 2);
    //cout <<"find_symbols_near_cols(0,2):\n";
    //for (const auto& s : s3){
    //    cout <<"\t" <<s <<endl;
    //}

    uint64_t pnsum = 0;
    for (auto& n : numbers){
        //cout <<"Running number: " <<n <<endl;
        int row = n.start.row;
        auto s2 = find_symbols_near_row(row);
        //cout <<"find_symbols_near_row(" <<row<<"):\n";
        //for (const auto& s : s2){
        //    cout <<"\t" <<s <<endl;
        //}
        int sc = n.start.col;
        int ec = n.end.col;
        auto s3= find_symbols_near_cols(s2, sc, ec);
        //cout <<"find_symbols_near_cols("<<sc <<", " <<ec <<"):\n";
        for (const auto& s : s3){
            //cout <<"\t" <<s <<endl;
            if (s.sym == '*'){
                gears[s].push_back(n);
            }
        }
        n.pn = !(s3.empty());
        if (n.pn){
            pnsum += n.num;
        }
    }
    //cout <<"Numbers: " <<endl;
    //for (const auto& n : numbers){
    //    cout <<"\t" <<n <<endl;
    //}
    cout <<"PN Sum: " <<pnsum <<endl;

    //cout <<" Gears: " <<endl;
    for (auto g = gears.begin(); g != gears.end(); ){
        //cout <<"\t" <<g->first << ": " <<endl;
        auto gear_count = g->second.size();
        //for (const auto& s : g->second){
        //    cout <<"\t\t" <<s <<endl;
        //}
        if (gear_count == 1){
            g = gears.erase(g);
            //cout << "\t\t not a gear."<<endl;
        } else {
            ++g;
        }
    }
   
    uint64_t gear_sum = 0;
    for (const auto& g : gears){
        uint64_t gear_product = 1;
        for (const auto& n : g.second){
            gear_product *= n.num;
        }
        gear_sum += gear_product;
        //cout <<"\tGear product: " <<gear_product <<endl;
        //cout <<"\tGear sum: " <<gear_sum <<endl;
    }

    cout <<"Gear sum: " <<gear_sum <<endl;

    return 0;
}
