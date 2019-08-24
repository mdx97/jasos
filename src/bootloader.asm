; Allows our code to be run in real mode.
BITS 16

BootMessage: db "Booting..."
End:

section .text
global _start
_start:

jmp Start

; Moves the cursor to row dl, col dh.
MoveCursor:
        mov ah, 2
        mov bh, 0
        int 10h
        ret

; Prints the character in al to the screen.
PrintChar:
        mov ah, 10
        mov bh, 0
        mov cx, 1
        int 10h
        ret

; Set cursor position to 0, 0.
ResetCursor:
        mov dh, 0
        mov dl, 0
        call MoveCursor
        ret

; Initialize registers for 10h interrupts.
Start:	call ResetCursor

; Clears the screen before we print the boot message.
; QEMU has a bunch of crap on the screen when booting.
Clear:
        mov al, ' '
        call PrintChar

        inc dl
        call MoveCursor

        cmp dl, 80
        jne Clear

        mov dl, 0
        inc dh
        call MoveCursor

        cmp dh, 25
        jne Clear

; Begin printing the boot message. 
Msg:    call ResetCursor
        mov si, BootMessage

NextChar:
        lodsb
        call PrintChar

        inc dl
        call MoveCursor

        cmp si, End
        jne NextChar 

; Zerofill up to 510 bytes
times 510 - ($ - $$)  db 0

; Boot Sector signature
dw 0AA55h