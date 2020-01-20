#ifndef SHELL_H
#define SHELL_H

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
        char buffer[256];
        int buffer_ptr;
        void clear_buffer();
        void reset_pointer();
        void scroll();
        void putchar(char c);
};

#endif
