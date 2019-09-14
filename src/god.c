asm(".code16gcc");

// JASOS kernel entry point.
void let_there_be_light()
{
    asm(     "movb $0, %dl;"
             "inc %dh;"
             "movb $2, %ah;"
             "movb $0, %bh;"
             "int $0x10;"
             "movb $'a', %al;"
             "movb $10, %ah;"
             "movw $1, %cx;"
             "int $0x10;"   );
    
    while (1);
}