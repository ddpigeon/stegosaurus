#include "../include/stego_main.h"
#include <magic.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


// Implementations of some stego functions

stego::stego(vector<string> &files, int encrypt, int compress) {

    // Class constructor
    // files = {input, output, message}
    // message may/may not be there

    compression_bit = compress;
    encryption_bit = encrypt;

    input_path = files[0];
    output_path = files[1];
    if (files.size() == 3) {
        message_path =  files[2];
    }
}


// Hide and extract
void stego::hide() {
    if (check_if_wav_or_png(input_path) == 0) throw runtime_error("Invalid input file");
    fetch_input();
    fetch_message();

    // Order of running is compress -> encrypt -> hide

    if (compression_bit) compress();  
    if (encryption_bit) AESencrypt();

    if (check_if_wav_or_png(input_path) == 1) hide_png();
    else if (check_if_wav_or_png(input_path) == 2) hide_wav();
    write_output();
}

void stego::extract() {
    if (check_if_wav_or_png(input_path) == 0) throw runtime_error("Invalid input file");
    fetch_input();

    // Order is extract -> decrypt -> decompress

    if (check_if_wav_or_png(input_path) == 1) extract_png();
    else if (check_if_wav_or_png(input_path) == 2) extract_wav();

    if (encryption_bit) AESdecrypt();
    if (compression_bit) decompress();
}

void stego::standalone_compress() {
    fetch_input();
    message_buffer << input_buffer.rdbuf();
    compress();
    output_buffer << message_buffer.rdbuf();
    write_output();
}

void stego::standalone_decompress() {
    fetch_input();
    message_buffer << input_buffer.rdbuf();
    decompress();
    output_buffer << message_buffer.rdbuf();
    //cout << output_buffer.rdbuf();
    write_output();
}


int stego::check_if_wav_or_png(string &file_path) {

    // Recognition of files using metadata/magic numbers
    // https://man7.org/linux/man-pages/man3/libmagic.3.html

    auto handle = magic_open(MAGIC_NONE);
    magic_load(handle, NULL);
    auto typ = magic_file(handle, file_path.c_str());
    string filetype = typ;

    if (filetype.find("PNG") != string::npos) return 1; 
    else if (filetype.find("WAVE audio") != string::npos) return 2;
    return 0;
}

void stego::write_output() {
    // Writes output buffer to output file

    ofstream outputfile(output_path, ios::binary);
    outputfile << output_buffer.rdbuf();
    outputfile.close();
}

void stego::fetch_input() {
    // Reads from input file to input buffer

    //cout << input_path << endl;
    ifstream inputfile(input_path, ios::binary);
    if (inputfile) {
        input_buffer << inputfile.rdbuf();
        inputfile.close();
    }
}

void stego::fetch_message() {
    // Reads from message file to message buffer

    ifstream messagefile(message_path, ios::binary);
    message_buffer << messagefile.rdbuf();
    messagefile.close();
}

void stego::print_input() {
    cout << input_buffer.rdbuf();
}

void stego::check_ce() {
    // Implement this after stego hiding, since the exact specifics of the format we're hiding in might change
}

     

