#ifndef readCSV_h
#define readCSV_h

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>


class readCSV{
    public:
        std::vector<std::vector<double>> read_out(std::string input_path);
        std::vector<std::string> split(std::string& input, char delimiter);
        std::vector<double> motor1_1;
        std::vector<double> motor1_2;
        std::vector<double> motor1_3;
        std::vector<double> motor2_1;
        std::vector<double> motor2_2;
        std::vector<double> motor2_3;
        std::vector<double> motor3_1;
        std::vector<double> motor3_2;
        std::vector<double> motor3_3;

        std::vector<std::vector<double>> output;

        std::string input_path;




    private:
        std::vector<std::string> result;
        std::string str_buf;
        std::string field;
        std::string str_conma_buf;
};


#endif