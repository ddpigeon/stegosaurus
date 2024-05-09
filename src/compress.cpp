#include "../include/stego_main.h"
#include <cstdint>
#include <iostream>
#include <unordered_map>
using namespace std;

void stego::compress() {
    // LZW compression logic

    string message_string = message_buffer.str();
    //cout << "Encoding\n"; 
    unordered_map<string, int> encoding_table; 
    for (int i = 0; i <= 255; i++) { 
        string ch = ""; 
        ch += char(i); 
        encoding_table[ch] = i; 
    } 
  
    string p = "", c = ""; 
    p += message_string[0]; 
    int code = 256; 
    vector<int> output_code; 
    //cout << "String\tOutput_Code\tAddition\n"; 
    for (int i = 0; i < message_string.length(); i++) { 
        if (i != message_string.length() - 1) c += message_string[i + 1]; 
        if (encoding_table.find(p + c) != encoding_table.end()) p += c; 

        else { 
            //cout << p << "\t" << encoding_table[p] << "\t\t" 
            //     << p + c << "\t" << code << endl; 
            output_code.push_back(encoding_table[p]); 
            encoding_table[p + c] = code; 
            code++; 
            p = c; 
        } 
        c = ""; 
    } 
    //cout << p << "\t" << encoding_table[p] << endl; 
    output_code.push_back(encoding_table[p]); 
    message_buffer.str("");

    for (int i = 0; i < output_code.size(); i++) cout << hex << output_code[i] << " ";
    cout << endl;

    //Write output code to message buffer
    if (output_code.size() % 2 == 1) output_code.push_back(0); // Pad with zero in case of odd number of codes
    
    for (int i = 0; i < output_code.size(); i += 2) {
        uint32_t c1 = output_code[i];
        uint32_t c2 = output_code[i+1];

        char coded_bytes[3];
        coded_bytes[0] = c1 >> 4;
        coded_bytes[2] = c2 % 256;
        coded_bytes[1] = (c1 % 16) << 4 | (c2 >> 8);

        message_buffer.write(coded_bytes, 3);
    }
}
