#include "../include/stego_main.h"
#include <unordered_map>
using namespace std;
void stego::decompress(){
   
     vector <int> codes_of_dictionary;
    //read the message_buffer until end and extract codes for dictionary
       while (true) {
        int integer = 0;
        int integer1 = 0;
        char byte1, byte2, byte3;
        char bits1, bits2;

        // Reading 1 byte from stringstream
        message_buffer >> byte1;
        if (message_buffer.eof()) break; // Check if last character read
        integer = (byte1) << 4;

        message_buffer >> byte2;
        // Adding first 4 bits of 2nd byte to integer
        bits1 = byte2 >> 4;
        integer |= bits1;
        codes_of_dictionary.push_back(integer);
        if (message_buffer.eof()) break; // Check if last character read

        // Reading last 4 bits of 2nd byte and appending with 3rd byte
        bits2 = byte2 << 4;
        integer1 = bits2 << 4;
        message_buffer >> byte3; 
        integer1 |= byte3;
        codes_of_dictionary.push_back(integer1);
         if (message_buffer.eof()) break; // Check if last character read
    }
    //creating dictionary
    unordered_map<int, string> dictionary;
    int dictSize = 256;

    // 0-255 assigned to single characters added to dictionary
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = string(1, static_cast<char>(i));
    }
    //lzw decompression algorithm
    string output;
    int prevCode = codes_of_dictionary[0];
    string prevString = dictionary[prevCode];
    output += prevString;

    for (int i = 1; i < codes_of_dictionary.size(); ++i) {
        int code = codes_of_dictionary[i];

        string currentString;
        if (dictionary.find(code) != dictionary.end()) {
            currentString = dictionary[code];
        } else if (code == dictSize) {
            currentString = prevString + prevString[0];
        } else {
            throw runtime_error("Invalid code in compressed data.");
        }

        output += currentString;

        // Add entry to the dictionary
        dictionary[dictSize++] = prevString + currentString[0];

        // Update previous values
        prevString = currentString;
    }

    //storing final output string in message_buffer
    message_buffer << output;


}
