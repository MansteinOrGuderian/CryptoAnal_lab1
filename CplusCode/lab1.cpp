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
    
    std::cout << "=== PROBABILITY DATA ===" << std::endl;
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
    std::vector<std::vector<int>> raw_cipher_table = readIntegerCSV(intFilename);
    
    //    \ M_1 M_2 ... M_n           \  K_1 K_2 ... K_n
    //  K_1 C_i C_j ... C_f          M_1 C_i C_h ... C_g
    //  K_2 C_h              ----->  M_2 C_j 
    //  ... ...                      ... ... 
    //  K_n C_g                      M_n C_f

    std::vector<std::vector<int>> cipher_table = make_transpose(raw_cipher_table);

    if (!cipher_table.empty()) {
        std::cout << "\n" << "=== Cipher algorithm Info ===" << std::endl;
        print2DArray(cipher_table);
    } else {
        std::cerr << "Failed to read integer CSV file or file is empty!" << std::endl;
    }
    
    std::vector<double> probability_C = calculate_ciphertext_probabilities(probability_M, probability_K, cipher_table);
    std::cout << "\nP(C): ";
    print1DArray(probability_C);

    std::vector<std::vector<double>> probability_M_C = calculate_joint_probabilities(probability_M, probability_K, cipher_table);
    std::cout << "\nP(M, C) : \n";
    print2DArray(probability_M_C);

    std::vector<std::vector<double>> probability_M_in_case_C = calculate_conditional_probabilities(probability_M_C, probability_C);
    std::cout << "\nP(M | C) : \n";
    print2DArray(probability_M_in_case_C);
    
    std::vector<int> optimal_deterministic_decision = optimal_deterministic_decision_function(probability_M_in_case_C);
    std::cout << "Optimal deterministic decision is : \n";
    print1DArray(optimal_deterministic_decision);

    std::vector<std::vector<double>> optimal_stochastic_decision = optimal_stochastic_decision_function(probability_M_in_case_C);
    std::cout << "Optimal stochastic decision is : \n";
    print2DArray(optimal_stochastic_decision);

    double average_loss_deterministic = average_loss_deterministic_function(probability_M_in_case_C, optimal_deterministic_decision);
    std::cout << "Avarage deterministic decision loss is : " << average_loss_deterministic << "\n";
    
    double average_loss_stochastic = average_loss_stochastic_function(probability_M_in_case_C, optimal_stochastic_decision);
    std::cout << "Avarage stochastic decision loss is : " << average_loss_deterministic << "\n";

    return 0;
}

