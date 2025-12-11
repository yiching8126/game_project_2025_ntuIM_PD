#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <locale>
#include <sstream>
#include <cctype>
using std::endl;
// Note: Using default min=-1 and max=8, but now -1 explicitly means no minimum boundary.
int force_input_integer(int min = -1, int max = 8, int exclude = -999999) {
    // Determine if a range check should be performed.
    // The range check is active IF min != -1 OR max != 8 (if max is meant to be checked)
    // For simplicity and clarity, we'll assume a range is checked if min or max is NOT the sentinel/default.
    
    // The most common convention is: -1 is a sentinel value indicating no boundary check.
    // The check below ensures we only check the range if a valid boundary is set.
    bool check_min = (min != -1); 
    bool check_max = (max != 8); // Assuming 8 is also a sentinel/default if not explicitly passed

    std::string s;
    int res;

    // Use a while loop instead of recursion to prevent stack overflow
    while (true) {
       // std::cout << "請輸入整數";
        
        // Print range requirements if boundaries are set
        if (check_min || check_max ) {
             std::cout << " (";
             if (check_min) std::cout << ">= " << min;
             if (check_min && check_max) std::cout << " 且 ";
             if (check_max) std::cout << "<= " << max;
             std::cout << ")";
        }
        std::cout << ": ";
        
        // 1. Get user input
        if (!std::getline(std::cin, s)) {
            // Handle EOF or fatal stream error
            throw std::runtime_error("Input stream error occurred.");
        }

        try {
            // 2. Attempt conversion
            res = std::stoi(s);

            // 3. Perform Range Check
            if (check_min && res < min) {
                std::cout << "錯誤：輸入值小於下限" << min << std::endl;
                continue; // Loop again
            }
            if (check_max && res > max) {
                std::cout << "錯誤：輸入值大於上限" << max << std::endl;
                continue; // Loop again
            }
            if(res == exclude){
                std::cout << "錯誤：該目標不能被指定" << endl;
            }

            // Success! Return the result and exit the loop.
            return res;

        } catch (const std::invalid_argument &e) {
            std::cerr << "錯誤：請重新輸入一個整數 (" << e.what() << ")" << std::endl;
            // Loop again
        } catch (const std::out_of_range &e) {
            // Handle numbers that are too large or too small for 'int'
            std::cerr << "錯誤：輸入值超出整數範圍 (" << e.what() << ")" << std::endl;
            // Loop again
        }
        // No need for a generic catch if we handle the main two cases.
        // Any other std::exception would be highly unusual for std::stoi.
    }
}