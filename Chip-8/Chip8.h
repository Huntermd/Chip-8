#include <stdint.h>
#include <string>

class Chip8
{
public:
	
   
    bool drawFlag;
    bool load(const char *fileName);
    void TestLoop();
    
    Chip8();
    ~Chip8();

    void execution_cycle();
    

    

private:
    uint8_t Memory[4096];
    uint8_t V[16];
    uint16_t I = 0;
    uint16_t Stack[16];
    uint16_t sp = 0;
    uint8_t Display[64 * 32];
    uint8_t DelayTimer = 0;
    uint8_t SoundTimer = 0;
    uint8_t  key[16];
    uint16_t Pc;
    void init();
};


