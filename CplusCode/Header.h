#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

std::vector<double> parseCSVLineDouble(const std::string& line);
std::vector<int> parseCSVLineInt(const std::string& line);
std::vector<std::vector<int>> readIntegerCSV(const std::string& filename);
void print1DArray(const std::vector<double>& arr);
void print2DArray(const std::vector<std::vector<int>>& arr);
std::vector<double> calculate_ciphertext_probabilities(std::vector<double>& probability_M, std::vector<double>& probability_K, const std::vector<std::vector<int>>& cipher_table);
