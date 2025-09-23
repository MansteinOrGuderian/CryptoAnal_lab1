#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

std::vector<double> parseCSVLineDouble(const std::string& line);
std::vector<int> parseCSVLineInt(const std::string& line);
std::vector<std::vector<int>> readIntegerCSV(const std::string& filename);
template<typename T>
void print1DArray(const std::vector<T>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
}
template<typename T>
void print2DArray(const std::vector<std::vector<T>>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        std::cout << "R " << i << ":\t";
        for (size_t j = 0; j < arr[i].size(); j++) {
            std::cout << arr[i][j];
            if (j < arr[i].size() - 1)
                std::cout << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
std::vector<std::vector<int>> make_transpose(const std::vector<std::vector<int>>& matrix);
std::vector<double> calculate_ciphertext_probabilities(std::vector<double>& probability_M, std::vector<double>& probability_K, const std::vector<std::vector<int>>& cipher_table);
std::vector<std::vector<double>> calculate_joint_probabilities(std::vector<double>& probability_M, std::vector<double>& probability_K, const std::vector<std::vector<int>>& cipher_table);
std::vector<std::vector<double>> calculate_conditional_probabilities(std::vector<std::vector<double>>& probability_M_C, std::vector<double>& probability_C);
std::vector<int> optimal_deterministic_decision_function(std::vector<std::vector<double>>& probability_M_in_case_C);
std::vector<std::vector<double>> optimal_stochastic_decision_function(std::vector<std::vector<double>>& probability_M_in_case_C);
double average_loss_deterministic_function(std::vector<std::vector<double>>& probability_M_in_case_C, std::vector<int>& optimal_deterministic_decision);
double average_loss_stochastic_function(std::vector<std::vector<double>>& probability_M_in_case_C, std::vector<std::vector<double>>& optimal_stochastic_decision);