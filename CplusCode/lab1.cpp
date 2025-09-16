#include "Header.h"

int main() {
    std::string doubleFilename = "../var_data/prob_15.csv";
    std::vector<double> probability_M, probability_K;
    
    std::ifstream file(doubleFilename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << doubleFilename << std::endl;
        return 1;
    }
    
    std::string temp_line;
    
    if (std::getline(file, temp_line)) {
        probability_M = parseCSVLineDouble(temp_line);
    }
    if (std::getline(file, temp_line)) {
        probability_K = parseCSVLineDouble(temp_line);
    }
    file.close();
    
    std::cout << "=== DOUBLE DATA ===" << std::endl;
    std::cout << "Opened text probability: ";
    for (const auto& item : probability_M) {
        std::cout << item << " ";
    }

    std::cout << std::endl;
    
    std::cout << "Secret keys probability: ";
    for (const auto& item : probability_K) {
        std::cout << item << " ";
    }
    
    // ---------------------------------------------------

    std::string intFilename = "../var_data/table_15.csv";
    std::vector<std::vector<int>> cipher_table = readIntegerCSV(intFilename);
    
    if (!cipher_table.empty()) {
        std::cout << "\n" << "=== Cipher algorithm Info ===" << std::endl;
        print2DArray(cipher_table);
    } else {
        std::cerr << "Failed to read integer CSV file or file is empty!" << std::endl;
    }
    
    std::vector<double> ciphertext_probabilities = calculate_ciphertext_probabilities(probability_M, probability_K, cipher_table);
    print1DArray(ciphertext_probabilities);
    return 0;
}

