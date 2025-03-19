# **C++ CLI**

A simple command-line interface (CLI) for writing and executing C++ code interactively. This project allows you to write and run C++ code in real-time, with features like multiline input, tab spacing, and command history.

---

## **Features**

- **Interactive C++ Code Execution**:
  - Write and execute C++ code directly in the terminal.
  - Supports multiline input for writing functions, loops, and more.

- **Customizable Tab Width**:
  - The number of spaces inserted by the **Tab** key can be configured via a `.env` file.

- **Multiline Mode**:
  - Enter multiline mode by typing the `file` command.
  - Exit multiline mode by pressing **Ctrl+X**.

- **Command History**:
  - Use the **Up** and **Down** arrow keys to navigate through previously entered commands.

- **Clear Screen**:
  - Use the `clear` command to clear the terminal screen.

- **Reset Code Buffer**:
  - Use the `reset` command to clear all previously entered code.

---

## **How It Works**

The program uses the `linenoise` library for line editing and command history. It compiles and executes the entered C++ code in real-time, displaying the output in the terminal.

### **Key Implementation Details**

1. **Tab Width**:
   - The number of spaces inserted by the **Tab** key is configurable via the `TAB_WIDTH` variable in the `.env` file.
   - Example `.env` file:
     ```plaintext
     TAB_WIDTH=4
     ```

2. **Multiline Mode**:
   - When in multiline mode, the program collects input until **Ctrl+X** is pressed.
   - The current line is saved, and the program exits multiline mode.

3. **Command Handling**:
   - The program supports commands like `file`, `clear`, `reset`, and `help`.

---

## **How to Run**

### **Prerequisites**

- **GCC** (GNU Compiler Collection) or another C++ compiler.
- **CMake** (optional, for building the project).

### **Build and Run**

1. Clone the repository:
   ```bash
   git clone https://github.com/theoFromArdeche/cppcli.git
   cd cppcli
   ```

2. Build the project:
   - Using `gcc` and `g++`:
     ```bash
     cd src
     gcc -c linenoise.c -o linenoise.o
     g++ main.cpp linenoise.o -o cppcli
     ```
   - Using CMake:
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```

3. Run the program:
   ```bash
   ./cppcli
   ```


## **Add to PATH**

To make the `cppcli` executable available globally, you can add it to your system's **PATH**. Here’s how:

### **Linux/macOS**

1. Move the `cppcli` executable to a directory in your PATH, such as `/usr/local/bin`:
   ```bash
   sudo mv cppcli /usr/local/bin/
   ```

2. Alternatively, add the directory containing `cppcli` to your PATH:
   ```bash
   export PATH=$PATH:/path/to/cppcli
   ```

   To make this change permanent, add the above line to your shell configuration file (e.g., `~/.bashrc`, `~/.zshrc`).

3. Verify the executable is in your PATH:
   ```bash
   which cppcli
   ```

### **Windows**

1. Move the `cppcli.exe` executable to a directory in your PATH, such as `C:\Windows\System32`.

2. Alternatively, add the directory containing `cppcli.exe` to your PATH:
   - Open **System Properties** > **Environment Variables**.
   - Under **System Variables**, find the `Path` variable and click **Edit**.
   - Add the directory containing `cppcli.exe` to the list.

3. Verify the executable is in your PATH:
   ```cmd
   where cppcli
   ```

---

## **Example Usage**

1. Start the program:
   ```bash
   cppcli
   ```

2. Enter multiline mode:
   ```plaintext
   >>> file
   >>> file (Ctrl+X to exit)
   for (int i = 0; i < 6; i++) {
       cout << i << endl;
   }
   ^X
   >>> End of file
   0
   1
   2
   3
   4
   5
   >>> 
   ```

3. Clear the screen:
   ```plaintext
   >>> clear
   ```

4. Reset the code buffer:
   ```plaintext
   >>> reset
   ```

5. Exit the program:
   ```plaintext
   >>> exit
   ```

---


## **Customization**

### **Tab Width**

To change the number of spaces inserted by the **Tab** key, modify the `TAB_WIDTH` variable in the `.env` file:

```plaintext
TAB_WIDTH=8
```

---



## **Contributing**

Contributions are welcome! Please open an issue or submit a pull request.
