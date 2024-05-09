#include "../include/stego_main.h"
#include <cstdint>
#include <iostream>
#include <unordered_map>
using namespace std;

void stego::decompress(){

    vector <int> codes_of_dictionary;
    int buffer_index = 0;
    message_buffer.seekg(0, ios::end);
    int buf_size = message_buffer.tellg();
    message_buffer.seekg(ios::beg);


    //read the message_buffer until end and extract codes for dictionary
    while (buffer_index < buf_size) {
        int integer = 0;
        int integer1 = 0;
        uint8_t bytes[3];
        uint8_t bits1, bits2;

        // Reading 1 byte from stringstream
        message_buffer.read((char*) bytes, 3);
        buffer_index += 3;
        integer = (bytes[0]) << 4;

        // Adding first 4 bits of 2nd byte to integer
        bits1 = bytes[1] >> 4;
        integer |= bits1;
        codes_of_dictionary.push_back(integer);

        // Reading last 4 bits of 2nd byte and appending with 3rd byte
        bits2 = bytes[1] << 4;
        integer1 = bits2 << 4;
        integer1 |= bytes[2];

        codes_of_dictionary.push_back(integer1);
        //if (message_buffer.eof()) break; // Check if last character read
    }

    for (int i = 0; i < codes_of_dictionary.size(); i++) cout << hex << codes_of_dictionary[i] << " ";
    cout << endl;


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
    message_buffer.str(output);
    //cout << message_buffer.rdbuf();

}
