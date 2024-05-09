#include "../include/stego_main.h"

using namespace std;

void stego::compress() {
    // Assuming 'message_buffer' is a member variable of the 'stego' class

    string input;
    getline(message_buffer, input); // Read the entire content of message_buffer into input

  
    vector<string> dictionary;
    for (int i = 0; i < 256; ++i) {
        string ch(1, static_cast<char>(i));
        dictionary.push_back(ch);
    }

    string output;
    string current_string;
    for (char c : input) {
        string combined = current_string + c;
        if (find(dictionary.begin(), dictionary.end(), combined) != dictionary.end()) {
            // If combined is already in the dictionary, extend the current string
            current_string = combined;
        } else {
            // Otherwise, output the code for the current string and add combined to the dictionary
            output += to_string(find(dictionary.begin(), dictionary.end(), current_string) - dictionary.begin()) + " ";
            if (dictionary.size() < 4096) {
                dictionary.push_back(combined);
            }
            current_string = string(1, c); // Start a new string with the current character
        }
    }

    // Output the code for the last current_string
    if (!current_string.empty()) {
        output += to_string(find(dictionary.begin(), dictionary.end(), current_string) - dictionary.begin());
    }

    // Overwrite the message buffer with the compressed output
    message_buffer.clear(); // Clear the buffer
    message_buffer.str(""); // Clear the content
    message_buffer << output; // Write the compressed output
}
