gcc -c linenoise/linenoise.c -o linenoise.o
g++ cppcli.cpp linenoise.o -o cppcli


To add x space tabs, x defined in .env file

```cpp
case TAB:
    for (int i=0; i<TAB_WIDTH; i++) {
        if (linenoiseEditInsert(l,' ')) return NULL;
    }
    break;
```

To exit the file mode with ctrl+x

```cpp
case CTRL_X:
    if (linenoiseEditInsert(l,CTRL_X)) return NULL;
    history_len--;
    free(history[history_len]);
    if (mlmode) linenoiseEditMoveEnd(l);
    if (hintsCallback) {
        /* Force a refresh without hints to leave the previous
            * line as the user typed it after a newline. */
        linenoiseHintsCallback *hc = hintsCallback;
        hintsCallback = NULL;
        refreshLine(l);
        hintsCallback = hc;
    }
    return strdup(l->buf);
```