#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

struct StockData {
    std::string date;
    double open, high, low, close;
    long volume;
};

// Helper function to clean dollar signs and spaces
std::string cleanString(const std::string &str) {
    std::string cleaned;
    for (char c : str) {
        if (std::isdigit(c) || c == '.' || c == '-') { // Allow numbers, decimals, and negative signs
            cleaned += c;
        }
    }
    return cleaned;
}

// read csv
std::vector<StockData> readCSV(const std::string &filename) {
    std::vector<StockData> data;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    bool isHeader = true;
    while (std::getline(file, line)) {
        if (isHeader) { // Skip the first line (header)
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        StockData entry;
        std::string temp;

        try {
            std::getline(ss, entry.date, ','); // Date (may need fixing)
            
            std::getline(ss, temp, ','); entry.open = std::stod(cleanString(temp));
            std::getline(ss, temp, ','); entry.volume = std::stol(cleanString(temp));
            std::getline(ss, temp, ','); entry.high = std::stod(cleanString(temp));
            std::getline(ss, temp, ','); entry.low = std::stod(cleanString(temp));
            std::getline(ss, temp, ','); entry.close = std::stod(cleanString(temp));

            data.push_back(entry);
        } catch (const std::exception &e) {
            std::cerr << "Error parsing line: " << line << " - " << e.what() << std::endl;
        }
    }
    return data;
}



double backtestBuyHold(const std::vector<StockData> &data, double initialCapital = 10000) {
    if (data.empty()) return 0;

    double shares = initialCapital / data[0].open;
    double finalValue = shares * data.back().close;

    std::cout << "Initial Capital: $" << initialCapital << std::endl;
    std::cout << "Final Portfolio Value: $" << finalValue << std::endl;
    std::cout << "Return: " << ((finalValue - initialCapital) / initialCapital) * 100 << "%" << std::endl;

    return finalValue;
}

int main() {
    std::string filename = "tsla.csv";
    std::vector<StockData> marketData = readCSV(filename);

    if (marketData.empty()) {
        std::cerr << "No data loaded." << std::endl;
        return 1;
    }

    backtestBuyHold(marketData);
    return 0;
}

