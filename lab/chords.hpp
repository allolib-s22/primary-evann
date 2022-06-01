#ifndef CHORDS_HPP
#define CHORDS_HPP

#include <iostream>
#include <unordered_map>
#include <string>
#include <string.h>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<string> allChords = {"I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"};
unordered_map< string, vector<string> > major = 
{
      {"I", vector<string>({"I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"})},
      {"I6", vector<string>({"I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"})},
      {"I64", vector<string>({"I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"})},
      {"ii", vector<string>({"I", "V", "viid", "I6", "V6", "viid6", "I64", "V64", "vii64"})},
      {"ii6", vector<string>({"I", "V", "viid", "I6", "V6", "viid6", "I64", "V64", "vii64"})},
      {"ii64", vector<string>({"I", "V", "viid", "I6", "V6", "viid6", "I64", "V64", "vii64"})},
      {"iii", vector<string>({"I", "ii", "IV", "vi", "I6", "ii6", "IV6", "vi6", "I64", "ii64", "IV64", "vi64"})},
      {"iii6", vector<string>({"I", "ii", "IV", "vi", "I6", "ii6", "IV6", "vi6", "I64", "ii64", "IV64", "vi64"})},
      {"iii64", vector<string>({"I", "ii", "IV", "vi", "I6", "ii6", "IV6", "vi6", "I64", "ii64", "IV64", "vi64"})},
      {"IV", vector<string>({"I", "ii", "iii", "V", "viid", "I6", "ii6", "iii6", "V6", "viid6", "I64", "ii64", "iii64", "V64", "viid64"})},
      {"IV6", vector<string>({"I", "ii", "iii", "V", "viid", "I6", "ii6", "iii6", "V6", "viid6", "I64", "ii64", "iii64", "V64", "viid64"})},
      {"IV64", vector<string>({"I", "ii", "iii", "V", "viid", "I6", "ii6", "iii6", "V6", "viid6", "I64", "ii64", "iii64", "V64", "viid64"})},
      {"V", vector<string>({"I", "vi", "I6", "vi6", "I64", "vi64"})},
      {"V6", vector<string>({"I", "vi", "I6", "vi6", "I64", "vi64"})},
      {"V64", vector<string>({"I", "vi", "I6", "vi6", "I64", "vi64"})},
      {"vi", vector<string>({"I", "ii", "iii", "IV", "V", "I6", "ii6", "iii6", "IV6", "V6", "I64", "ii64", "iii64", "IV64", "V64"})},
      {"vi6", vector<string>({"I", "ii", "iii", "IV", "V", "I6", "ii6", "iii6", "IV6", "V6", "I64", "ii64", "iii64", "IV64", "V64"})},
      {"vi64", vector<string>({"I", "ii", "iii", "IV", "V", "I6", "ii6", "iii6", "IV6", "V6", "I64", "ii64", "iii64", "IV64", "V64"})},
      {"viid", vector<string>({"I", "iii", "I6", "iii6", "I64", "iii64"})},
      {"viid6", vector<string>({"I", "iii", "I6", "iii6", "I64", "iii64"})},
      {"viid64", vector<string>({"I", "iii", "I6", "iii6", "I64", "iii64"})}   
};

vector<string> chordToNotes(string chord) {
    if (chord == "I") 
        return vector<string> {"C", "E", "G"};
    if (chord == "I6") 
        return vector<string> {"E", "G", "C"};
    if (chord == "I64") 
        return vector<string> {"G", "C", "E"};

    if (chord == "ii") 
        return vector<string> {"D", "F", "A"};
    if (chord == "ii6") 
        return vector<string> {"F", "A", "D"};
    if (chord == "ii64") 
        return vector<string> {"A", "D", "F"};

    if (chord == "iii") 
        return vector<string> {"E", "G", "B"};
    if (chord == "iii6") 
        return vector<string> {"G", "B", "E"};
    if (chord == "iii64") 
        return vector<string> {"B", "E", "G"};

    if (chord == "IV") 
        return vector<string> {"F", "A", "C"};
    if (chord == "IV6") 
        return vector<string> {"A", "C", "F"};
    if (chord == "IV64") 
        return vector<string> {"C", "F", "A"};

    if (chord == "V") 
        return vector<string> {"G", "B", "D"};
    if (chord == "V6") 
        return vector<string> {"B", "D", "G"};
    if (chord == "V64") 
        return vector<string> {"D", "G", "B"};

    if (chord == "vi") 
        return vector<string> {"A", "C", "E"};
    if (chord == "vi6") 
        return vector<string> {"C", "E", "A"};
    if (chord == "vi64") 
        return vector<string> {"E", "A", "C"};

    if (chord == "viid") 
        return vector<string> {"B", "D", "F"};
    if (chord == "vi6") 
        return vector<string> {"D", "F", "B"};
    if (chord == "vi64") 
        return vector<string> {"F", "B", "D"};
};

#endif