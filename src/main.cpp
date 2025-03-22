#include <bits/stdc++.h>
#include "../include/linenoise.h"

using namespace std;


void ensureFilesExist(const string& basePath) {
    if (!filesystem::exists(basePath + "/../code")) {
        filesystem::create_directory(basePath + "/../code");
    }

    for (const string& path : {"/../code/code.cpp", "/../code/compile_errors.txt", "/../code/output.txt"}) {
        ofstream codeFile(basePath + path, ios::app);
        if (!codeFile) {
            cerr << "Error creating " << path << endl;
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

// Compile and run the code
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
        if (!hasOutput) hasOutput = 1;
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
    cout << "  - toggle: In multiline mode, toggle between Context Mode and Temporary Mode.\n";
    cout << "      - Context Mode: New code is appended to the existing code.\n";
    cout << "      - Temporary Mode: New code is executed without affecting the existing context.\n";
    cout << "            Note: Changes made in Temporary Mode are not saved after execution.\n";
    cout << "  - help: Show this help message.\n";
    cout << "  - exit: Exit the program.\n";
    cout << "You can also type C++ code directly at the prompt and press Enter to execute it.\n\n";
}


int readTabSizeFromEnv(const string& basePath) {
    ifstream envFile(basePath + "/../.env");
    string line;
    while (getline(envFile, line)) {
        if (line.find("TAB_SIZE=") == 0) {
            string tabSizeStr = line.substr(string("TAB_SIZE=").size());
            return stoi(tabSizeStr);
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
    if (!ifsTemplate) {
        cerr << "Error: Could not open templateTop.txt\n";
        return 1;
    }
    templateTop.assign((istreambuf_iterator<char>(ifsTemplate)), istreambuf_iterator<char>());
    ifsTemplate.close();

    ifstream ifsTemplateBottom(basePath + "/../templates/templateBottom.txt");
    if (!ifsTemplateBottom) {
        cerr << "Error: Could not open templateBottom.txt\n";
        return 1;
    }
    templateBottom.assign((istreambuf_iterator<char>(ifsTemplateBottom)), istreambuf_iterator<char>());
    ifsTemplateBottom.close();


    int tabSize = readTabSizeFromEnv(basePath);
    linenoiseSetTabSize(tabSize);
    linenoiseSetMultiLine(1);
    const char* noNewlineText = "toggle";
    linenoiseSetNoNewlineText(noNewlineText);


    bool contextMode = true;  // Default to Context Mode
    while (true) {
        char* input = linenoise(">>> ");
        if (!input) exit(0);

        line = string(input);
        free(input);

        if (line.empty()) continue;
        linenoiseHistoryAdd(line.c_str());

        string previousCode = code;
        if (line == "file") {
            cout << "\033[F>>> file (Ctrl+X to exit) (Mode: " << (contextMode ? "Context" : "Temporary") << ")\n";
            line = "";
            int historyCount = 1;

            while (true) {
                char* temp = linenoise("");
                if (!temp) break;

                string tempStr(temp);
                free(temp);

                if (tempStr == "toggle") {
                    contextMode = !contextMode;
                    cout << "\033[" << historyCount << "F\033[K>>> file (Ctrl+X to exit) (Mode: " << (contextMode ? "Context" : "Temporary") << ")\n";
                    cout << "\033[" << historyCount - 1 << "E\033[K" << flush;
                    continue;
                }

                line += "\n    " + tempStr;
                historyCount++;

                if (tempStr.back() == '\x18') {  // Ctrl+X
                    line.pop_back();
                    break;
                }
            }
            cout << ">>> End of file\n";

            if (contextMode) code += "\n    " + line;
            else code = line;

        } else if (line == "clear") {
            cout << "\033[2J\033[H" << flush;
            continue;
        } else if (line == "reset") {
            code = "";
            continue;
        } else if (line == "help") {
            showHelp();
            continue;
        } else if (line == "exit") {
            exit(0);
        } else {
            code += "\n    " + line;
        }


        ofstream codeFileStream(basePath + "/../code/code.cpp");
        if (!codeFileStream) {
            cerr << "Error: Could not open code.cpp\n";
            return 1;
        }
        codeFileStream << templateTop << code << templateBottom;
        codeFileStream.close();

        int result = runCode(basePath);
        if (result || !contextMode) code = previousCode;
        
    }

    return 0;
}