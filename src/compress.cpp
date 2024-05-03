#include "../include/stego_main.h"
#include <unordered_map>

using namespace std;

string stego::compress() {
    // Assuming 'message_buffer' is a member variable of the 'stego' class

    vector<int> codes_of_dictionary;

    // Read bytes from message_buffer and store as integers
    while (true) {
        int integer = 0;
        char byte1, byte2, byte3, bits1, bits2;

        message_buffer >> byte1;
        if (message_buffer.eof()) break; // Check if last character read
        integer = byte1 << 4;

        message_buffer >> byte2;
        bits1 = byte2 >> 4;
        integer |= bits1;
        codes_of_dictionary.push_back(integer);

        if (message_buffer.eof()) break; 
        bits2 = byte2 << 4;
        integer = bits2 << 4;
        message_buffer >> byte3; 
        integer |= byte3;
        codes_of_dictionary.push_back(integer);

        if (message_buffer.eof()) break; 
    }

    // LZW compression logic
    unordered_map<string, int> dictionary;
    int next_code = 256;

    string output;
    string current_string;

    for (int code : codes_of_dictionary) {
        string entry;
        if (dictionary.find(entry) == dictionary.end()) {
            entry = current_string + current_string[0];
        } else {
            output += to_string(dictionary[current_string]) + " ";
            if (next_code < 4096) {
                dictionary[current_string + entry[0]] = next_code++;
            }
            current_string = entry;
        }
    }

    // Handle the last code
    output += to_string(dictionary[current_string]);

   
    return output;
}
