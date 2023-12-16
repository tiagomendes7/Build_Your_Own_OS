
#include "gdt.h"
#include "interrupts.h"

// 
// Clean screen
//
void clean_screen(int maxCol, int maxLine, uint16_t* VideoMemory){

    for(int line = 0; line < maxLine; line++){
        for(int col = 0; col < maxCol; col++){
            // replace each byte with empty string (' ')
            VideoMemory[maxCol*line + col] = (VideoMemory[maxCol*line + col] & 0xFF00) | ' ';
        }
    }
}

// 
// SMALL Printf function
//
void printf(char* str){
 
    static uint16_t* VideoMemory = (uint16_t*) 0xb8000;
    static int maxCol = 80, maxLine = 25, line = 0, col = 0;

    for(int i = 0; str[i] != '\0'; i++){
        if( str[i] != '\n' )
            VideoMemory[maxCol*line + col] = (VideoMemory[maxCol*line + col] & 0xFF00) | str[i];
        
        // write one character and go to next column
        // if you reach end of column, go to next line (screen is 80 x 25)
        // if you reach end of screen, 
        col++;

        if( col >= maxCol || str[i] == '\n' ){
            line++; 
            col = 0;
        }
        if( line >= maxLine ){
            clean_screen(maxCol, maxLine, VideoMemory);
            line = col = 0;    
        }

    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
} 

//
// Kernel Main Function
//
extern "C" void kernelMain(void* multiboot_struct, uint32_t magicnumber){

    for (int i = 0; i < 5; i++)
    {
        printf("Hello World, my OS.\n");
    }
    
    printf("I cleaned your screen\n");

    // Global Descriptor Table
    GDT gdt;
    
    // Interruptor Descriptor Table
    InterruptManager interrupts(&gdt);
    
    interrupts.Activate();

    while(1);
}