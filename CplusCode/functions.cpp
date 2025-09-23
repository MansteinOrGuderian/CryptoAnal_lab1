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

std::vector<std::vector<int>> make_transpose(const std::vector<std::vector<int>>& matrix) {

    int original_rows = matrix.size();
    int original_cols = matrix[0].size();
    std::vector<std::vector<int>> transposed_matrix(original_cols, std::vector<int>(original_rows));

    for (int i = 0; i < original_rows; ++i) {
        for (int j = 0; j < original_cols; ++j) {
            transposed_matrix[j][i] = matrix[i][j];
        }
    }

    return transposed_matrix;
}

std::vector<double> calculate_ciphertext_probabilities(std::vector<double>& probability_M, std::vector<double>& probability_K, const std::vector<std::vector<int>>& cipher_table) {
    //int max_cipher_value = 0; // not actually needed with given input data
    //for (const auto& row : cipher_table) {          //    \ K_1 K_2 ... K_n
    //    for (int value : row) {                     //  M_1 C_i C_j ... C_f
    //        if (value > max_cipher_value) {         //  M_2 C_h
    //            max_cipher_value = value;           //  ... ...
    //        }                                       //  M_n C_g
    //    }
    //}
    //int result_size = max_cipher_value + 1;
    //double* result_P_C = new double[result_size]();
    double* result_P_C = new double[cipher_table.size()] ();
    for (int i = 0; i < cipher_table.size(); i++) // key
    //{
        for (int j = 0; j < cipher_table[i].size(); j++) // opened message 
        //{
            //std::cout << cipher_table[i][j] << "+ K_" << i << "*" << "M_" << j << "\n";
            result_P_C[cipher_table[i][j]] += probability_M[i] * probability_K[j];
        //}
    //std::cout << '\n';
    //}
    std::vector<double> vec_result_P_C;
    for (int i = 0; i < cipher_table.size(); i++)
        vec_result_P_C.push_back(result_P_C[i]);
    delete[] result_P_C;
    return vec_result_P_C;
}

std::vector<std::vector<double>> calculate_joint_probabilities(std::vector<double>& probability_M, std::vector<double>& probability_K, const std::vector<std::vector<int>>& cipher_table) {
    double** probability_P_M_C = new double*[cipher_table.size()];
    for (int i = 0; i < cipher_table.size(); i++)
        probability_P_M_C[i] = new double[cipher_table.size()] ();
    
    for (int i = 0; i < cipher_table.size(); i++) 
        for (int j = 0; j < cipher_table.size(); j++) // P(M, C) = \sum_{(K, M) = C} P(M) * P(C)  // why sum, if all unique ????
            probability_P_M_C[i][cipher_table[i][j]] += probability_M[i] * probability_K[j]; 

    std::vector<std::vector<double>> vec_probability_P_M_C(cipher_table.size(), std::vector<double>(cipher_table.size(), 0));
    for (int i = 0; i < cipher_table.size(); i++)
        for (int j = 0; j < cipher_table.size(); j++)
            vec_probability_P_M_C[i][j] = probability_P_M_C[i][j];

    for (int i = 0; i < cipher_table.size(); i++)
        delete[] probability_P_M_C[i];
    delete[] probability_P_M_C;

    return vec_probability_P_M_C;
}

std::vector<std::vector<double>> calculate_conditional_probabilities(std::vector<std::vector<double>>& probability_M_C, std::vector<double>& probability_C) {
    std::vector<std::vector<double>> probability_M_in_case_C = probability_M_C;
    for (int i = 0; i < probability_M_in_case_C.size(); i++)
        for (int j = 0; j < probability_M_in_case_C[i].size(); j++)
            probability_M_in_case_C[i][j] /= probability_C[j];
    return probability_M_in_case_C;
} // TO DO

std::vector<int> optimal_deterministic_decision_function(std::vector<std::vector<double>>& probability_M_in_case_C) {
    std::vector<int> optimal_deterministic_decision;
    for (int i = 0; i < probability_M_in_case_C.size(); i++) { // row
        int index_of_max_value = 0;
        double max_val = probability_M_in_case_C[i][0];
        for (int j = 0; j < probability_M_in_case_C[i].size(); j++) { // column
            if (probability_M_in_case_C[i][j] > max_val) {
                max_val = probability_M_in_case_C[i][j];
                index_of_max_value = j;
            }                
        }
        optimal_deterministic_decision.push_back(index_of_max_value);
    }
    return optimal_deterministic_decision;
}

std::vector<std::vector<double>> optimal_stochastic_decision_function(std::vector<std::vector<double>>& probability_M_in_case_C) {
    std::vector<std::vector<double>> optimal_stochastic_decision = probability_M_in_case_C;
    for (int i = 0; i < probability_M_in_case_C.size(); i++) {
        
        int amount_of_max_value = 0;
        double maxValue = *std::max_element(probability_M_in_case_C[i].begin(), probability_M_in_case_C[i].end());
        //std::cout << maxValue << ' ';
        for (int j = 0; j < probability_M_in_case_C[i].size(); j++)
            if (probability_M_in_case_C[i][j] == maxValue) 
                amount_of_max_value += 1;
        //std::cout << amount_of_max_value << '\n';
        for (int j = 0; j < probability_M_in_case_C[i].size(); j++)
            optimal_stochastic_decision[i][j] == maxValue ? optimal_stochastic_decision[i][j] = 1 / amount_of_max_value : optimal_stochastic_decision[i][j] = 0; // maybe not integer value in some row
    }
    return optimal_stochastic_decision;
} 

double average_loss_deterministic_function(std::vector<std::vector<double>>& probability_M_in_case_C, std::vector<int>& optimal_deterministic_decision) {
    double average_loss_deterministic = 0.0;
    for (int i = 0; i < optimal_deterministic_decision.size(); i++) {
        int index = optimal_deterministic_decision[i];
        average_loss_deterministic += probability_M_in_case_C[index][i];
    }
    return (1.0 - average_loss_deterministic); // 1 - \sum_{c \in C} P(M = \delta(C=c), C=c)
}

double average_loss_stochastic_function(std::vector<std::vector<double>>& probability_M_in_case_C, std::vector<std::vector<double>>& optimal_stochastic_decision) {
    double average_loss_stochastic = 0.0;
    for (int i = 0; i < probability_M_in_case_C.size(); i++) {
        for (int j = 0; j < probability_M_in_case_C[i].size(); j++) {
            average_loss_stochastic += probability_M_in_case_C[i][j] * optimal_stochastic_decision[j][i];
        }
    }
    return (1.0 - average_loss_stochastic); // 1 - \sum_{ \forall (m,c) } P(M = m, C = c) * \delta_S(c, m)
}