#include "../include/stego_main.h"

void stego::extract_png() {
    // Implement extraction of message from png file here
     encryption_bit = 0;
     compression_bit = 0;
    
    size_t currentindex = 8;
    string input_str = input_buffer.str();

    while (currentindex < input_str.size()) 
    {
        size_t chunkSize = 
                (input_str[currentindex] << 24) |
                (input_str[currentindex + 1] << 16) |
                (input_str[currentindex + 2] << 8) |
                (input_str[currentindex + 3]);
        currentindex += 4;

        if (input_str[currentindex] == 'I' && input_str[currentindex + 1] == 'D' &&
                input_str[currentindex + 2] == 'A' && input_str[currentindex + 3] == 'T')
        {
            currentindex += 4;

            encryption_bit = input_str[currentindex] & 0x01;
            compression_bit = input_str[currentindex] & 0x02;

            uint32_t messageSize = 0;
            for (int i = 0; i < 4; i++) 
            {
                uint8_t message_size_byte = 
                ((input_str[currentindex] & 0x03) << 6) |
                ((input_str[currentindex + 1] & 0x03) << 4) |
                ((input_str[currentindex + 2] & 0x03)<< 2) |
                ((input_str[currentindex + 3] & 0x03));
                

                messageSize |= message_size_byte << (8 * (3-i));
                currentindex += 4;

            for (size_t i = 0; i < messageSize; ++i)
            {
                unsigned char messageByte = 0;

                for (int j = 0; j < 4; ++j) 
                {
                    unsigned char pattern = input_str[currentindex + j] & 0x03;
                    messageByte |= pattern;
                    if (j < 3)
                    {
                        messageByte <<= 2;
                    }
                  
                    currentindex += 4;
                }
                
                message_buffer.put(messageByte);
            }
            break;
          } 
        }
        else 
        {
            currentindex += (chunkSize + 8);
        }
    }
}
