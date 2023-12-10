#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <map>

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

void print(vector<vector<int64_t>> a){
    for (auto& b: a){
        cout <<b <<endl;
    }
}

int main(int argc, char* argv[]){
    using std::begin, std::end;
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    string line;
    vector<int64_t> next_vals; 
    vector<int64_t> prev_vals; 
    while(getline(in,line)){
        vector<vector<int64_t>> arrays;
        vector<int64_t> inputvals;
        vector<int64_t> offsets;
        uint64_t lc = 100;
        stringstream ss(line);
        int iv;
        while (ss){
            ss >>iv;
            if (ss) inputvals.push_back(iv);
        }
        uint64_t i = 0;
        arrays.push_back(inputvals);
        bool allzero = true;
        do{
            inputvals.clear();
            std::adjacent_difference(arrays[i].begin(), arrays[i].end(), std::back_inserter(inputvals));
            inputvals[i] = 0;
            arrays.push_back(inputvals);
            ++i; //before all_of
            allzero = std::all_of(inputvals.begin()+i, inputvals.end(), [](auto a){ return a == 0;});
            offsets.push_back(i);
        }while((!allzero) && (--lc > 0));

        
        cout <<"Product:\n ";
        for (const auto& a : arrays){
            cout <<"\t" <<a  <<endl;
        }
        cout <<"\n============\n";
        cout <<"Offsets: \n";
        cout <<offsets ;
        cout <<"\n============\n";
        auto old_it = arrays.rbegin();
        auto depth = arrays.size();
        auto offit = offsets.rbegin();
        int64_t frontval = 0;
        cout <<"Arrays size: " <<arrays.size() <<endl;
        i = arrays.size() - 1; 
        for (auto it = arrays.rbegin()+1; it != arrays.rend(); ++it){
            cout <<"it: " <<*it <<endl; 
            auto val = it->back() + old_it->back();
            it->push_back(it->back() + old_it->back());
            frontval = (*it)[*offit - 1] - (*old_it)[*offit];
            cout <<"\t f:" <<(*it)[*offit - 1] <<"\t s:" <<(*old_it)[*offit] <<endl;
            (*it)[*offit-1] = frontval;
            cout <<"\t\t" <<*it <<"\n";
            ++old_it;
            ++offit;
        }
        next_vals.push_back(arrays[0].back());
        prev_vals.push_back(frontval);
        print(arrays);
    }
    cout <<"Product: " <<next_vals<<endl;
    cout <<"Sum: " <<std::accumulate(next_vals.begin(), next_vals.end(), 0) <<endl;
    cout <<"P2Sum: " <<std::accumulate(prev_vals.begin(), prev_vals.end(), 0) <<endl;
    return 0;
}




