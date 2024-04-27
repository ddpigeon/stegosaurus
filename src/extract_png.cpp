#include "../include/stego_main.h"

void stego::extract_png() {
    // Implement extraction of message from png file here
    bool encryption_bit = 0;
    bool compression_bit = 0;
    
    size_t currentindex = 8;

    while (currentindex < input_buffer.str().size()) 
    {
        size_t chunkSize = 
                (input_buffer.str()[currentindex] << 24) |
                (input_buffer.str()[currentindex + 1] << 16) |
                (input_buffer.str()[currentindex + 2] << 8) |
                (input_buffer.str()[currentindex + 3]);
        currentindex += 4;

        if (input_buffer.str()[currentindex] == 'I' && input_buffer.str()[currentindex + 1] == 'D' &&
                input_buffer.str()[currentindex + 2] == 'A' && input_buffer.str()[currentindex + 3] == 'T')
        {
            currentindex += 4;

            encryption_bit = input_buffer.str()[currentindex] & 0x01;
            compression_bit = input_buffer.str()[currentindex] & 0x02;
            currentindex += 17;

            size_t messageSize =
                (input_buffer.str()[currentindex] << 24) |
                (input_buffer.str()[currentindex + 1] << 16) |
                (input_buffer.str()[currentindex + 2] << 8) |
                (input_buffer.str()[currentindex + 3]);
            currentindex += 4;

            for (size_t i = 0; i < messageSize; ++i)
            {
                unsigned char messageByte = 0;

                for (int j = 0; j < 4; ++j) 
                {
                    unsigned char pattern = input_buffer.str()[currentindex + j] & 0x03;
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
        else 
        {
            currentindex += chunkSize + 4;
        }
    }
}
