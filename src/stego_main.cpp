#include "../include/stego_main.h"
#include <magic.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;


// Implementations of some stego functions

stego::stego(vector<string> &files, int encrypt, int compress) {

    // Class constructor
    // files = {input, output, message}
    // message may/may not be there

    is_compressed = compress;
    is_encrypted = encrypt;

    string input_path = files[0];
    string output_path = files[1];
    if (files.size() == 3) {
        string message_path =  files[2];
    }
}


int stego::check_if_wav_or_png(string &file_path) {
    //ifstream inputfile(file_path);
    auto handle = magic_open(MAGIC_NONE);
    magic_load(handle, NULL);
    auto type = magic_file(handle, file_path.c_str());
    cout << type << endl;
    return 0;
}
     
