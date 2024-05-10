#include "../include/stego_main.h"
#include "../include/png_helpers.h"
#include <cstdint>
#include <iostream>
#include <zlib.h>
using namespace std;

void stego::hide_png() {

    read_png_file((char *) input_path.c_str());

    // row_pointers contains pixel data as (R, G, B, A) for each pixel in a row
    // each element of row_pointer contains 4 * width bytes which can be used to hide stuff 

    // Write the status of compression/encryption.
    uint8_t pattern = (compression_bit << 1) | encryption_bit;
    row_pointers[0][0] = (row_pointers[0][0] & 0xFC) | pattern;


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
        pair<int, int> png_indices;
        png_indices = convert_index(13 - 4*i);
        row_pointers[png_indices.first][png_indices.second] = (row_pointers[png_indices.first][png_indices.second] & 0xFC) | (cur_byte >> 6);
        png_indices = convert_index(14 - 4*i);
        row_pointers[png_indices.first][png_indices.second] = (row_pointers[png_indices.first][png_indices.second] & 0xFC) | ((cur_byte & 0x30) >> 4);
        png_indices = convert_index(15 - 4*i);
        row_pointers[png_indices.first][png_indices.second] = (row_pointers[png_indices.first][png_indices.second] & 0xFC) | ((cur_byte & 0x0C) >> 2);
        png_indices = convert_index(16 - 4*i);
        row_pointers[png_indices.first][png_indices.second] = (row_pointers[png_indices.first][png_indices.second] & 0xFC) | (cur_byte & 0x03);
    }

    // Actually hiding the message bytes
    int current_index = 16;
    char message_byte[1];
    for (int i = 0; i < message_size; i++) {
        message_buffer.read(message_byte, 1);
        pair<int, int> png_indices;
        png_indices = convert_index(++current_index);
        row_pointers[png_indices.first][png_indices.second] = (row_pointers[png_indices.first][png_indices.second] & 0xFC) | (message_byte[0] >> 6);
        png_indices = convert_index(++current_index);
        row_pointers[png_indices.first][png_indices.second] = (row_pointers[png_indices.first][png_indices.second] & 0xFC) | ((message_byte[0] & 0x30) >> 4);
        png_indices = convert_index(++current_index);
        row_pointers[png_indices.first][png_indices.second] = (row_pointers[png_indices.first][png_indices.second] & 0xFC) | ((message_byte[0] & 0x0C) >> 2);
        png_indices = convert_index(++current_index);
        row_pointers[png_indices.first][png_indices.second] = (row_pointers[png_indices.first][png_indices.second] & 0xFC) | (message_byte[0] & 0x03);
    }

    write_png_file((char *) output_path.c_str());

}
