#ifndef MAIN_H
#define MAIN_H

#include <sstream>
#include <string>
#include <vector> 
using namespace std;

class stego {

    public:
        // Constructor
        stego(vector<string> &files, int encrypt, int compress);

        // Main utility functions of the tool
        // Deals with only the message buffer
        void compress();
        void decompress();
        void AESdecrypt();
        void AESencrypt();
        
        //Standalone compression/decompression
        void standalone_compress();
        void standalone_decompress();


        // Deals with the steganography bit
        // Convenient wrappers for the actual thing
        void hide();
        void extract();

        // Actual implementations
        void hide_png();
        void hide_wav();
        void extract_png();
        void extract_wav();


        //Helper functions
        void fetch_input();  // Gets input from file to input buffer
        void fetch_message(); // Gets message from file to message buffer
        void write_output(); // Writes the output buffer to the output file

        int check_if_wav_or_png(string &file_path); 
        // 1 = png
        // 2 = wav
        // 0 = other

        void print_input();
        //for testing/debug purposes, prints the input buffer to stdout

        void check_ce(); // Check if the hidden message is compressed and/or encrypted

        //NOTE: we might make more functions later as well, eg. to copy the message buffer to output/input directly etc.
    

    private:
        bool compression_bit; //Equals 1 in case we should compress / message is compressed
        bool encryption_bit; // Equals 1 in case we should encrypt / message is encrypted
        string input_path;
        string message_path;
        string output_path;
        stringstream input_buffer;
        stringstream message_buffer;
        stringstream output_buffer;
};

#endif
