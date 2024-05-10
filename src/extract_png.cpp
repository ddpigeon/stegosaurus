#include "../include/stego_main.h"
#include "../include/png_helpers.h"
#include <cstdint>
#include <png.h>
using namespace std;

void stego::extract_png() {

    read_png_file((char *) input_path.c_str());

    encryption_bit = row_pointers[0][0] & 0x01;
    compression_bit = row_pointers[0][0] & 0x02;

    int message_size = 0;
    for (int i = 1; i < 17; i++) {
        message_size <<= 2;
        pair<int, int> png_indices = convert_index(i);
        message_size |= (row_pointers[png_indices.first][png_indices.second] & 0x03);
    }

    int current_index = 16;
    for (int i = 0; i < message_size; i++) {
        uint8_t message_byte = 0;
        for (int j = 0; j < 4; j++) {
            current_index++;
            message_byte <<= 2;
            pair<int, int> png_indices = convert_index(current_index);
            message_byte |= (row_pointers[png_indices.first][png_indices.second] & 0x03);
        }
        message_buffer.put(message_byte);
    }


}
