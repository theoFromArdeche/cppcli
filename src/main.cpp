#include <bits/stdc++.h>
#include "../include/linenoise.h"

using namespace std;

void ensureFilesExist(const std::string& basePath) {
    if (!filesystem::exists(basePath + "/../code")) {
        filesystem::create_directory(basePath + "/../code");
    }

    for (string path:vector<string>{"/../code/code.cpp", "/../code/compile_errors.txt", "/../code/output.txt"}) {
        std::ofstream codeFile(basePath + path, std::ios::app);
        if (!codeFile) {
            std::cerr << "Error creating " << path << std::endl;
        } else {
            codeFile.close();
        }
    }
}


string getExecutablePath() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    string fullPath = (count != -1) ? string(result, count) : "";
    return fullPath.substr(0, fullPath.find_last_of('/'));
}


int runCode(const string& basePath) {
    string compileCommand = "g++ " + basePath + "/../code/code.cpp -o " + basePath + "/../code/code 2> " + basePath + "/../code/compile_errors.txt";
    int compileStatus = system(compileCommand.c_str());

    if (compileStatus != 0) {
        ifstream errorFile(basePath + "/../code/compile_errors.txt");
        string errorLine;
        cout << "Compilation failed!\n";
        while (getline(errorFile, errorLine)) {
            cout << errorLine << endl;
        }
        return 1;
    }

    string runCommand = basePath + "/../code/code > " + basePath + "/../code/output.txt 2>&1";
    int runStatus = system(runCommand.c_str());

    if (runStatus != 0) {
        cout << "Error while running the program." << endl;
        return 1;
    }

    ifstream outputFile(basePath + "/../code/output.txt");
    string outputLine;
    int hasOutput = 0;
    
    while (getline(outputFile, outputLine)) {
        cout << outputLine << endl;
        if (!hasOutput) hasOutput = 1;;
    }

    return hasOutput;
}



void showHelp() {
    cout << "\nWelcome to the C++ CLI!\n\n";
    cout << "This program allows you to write and execute C++ code interactively.\n";
    cout << "Available commands:\n";
    cout << "  - file: Enter multiline code. Type 'file' and press Enter, then start writing your code.\n";
    cout << "          Press Ctrl+X to exit and execute the code.\n";
    cout << "  - clear: Clear the terminal screen.\n";
    cout << "  - reset: Reset the code buffer (clear all previously entered code).\n";
    cout << "  - help: Show this help message.\n";
    cout << "  - exit: Exit the program.\n";
    cout << "You can also type C++ code directly at the prompt and press Enter to execute it.\n\n";
}



int readTabWidthFromEnv(string basePath) {
    ifstream envFile(basePath+"/../.env");
    string line;
    while (getline(envFile, line)) {
        if (line.find("TAB_WIDTH=") == 0) {
            string tabWidthStr = line.substr(10);
            return std::atoi(tabWidthStr.c_str());
        }
    }
    return 4;
}



int main(int argc, char** argv) {
    string basePath = getExecutablePath();
    ensureFilesExist(basePath);

    string code, line, templateTop, templateBottom;

    cout << "C++ CLI (v1.0.0) - Type 'help' for a list of commands.\n";

    ifstream ifsTemplate(basePath + "/../templates/templateTop.txt");
    if (ifsTemplate) {
        templateTop.assign((istreambuf_iterator<char>(ifsTemplate)), istreambuf_iterator<char>());
    }
    ifsTemplate.close();


    ifstream ifsTemplateBottom(basePath + "/../templates/templateBottom.txt");
    if (ifsTemplateBottom) {
        templateBottom.assign((istreambuf_iterator<char>(ifsTemplateBottom)), istreambuf_iterator<char>());
    }
    ifsTemplateBottom.close();


    const int tabSize=readTabWidthFromEnv(basePath);
    linenoiseSetTabSize(tabSize);
    linenoiseSetMultiLine(1);

    while (true) {
        char* input = linenoise(">>> ");
        if (!input) exit(0);
        
        line = string(input);
        free(input);
        
        if (line.empty()) continue;
        linenoiseHistoryAdd(line.c_str());

        if (line=="file") {
            cout << "\033[F>>> file (Ctrl+X to exit)\n";
            line="";
            while (true) {
                char* temp = linenoise("");
                if (!temp) break;
                line += "\n    " + string(temp);
                if (string(temp).back()==24) { // Ctrl-X
                    line.pop_back();
                    break;
                }
            }
            cout << ">>> End of file\n";
        } else if (line=="clear") {
            cout << "\033[2J\033[H" << flush;
            continue;
        } else if (line=="reset") {
            code="";
            continue;
        } else if (line == "help") {
            showHelp();
            continue;
        } else if (line == "exit") {
            exit(0);
        }

        string previousCode = code;
        code += "\n    " + line;

        ofstream codeFileStream(basePath + "/../code/code.cpp");
        codeFileStream << templateTop << code << templateBottom;
        codeFileStream.close();

        int result = runCode(basePath);
        if (result) code = previousCode;
    }

    return 0;
}