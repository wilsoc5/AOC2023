#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>

using namespace std;

struct mapping_t{ 
    uint64_t dest_range_start = 0;
    uint64_t source_range_start = 0;
    uint64_t count = 0;
    uint64_t source_range_end = 0;
    uint64_t dest_range_end = 0;
};

ostream& operator<<(ostream& os, const mapping_t& m){
    os <<"{ [" <<m.dest_range_start <<", " <<m.dest_range_end <<"), ["
    <<m.source_range_start <<", " <<m.source_range_end <<"), " <<m.count <<"}";
    return os;
}

struct map_t{
    string source_name;
    string dest_name;
    vector<mapping_t> mappings;
};

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

ostream& operator<<(ostream& os, const map_t& m){
    os <<"{ " <<m.source_name <<", " <<m.dest_name <<", [ " << m.mappings <<" ]}";
    return os;
}

struct almanac_t{
    vector<uint64_t> seeds;
    vector<map_t> maps ;
};
ostream& operator<<(ostream& os, almanac_t& a){
    os <<"Seeds: [ " <<a.seeds <<" ]\nMaps: [ " <<a.maps <<" ]";;
    return os;
}

std::optional<mapping_t> parse_mapping(istream& in){
    mapping_t m;
    in >> m.dest_range_start;
    in >> m.source_range_start;
    in >> m.count;
    cout <<"\t\t\t\tMapping_t: " <<m;
    if (!in){
        return {};
    }
    m.dest_range_end = m.dest_range_start + m.count;
    m.source_range_end = m.source_range_start + m.count;
    return m;
}

std::optional<map_t> parse_map(istream& in){
    map_t m;
    string name, line;
    in >>name >>line;
    cout <<"PMap:\n\tname: |" <<name <<"| line: |" <<line<<"|" <<endl;
    auto dash1 = name.find('-');
    cout <<"\tdash1: " <<dash1;
    if (dash1 == string::npos){
    }
    m.source_name = name.substr(0,dash1);
    cout <<"\tSource name: " <<m.source_name;
    auto dash2 = name.find('-',dash1+1);
    cout <<"\n\tdash2: " <<dash2;
    m.dest_name = name.substr(dash2+1);
    cout <<"\tdest name: " <<m.dest_name;
    getline(in,line); //eat newline at end of name line;
    getline(in,line); //Grab first mapping line.
    cout <<"\n\tline: |" <<line<<"|"<<endl;
    while (!line.empty()){
        stringstream ss(line);
        auto ming = parse_mapping(ss);
        if (ming){
            m.mappings.push_back(*ming);
            cout <<"\n\t\tMing: " <<*ming <<endl;
        }
        getline(in,line);
    }
    sort(m.mappings.begin(), m.mappings.end(), [](auto a, auto b){ return a.source_range_start < b.source_range_start; });
    return m;
}

vector<uint64_t> parse_seeds(istream& in){
    string seedstr, line;
    string val;
    vector<uint64_t> seeds;
    getline(in,line);
    cout <<"Parse Seeds: ";
    cout <<line<<endl;
    stringstream ss(line);
    ss >> seedstr;
    while (ss){
        ss >> val;
        if (ss){
            seeds.push_back(stoull(val));
        }
    }
    return seeds;
}

almanac_t parse_file(istream& in){
    almanac_t a;
    a.seeds = parse_seeds(in);

    while(in){
        auto m = parse_map(in);
        if (m){
            a.maps.push_back(*m);
        }
        else {
            cout <<"Empty map_t optional" <<endl;
        }
    }
    return a;
}

uint64_t find_dest(map_t m, uint64_t source_val){
    auto it = std::find_if(m.mappings.begin(), m.mappings.end(), [source_val](auto m){
        return (m.source_range_start <= source_val) && (source_val < m.source_range_end );
    });
    if (it == m.mappings.end()){
        //Couldn't find mapping. Defaults to same.
        return source_val;
    }
    auto offset = source_val - it->source_range_start;
    return it->dest_range_start + offset;
}

int main(int argc, char* argv[]){
    using std::begin, std::end;
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    string line;
    uint64_t line_count = 0;
    uint64_t game_number;
    uint64_t sum = 0;
    uint64_t power_sum = 0;

    auto a = parse_file(in);
    cout <<"a " <<a <<endl;
    std::vector<uint64_t> test_vec = {0,1,48,49,50,51,96,97,98,99};
    for (auto ti : test_vec)
        cout <<ti <<"\t" <<find_dest(a.maps[0],ti) <<endl;

    std::vector<vector<uint64_t>> results;
    results.reserve(a.maps.size() + 1);
    results.push_back(a.seeds);
    
    auto resultsi = results.begin();
    for (auto mapi : a.maps){
        std::vector<uint64_t> output;
        for (auto ti : *resultsi){
            output.push_back(find_dest(mapi,ti));
        }
        results.push_back(output);
        ++resultsi;
    }

    bool first = true;
    for (const auto& ri : results){
        if (!first) cout <<"\n";
        cout <<ri ; 
        first = false;
    }
    cout <<endl;
    auto best_loc = std::min_element(begin(results.back()), end(results.back()));
    cout <<"Minimum location: " <<*best_loc<<endl;

    return 0;
}
