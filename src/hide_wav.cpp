#include "../include/stego_main.h"
#include <iostream>
#include <cstdint>

void stego::hide_wav() {
    //implement hiding in wav files
    size_t current_index = 0;
    string input_str = input_buffer.str();

    // Write wav header
    output_buffer.write(input_str.substr(0, 12).c_str(), 12);
    current_index+=12;

    while(current_index<input_str.size()){

        // Get subchunk ID and size
        string subchunk_id = input_str.substr(current_index, 4);     
        current_index+=4;
        int subchunk_size = *(int*) &input_str[current_index];
        current_index+=4;

        if(subchunk_id != "data"){
            output_buffer.write(&input_str[current_index-8], 8);
            output_buffer.write(&input_str[current_index], subchunk_size);
            current_index+=subchunk_size;
        }

        else{  
            output_buffer.write(&input_str[current_index-8], 8);

            char first_byte= input_str[current_index];
            first_byte &= 0xFC;
            first_byte |= (encryption_bit); 
            first_byte |= (compression_bit << 1);
            output_buffer.put(first_byte);
            current_index++;


            message_buffer.seekg(0, ios::end);
            int message_size = message_buffer.tellg();
            message_buffer.seekg(ios::beg);

            input_buffer.seekg(current_index);
            uint8_t *size_bytes = (uint8_t*) &message_size;

            // Writes the size to the next 16 bytes
            for (int i = 3; i >= 0; i--) {
                uint8_t cur_byte = size_bytes[i];
                char size_bytes_out[4];
                input_buffer.read(size_bytes_out, 4);
                size_bytes_out[0] = (size_bytes_out[0] & 0xFC) | (cur_byte >> 6);
                size_bytes_out[1] = (size_bytes_out[1] & 0xFC) | ((cur_byte & 0x30) >> 4);
                size_bytes_out[2] = (size_bytes_out[2] & 0xFC) | ((cur_byte & 0x0C) >> 2);
                size_bytes_out[3] = (size_bytes_out[3] & 0xFC) | (cur_byte & 0x03);
                output_buffer.write(size_bytes_out, 4);
                current_index += 4;
            }


            for(int i = 0; i < message_size; i++){
                char message_byte = message_buffer.get();
                for (int j=0; j<4; ++j){
                    char input_byte = input_buffer.get();
                    input_byte &= 0xFC;  
                    input_byte |= ((message_byte>>(6-2*j)) & 0x03);  
                    output_buffer.put(input_byte);
                    current_index += 1;
                }
            }
            while (current_index < input_str.size()) {
            output_buffer.put(input_buffer.get()); 
            current_index++;
            }
        }
    }   
}
