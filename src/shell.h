#ifndef SHELL_H
#define SHELL_H

#define SHELL_INPUT_BUFFER_SIZE 256

class Shell
{
    public:
        Shell(volatile char *video_pointer, int w, int h);
        void clear();
        void input(char c);
        void output(const char *string);
        void ready_input();
        
    private:
        volatile char *video_pointer_origin, *video_pointer_current;
        int width, height;
        char buffer[SHELL_INPUT_BUFFER_SIZE];
        int buffer_ptr;
        void clear_buffer();
        void reset_pointer();
        void handle_scroll();
        void scroll();
        void putchar(char c);
};

#endif
