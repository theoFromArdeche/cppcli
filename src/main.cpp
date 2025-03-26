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
int runCode(const string& basePath, bool flagCompile=true) {
    string compileCommand = "g++ " + basePath + "/../code/code.cpp -o " + basePath + "/../code/code 2> " + basePath + "/../code/compile_errors.txt";
    
    int compileStatus = 0;
    if (flagCompile) compileStatus = system(compileCommand.c_str());

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

    return hasOutput ? 2 : 0;
}

void showHelp() {
    cout << "\nC++ CLI HELP\n";
    cout << "-----------\n\n";
    
    cout << "HOW CODE RUNS:\n";
    cout << "• Script Mode (default):\n";
    cout << "  - Immediate execution\n";
    cout << "  - Output displays ONCE (like Python REPL)\n";
    cout << "  - Output-producing lines aren't saved\n\n";
    
    cout << "• File Mode (type 'file'):\n";
    cout << "  - Multi-line editing (Ctrl+X to exit and run)\n";
    cout << "  - Two ways to run:\n";
    cout << "    1) CONTEXT MODE: Shares variables with Script Mode\n";
    cout << "    2) TEMPORARY MODE: Runs independently - no variable sharing\n\n";
    
    cout << "KEY DIFFERENCES:\n";
    cout << "┌──────────────┬──────────────────────────┬──────────────────────┐\n";
    cout << "│              │ Script Mode              │ File Mode            │\n";
    cout << "├──────────────┼──────────────────────────┼──────────────────────┤\n";
    cout << "│ Output       │ Shows once               │ Shows every execution│\n";
    cout << "├──────────────┼──────────────────────────┼──────────────────────┤\n";
    cout << "│ Persistence  │ Only non-output lines    │ All lines saved      │\n";
    cout << "├──────────────┼──────────────────────────┼──────────────────────┤\n";
    cout << "│ Best for     │ Test Syntax, calculations│ Complex code, output │\n";
    cout << "└──────────────┴──────────────────────────┴──────────────────────┘\n\n";
    
    cout << "ESSENTIAL COMMANDS:\n";
    cout << "• file  -  Enter file mode\n";
    cout << "• clear  -  Clean terminal\n";
    cout << "• reset  -  Clear ALL code\n";
    cout << "• reset script  -  Clear only script\n";
    cout << "• reset file  -  Clear only file code\n";
    cout << "• help  -   Show this help\n";
    cout << "• exit  -   Quit\n\n";
    
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


bool containsOutput(const string& code) {
    const vector<string> outputPatterns = {
        "cout", "printf", "cerr", "puts", "fprintf"
    };

    for (const string& pattern:outputPatterns) {
        if (code.find(pattern) == string::npos) continue;
        return true;
    }

    return false;
}


int main(int argc, char** argv) {
    string basePath = getExecutablePath();
    ensureFilesExist(basePath);

    string templateTop, templateBottom;

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
    bool fileMode = false;
    string file = "", script = "", line;
    int result=1;
    bool runOnEnter=true; // option
    while (true) {
        char* input = linenoise(">>> ");
        if (!input) exit(0);

        line = string(input);
        free(input);

        if (line.empty()) {
            if (runOnEnter && result==2) {
                runCode(basePath, false);
            }
            continue;
        }
        linenoiseHistoryAdd(line.c_str());

        string previousScript = script;
        if (line == "file") {
            fileMode = true;
            cout << "\033[F>>> file (Ctrl+X to exit and run) (Mode: " << (contextMode ? "Context" : "Temporary") << ")\n";
            linenoiseSetMode(1);
            linenoiseSetMultiLine(0);

            while (true) {
                char* temp = linenoise("");
                if (!temp) break;

                string tempStr(temp);
                free(temp);

                if (tempStr.back() == '\x14') { // Ctrl+T
                    contextMode = !contextMode;
                    int history_file_len=0, history_file_index=0;
                    linenoiseGetFileHistorySizes(&history_file_len, &history_file_index);

                    cout << "\033[" << history_file_len - history_file_index << "F\033[K>>> file (Ctrl+X to exit and run) (Mode: " << (contextMode ? "Context" : "Temporary") << ")";
                    cout << "\033[" << history_file_len - history_file_index << "E\033[K" << flush;
                    continue;
                }

                if (tempStr.back() == '\x18') {  // Ctrl+X
                    break;
                }

            }

            int history_file_len=0, history_file_index=0;
            linenoiseGetFileHistorySizes(&history_file_len, &history_file_index);
            char** history_file = linenoiseGetHistoryFile();

            cout << "\033[" << history_file_index << "E\n";
            cout << ">>> End of file\n";

            file = "\n    ";
            for (int i=history_file_len-1; i>=0; i--) {
                file += string(history_file[history_file_len-1-i]) + "\n    ";
            }

            //cout << "\n|||\n" << line << "\n|||\n";

            linenoiseSetMode(0);
            linenoiseSetMultiLine(1);

        } else if (line == "clear") {
            cout << "\033[2J\033[H" << flush;
            continue;
        } else if (line == "reset script") {
            if ((contextMode && !script.empty()) || !fileMode) result = 1;
            script = "";
            cout << ">>> Script has been reset!\n";
            continue;
        } else if (line == "reset file") {
            file = "";
            result = 1;
            linenoiseResetFileHistory();
            cout << ">>> File has been reset!\n";
            continue;
        } else if (line == "reset") {
            script = "";
            file = "";
            result = 1;
            linenoiseResetFileHistory();
            cout << ">>> Script and File has been reset!\n";
            continue;
        } else if (line == "help") {
            showHelp();
            continue;
        } else if (line == "exit") {
            exit(0);
        } else {
            script += "\n    " + line;
            if (fileMode) fileMode=false;
        }


        ofstream codeFileStream(basePath + "/../code/code.cpp");
        if (!codeFileStream) {
            cerr << "Error: Could not open code.cpp\n";
            return 1;
        }

        if (contextMode) {
            codeFileStream << templateTop << file << script << templateBottom;
        } else if (fileMode)  {
            codeFileStream << templateTop << file << templateBottom;
        } else {
            codeFileStream << templateTop << script << templateBottom;
        }
        
        codeFileStream.close();

        result = runCode(basePath); // 0 -> compiled, no output | 1 -> did not compile | 2 -> compiled, output
        if (result==1 || (!fileMode && containsOutput(line))) {
            // did not compile
            // or in script mode and there is an output
            script = previousScript;
        }
        
    }

    return 0;
}