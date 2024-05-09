#include "../include/stego_main.h"
#include <cstdint>
#include <cstring>
#include <iostream>
using namespace std;

void stego::hide_png() {
    // Implement hiding of message in png file here
    
    //string input_string = input_buffer.str();
    input_buffer.seekg(0, ios::end);
    int input_buf_size = input_buffer.tellg();
    //cout << "SIZE: " << hex << input_buf_size << endl;
    input_buffer.seekg(ios::beg);

    // Deal with PNG header
    char header_bytes[8];
    input_buffer.read(header_bytes, 8);
    output_buffer << header_bytes;
    int input_buf_index = 8; 

    while (input_buf_index < input_buf_size) {
        // Read chunk size
        char chunk_size_bytes[4];
        input_buffer.read(chunk_size_bytes, 4);
        output_buffer.write(chunk_size_bytes, 4);
        int chunk_size = 0;
        chunk_size = 
            (chunk_size_bytes[0]) << 24
            | (chunk_size_bytes[1]) << 16
            | (chunk_size_bytes[2]) << 8
            | static_cast<uint8_t>(chunk_size_bytes[3]);

        // Get chunk type
        char chunk_type[5];
        input_buffer.read(chunk_type, 4);
        chunk_type[4] = 0;
        //cout << chunk_type << ": " << hex << chunk_size << endl;


        //output_buffer << chunk_size_bytes;
        output_buffer.write(chunk_type, 4);
        input_buf_index += 8;

        if (strcmp(chunk_type, "IDAT") != 0) {
            // just write to output for non IDAT chunks.
            char data_byte[1];
            for (int i = 0; i < chunk_size; i++) {
                input_buffer.read(data_byte, 1);
                output_buffer.write(data_byte, 1);
            }

            char checksum_bytes[4];
            input_buffer.read(checksum_bytes, 4);
            output_buffer.write(checksum_bytes, 4);

            input_buf_index += (chunk_size + 4);
        }
        else {
            int end_of_chunk_index = input_buf_index + (chunk_size + 4);

            // Write the status of compression/encryption.
            uint8_t pattern = (compression_bit << 1) | encryption_bit;
            char first_byte[1];
            input_buffer.read(first_byte, 1);
            first_byte[0] = (first_byte[0] & 0xFC) | pattern;
            output_buffer.write(first_byte, 1);
            input_buf_index += 1;


            // Get message size
            message_buffer.seekg(0, ios::end);
            int message_size = message_buffer.tellg();
            //cout << "MESSAGE SIZE: " << hex << message_size << endl;
            message_buffer.seekg(ios::beg);
            //cout << message_buffer.rdbuf();

            // stored in little endian, so last byte is the first etc etc.
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
                input_buf_index += 4;
            }

            // Actually hiding the message bytes
            char message_byte[1];
            char message_bytes_out[4];
            for (int i = 0; i < message_size; i++) {
                message_buffer.read(message_byte, 1);
                input_buffer.read(message_bytes_out, 4);
                message_bytes_out[0] = (message_bytes_out[0] & 0xFC) | (message_byte[0] >> 6);
                message_bytes_out[1] = (message_bytes_out[1] & 0xFC) | ((message_byte[0] & 0x30) >> 4);
                message_bytes_out[2] = (message_bytes_out[2] & 0xFC) | ((message_byte[0] & 0x0C) >> 2);
                message_bytes_out[3] = (message_bytes_out[3] & 0xFC) | (message_byte[0] & 0x03);
                output_buffer.write(message_bytes_out, 4);
                input_buf_index += 4;
            }
            cout << hex << input_buf_index << endl;

            // Write remaining bytes as is.
            while (input_buf_index < end_of_chunk_index) {
                input_buffer.read(message_byte, 1);
                output_buffer.write(message_byte, 1);
                input_buf_index++;
            }
            cout << hex << input_buf_index << endl;

        }
        //char chunksize = static_cast<int>(input_buffer);   

    }
    //cout << output_buffer.rdbuf();



    //cout << std::hex << *(int*) &(output_string[0]);
    //cout << std::hex << *(int*) &(output_string[4]);
    //cout << std::hex << output_buffer.rdbuf();

}
