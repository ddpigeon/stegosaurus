#include <iostream>
#include <fstream>
#include <vector>
#include "../include/stego_main.h"
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;


int main(int ac, char* av[]) { 
    // Stuff to parse command line options
    // https://www.boost.org/doc/libs/1_84_0/doc/html/program_options/tutorial.html

    po::options_description desc("Allowed options: ");
    desc.add_options()
        ("help,h", "display help page")
        ("input-file,if", po::value<string>(), "Input file path")
        ("input-message,im", po::value<string>(), "Input message file path")
        ("output-file,o", po::value<string>(), "Output file path")
        ("encrypt,e", "Encrpyt")
        ("compress,c", "Compress")
        ("perform-command,pc", po::value<string>()->required(), "Selects which function to perform")
        ;

    po::positional_options_description p;
    p.add("perform-command", -1);


    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).options(desc).positional(p).run(), vm);
    po::notify(vm);


    if (vm.count("help") || (vm.count("perform-command") && vm["perform-command"].as<string>() == "h")) {
        // Read help text, and output it
        ifstream helptext("help.txt");
        if (helptext.is_open()) cout << helptext.rdbuf();
        return 0;
    }

    // Deals with whether we hide or extract
    // func = 1  ==> hide the file
    // func = 2  ==> extract from file
    int func;
    if (vm.count("perform-command")) {
        string cmnd = vm["perform-command"].as<string>();
        if (cmnd == "hf") func = 1;
        else if (cmnd == "ef") func = 2;
        else if (cmnd == "cf") func = 3;
        else if (cmnd == "df") func = 4;

        else if (cmnd == "h"); // do nothing, should be taken care of above 

        // Else the command is invalid, so we throw error
        else throw runtime_error("Invalid command!");
    }
        
    // Else the user hasn't given any command to do, which is an error
    else throw runtime_error("Command has not been provided!");

    // Variable declaration/definitions, fairly self explanatory
    string input_path;
    string message_path;
    string output_path;
    int do_encrypt = vm.count("encrypt");
    int do_compress = vm.count("compress");
    vector<string> file_paths;


    // Check that all 3 parameters are provided for hide file function
    if (func == 1) {
        if (vm.count("input-file")) {
            input_path = vm["input-file"].as<string>();
            file_paths.push_back(input_path);
        }
        else throw runtime_error("No input file path!");

        if (vm.count("output-file")) {
            output_path = vm["output-file"].as<string>();
            file_paths.push_back(output_path);
        }
        else throw runtime_error("No output file path!");

        if (vm.count("input-message")) {
            message_path = vm["input-message"].as<string>();
            file_paths.push_back(message_path);
        }
        else throw runtime_error("No input message file path!");
    }

    // Same as above, but for extract file, and the standalone compressions 
    else if (func == 2 || func == 3 || func == 4) {
        if (vm.count("input-file")) {
            input_path = vm["input-file"].as<string>();
            file_paths.push_back(input_path);
        }
        else throw runtime_error("No input file path!");

        if (vm.count("output-file")) {
            output_path = vm["output-file"].as<string>();
            file_paths.push_back(output_path);
        }
        else throw runtime_error("No output file path!");
    }

    //Instantiate an instance of the main class
    stego stegosaurus(file_paths, do_encrypt, do_compress);


    // Run hide/extract depending on function
    if (func == 1) {
        stegosaurus.hide();
    }

    else if (func == 2) {
        stegosaurus.extract();
    }

    else if (func == 3) {
        stegosaurus.standalone_compress();
    }

    else if (func == 4) {
        stegosaurus.standalone_decompress();
    }



    return 0;
}
