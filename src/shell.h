#ifndef SHELL_H
#define SHELL_H

#define SHELL_INPUT_BUFFER_SIZE 256
#define COMMAND_HISTORY_MAX_SIZE 20

class CommandHistory
{
    public:
        CommandHistory();
        int at(const char **string, int index);
        bool empty();

    private:
        const char *history[COMMAND_HISTORY_MAX_SIZE];
        int head;
        int size;
};

class Shell
{
    public:
        Shell(CommandHistory cmd_history, volatile char *video_pointer, int w, int h);
        void clear();
        void input(char c);
        void output(const char *string);
        void putchar(char c);
        void ready_input();
        
    private:
        CommandHistory command_history;
        volatile char *video_pointer_origin, *video_pointer_current;
        int width, height;
        char buffer[SHELL_INPUT_BUFFER_SIZE];
        int buffer_ptr;
        int history_ptr;
        void clear_buffer();
        void reset_pointer();
        void handle_scroll();
        void scroll();
};

#endif
