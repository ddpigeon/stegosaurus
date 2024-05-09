#include "../include/stego_main.h"
#include <cstdint>
#include <iostream>
using namespace std;

void stego::extract_wav() {

    int current_index = 12; //header
    string input_str = input_buffer.str();

    while (current_index < input_str.size()) {
        string subchunk_id = input_str.substr(current_index, 4);
        current_index += 4;
        int subchunk_size = *(int *) &input_str[current_index];
        current_index += 4;

        if(subchunk_id != "data") current_index += subchunk_size;

        else {
            // check for encryption/compression
            uint8_t first_bit = input_str[current_index];
            cout << hex << (int) first_bit << endl;
            encryption_bit = first_bit & 0x01;
            compression_bit = first_bit & 0x02;
            current_index++;

            // Message size
            uint32_t messageSize = 0;
            for (int i = 0; i < 4; i++) {
                uint8_t message_size_byte = 
                    ((input_str[current_index] & 0x03) << 6) |
                    ((input_str[current_index + 1] & 0x03) << 4) |
                    ((input_str[current_index + 2] & 0x03)<< 2) |
                    ((input_str[current_index + 3] & 0x03));


                messageSize |= message_size_byte << (8 * (3-i));
                current_index += 4;
            }


            for (int i = 0; i < messageSize; i++) {
                uint8_t message_byte = 0;
                for (int j = 0; j < 4; j++) {
                    message_byte <<= 2;
                    message_byte |= (input_str[current_index + j] & 0x03);
                }
                current_index += 4;
                message_buffer.put(message_byte);
            }
        }

    }
    message_buffer.seekg(0);

}
