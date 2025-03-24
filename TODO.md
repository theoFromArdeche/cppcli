

- file : being able to navigate the file and make changes

enter file mode : set the file_mode varibale to 1 

new call of linenoise : initialize the state with the element in the history if needed

arrow up : 
    if possible, increase the index_history variable,
        save the current line in the history (at the correct index)
        save the current position of the cursor
        returns it with an indicator that tells the main.cpp that arrow up was called  


arrow down :
    if possible, decrease the index_history variable,
        save the current line in the history (at the correct index)
        save the current position of the cursor
        returns it with an indicator that tells the main.cpp that arrow down was called

 
enter : 
    does it work ?
    if no : print the whole stack_after after a '\n'

    need to update history_file_len as well as history_len when press enter

exit file mode : set the file_mode varibale to 0 and remove the file part in the history


issue : the cursor should be working the same as in .txt for example when going up or down (not always same pos but its the same if possible)


issue : strange space at index 0 in file mode


TODO : send a signal after arrow up/down to tell the cpp (char at the end of the line, like ctrl-d works)

TODO : do the cpp part of the new file mode 
    receive arrows up and down and modify the right line
    (maybe use two stack, stack_before and stack_after)