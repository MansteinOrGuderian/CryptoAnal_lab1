#include "Header.h"

std::vector<double> parseCSVLineDouble(const std::string& line) { // Function to split CSV line and convert to doubles
    std::vector<double> result;
    std::stringstream ss(line);
    std::string cell;
    while (std::getline(ss, cell, ',')) { // Remove possible whitespace
        cell.erase(0, cell.find_first_not_of(" \t"));
        cell.erase(cell.find_last_not_of(" \t") + 1);
        
        try {
            double value = std::stod(cell);  
            result.push_back(value);
        } catch (const std::exception& e) {
            std::cerr << "Warning: Could not convert '" << cell << "' to double. Skipping." << std::endl;
        }
    }
    return result;
}

std::vector<int> parseCSVLineInt(const std::string& line) { // Function to split CSV line and convert to integers
    std::vector<int> result;
    std::stringstream ss(line);
    std::string cell;
    while (std::getline(ss, cell, ',')) { // Remove possible whitespace
        cell.erase(0, cell.find_first_not_of(" \t"));
        cell.erase(cell.find_last_not_of(" \t") + 1);
        
        try {
            int value = std::stoi(cell);
            result.push_back(value);
        } catch (const std::exception& e) {
            std::cerr << "Warning: Could not convert '" << cell << "' to int. Skipping." << std::endl;
        }
    }
    return result;
}

std::vector<std::vector<int>> readIntegerCSV(const std::string& filename) { // Function to read integer CSV file into 2D array
    std::vector<std::vector<int>> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return data;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            std::vector<int> row = parseCSVLineInt(line);
            if (!row.empty()) {
                data.push_back(row);
            }
        }
    }
    file.close();
    return data;
}

void print1DArray(const std::vector<double>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
}

void print2DArray(const std::vector<std::vector<int>>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        std::cout << "Row " << i << ": ";
        for (size_t j = 0; j < arr[i].size(); j++) {
            std::cout << arr[i][j];
            if (j < arr[i].size() - 1) 
                std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<double> calculate_ciphertext_probabilities(std::vector<double>& probability_M, std::vector<double>& probability_K, const std::vector<std::vector<int>>& cipher_table) {
    int max_cipher_value = 0; // not actually needed with given input data
    for (const auto& row : cipher_table) {          //    \ M_1 M_2 ... M_n
        for (int value : row) {                     //  K_1 C_i C_j ... C_f
            if (value > max_cipher_value) {         //  K_2 C_h
                max_cipher_value = value;           //  ... ...
            }                                       //  K_n C_g
        }
    }
    int result_size = max_cipher_value + 1;
    double* result_P_C = new double[result_size]();
    for (int i = 0; i < cipher_table.size(); i++) // key
    //{
        for (int j = 0; j < cipher_table[i].size(); j++) // opened message 
        //{
            //std::cout << cipher_table[i][j] << "+ K_" << i << "*" << "M_" << j << "\n";
            result_P_C[cipher_table[i][j]] += probability_K[i] * probability_M[j];
        //}
    //std::cout << '\n';
    //}
    std::vector<double> vec_result_P_C;
    for (int i = 0; i < cipher_table.size(); i++)
        vec_result_P_C.push_back(result_P_C[i]);
    delete[] result_P_C;
    return vec_result_P_C;
}