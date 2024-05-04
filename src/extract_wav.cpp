#include "../include/stego_main.h"

void stego::extract_wav() {
    encryption_bit = 0;
    compression_bit = 0;
    message_size = 0;

    // Skip the WAV header (12 bytes)
    int index = 12;

    // Iterate through subchunks until "data" subchunk is found
    while (index < input_size - 8) {
        // Read the subchunk ID and size
        string subchunk_id(input_buffer + index, 4);
        int subchunk_size = *((int*)(input_buffer + index + 4));

        // If it's the "data" subchunk, extract message
        if (subchunk_id == "data") {
            // Read encryption and compression bits from the first data byte
            char first_data_byte = input_buffer[index + 8];
            encryption_bit = first_data_byte & 0b00000001;
            compression_bit = (first_data_byte & 0b00000010) >> 1;

            // Read message size (4 bytes)
            message_size = *((int*)(input_buffer + index + 9));

            // Extract the message
            for (int i = 0; i < message_size; ++i) {
                char message_byte = 0;
                for (int j = 0; j < 4; ++j) {
                    char byte = input_buffer[index + 13 + (i * 4) + j];
                    char pattern = byte & 0b00000011;
                    message_byte |= pattern;
                    if (j < 3) {
                        message_byte <<= 2;
                    }
                }
                message_buffer << message_byte;
            }
            break;
        }
        // Move to the next subchunk
        index += 8 + subchunk_size;
    }
}
