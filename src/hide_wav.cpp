#include "../include/stego_main.h"

void stego::hide_wav() {
    //implement hiding in wav files
      size_t pointer = 0;

    output_buffer<<input_buffer.str().substr(pointer, 12);
    pointer+=12;
    
    while(pointer<input_buffer.str().size()){
    
        string subchunk_id = input_buffer.str().substr(pointer, 4);     
        pointer+=4;
        int subchunk_size = *(int*)&input_buffer.str()[pointer];
        pointer+=4;
    
        if(subchunk_id != "data"){
            output_buffer<<subchunk_id;
            output_buffer.write(&input_buffer.str()[pointer-8], 8);
            output_buffer.write(&input_buffer.str()[pointer], subchunk_size);
            pointer+=subchunk_size;
        }
        else{  
            output_buffer << "data";
            output_buffer.write((char*)&subchunk_size, 4);

            char first_byte=0;
            if (encryption_bit){
                first_byte |= 0x01;
            } 
            if (compression_bit){
                first_byte |= 0x02;
            }  
            output_buffer.put(first_byte);


            int message_size = message_buffer.str().size();
            output_buffer.write((char*)&message_size, 4);

            
            for(int i=0; i<message_size; ++i){
                char message_byte = message_buffer.get();
                for (int j=0; j<4; ++j){
                    char input_byte = input_buffer.get();
                    input_byte &= 0xFC;  
                    input_byte |= (message_byte>>(6-2*j)) & 0x03;  
                    output_buffer.put(input_byte);
                }
            }
            pointer+=subchunk_size;
        }
    }   
}


