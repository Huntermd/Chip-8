#include "Chip8.h"
#include <string>
#include <iostream>
#include <fstream>



unsigned char FontSet[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};
Chip8::Chip8() {}
Chip8::~Chip8() {}

void Chip8::init() {
	Pc = 0x200;
	I = 0;
	sp = 0;
    drawFlag = false;
    for (int i = 0; i < 16; ++i) { //clear the stack
        Stack[i] = 0;
        V[i] = 0;
        key[i] = 0;

    }
    for (int i = 0; i < 4096; i++) {
        Memory[i] = 0;
    }

    for (int i = 0; i < 2048; i++) { // clear display
        Display[i] = 0;
    }

    for (int i = 0; i < 80; i++) {//Loads fonts into memory
        Memory[i] = FontSet[i];
    }
}

bool Chip8::load(const char *fileName) {
    init();
    std::ifstream file;
    std::streampos size; //streampos is the type returned by tellg()
    std::streampos begin;
    std::streampos end;
    char* memoryBlock;
    file.open(fileName, std::ifstream::binary|std::ios::ate); // ios::ate flag, which means that the get pointer will be positioned at the end of the file.
    if (!file.is_open()) {
        std::cerr << "couldn't open file";
        return false;
    }
    size = file.tellg();
    file.seekg(0, std::ios::beg);; // gets size of file
    
    
    // Sets the position of the next character to be extracted from the input stream. beg = beginning of the stream
    
    std::cout << "size of the file: " << size << '\n';
    memoryBlock = new char[size];
    file.read(memoryBlock, size);
    file.close();
    
    if ((4096 - 512) > size) {
        for (int i = 0; i < size; i++) {
            Memory[i + 512] = memoryBlock[i];
        }
        delete[] memoryBlock;
        return true;
    }
    else {
        std::cerr << "rom is too fat" << '\n';
        delete[] memoryBlock;
        return false;
    }
};

void Chip8::execution_cycle() {
    uint16_t opcode = Memory[Pc] << 8 | Memory[Pc + 1];
   
    switch (opcode & 0xF000){
    case 0x0000:
    {
        switch (opcode & 0x000F) {
        case 0x0000:
            for (int i = 0; i < 2048; i++) { // clear display
                Display[i] = 0;
            }
            std::cout << " clear screen" << (opcode) << '\n';
            drawFlag = true;
            Pc += 2;
            break;
            
        case 0x000E://Return from subroutine 
            sp--;
            Pc = Stack[sp];
            
            break;
        default:
            break;
        }
        break;
    }
    case 0x6000://Loads VX with NN
    {
        std::cout << " Loads VX with NN: " << ((opcode & 0x00FF) >> 8) << '\n';
         
        V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        Pc += 2;
        break;
    }
    
    case 0xA000://Loads register I with NNN
    {
        std::cout << " Loads register I with NNN" << '\n';
        I = opcode & 0x0FFF;
        Pc += 2;
        break;
    }
    case 0x1000://Jumps to address NNN
    {
        std::cout << "Jumps to address NNN" << '\n';
        Pc = opcode & 0x0FFF;
        break;
    }
    case 0xD000:
    {
        std::cout << "The big D" << '\n';
        V[0xf] = 0;
        unsigned short x = V[(opcode & 0x0F00) >> 8];
        unsigned short y = V[(opcode & 0x00F0) >> 4];
        unsigned short height = opcode & 0x000F;
        unsigned short pixels;
        for (int y_axis = 0; y_axis < height; y_axis++) {
            pixels = Memory[I + y_axis];
            for (int x_axis = 0; x_axis < 8; x_axis++) {
                if ((pixels & (0x80 >> x_axis)) != 0) {
                    if (Display[x + x_axis + ((y + y_axis) * 64)] == 1)
                    {
                        V[0xF] = 1;
                    }
                    Display[x + x_axis + ((y + y_axis) * 64)] ^= 1;
                }
            }

        }
        drawFlag = true;
        Pc += 2;
        break;
    }

    default:
        std::cout<<"opcode bad";

        break;
    }
}

void Chip8::TestLoop() {
    uint16_t test_code = Memory[Pc] << 8 | Memory[Pc + 1];
    std::cout << "this is the code: " << (test_code & 0xF000) << '\n';
    Pc += 2;
}