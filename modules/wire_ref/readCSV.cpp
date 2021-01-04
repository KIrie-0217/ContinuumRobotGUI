#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "readCSV.h"

using namespace std;

vector<vector<double>> readCSV::read_out(string path){
    input_path = path;
    ifstream ifs(input_path);
    int i =0;
    while(getline(ifs,str_buf)){
        vector<string> strvec = split(str_buf,',');
        if(i==0){
            output.resize(strvec.size());
        }  
        for(int i=0; i < strvec.size(); i++){
            output[i].push_back(stod(strvec.at(i)));
        }

    }   
    return output;
}


vector<string> readCSV::split(string& input, char delimeter){
    istringstream stream(input);

    while(getline(stream,field,delimeter)){
        result.push_back(field);
    }
    return result;
}


