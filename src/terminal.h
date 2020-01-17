#ifndef TERMINAL_H
#define TERMINAL_H

class Terminal
{
    public:
        Terminal(volatile char *video_pointer, int w, int h);
        void init();
        void clear();
        void print(const char *string);
        void putchar(char c);

    private:
        void reset_pointer();
        volatile char *video_pointer_origin, *video_pointer_current;
        int width, height;

};

#endif
