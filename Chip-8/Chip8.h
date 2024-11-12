#include <stdint.h>
#include <string>
#include "SDL2/SDL.h"
class Chip8
{
public:
	
   
    bool drawFlag;
    bool load(const char *fileName);
    void TestLoop();
    uint8_t Display[64 * 32];
    uint8_t  key[16];
    bool ifPressed;
    bool clippingQuirk;
    bool jumpingQuirk;
    bool shiftingQuirk;
    bool memoryQuirk;
    bool vfQuirk;
    uint8_t DelayTimer = 0;
    uint8_t SoundTimer = 0;
    Chip8();
    ~Chip8();

    void execution_cycle();
    
    void updateTimers();
    void freeAudio();

private:
    uint8_t Memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t Stack[16];
    uint16_t sp;
    
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;
    SDL_AudioDeviceID deviceId;
    
    uint16_t Pc;
    void init();
    bool audioLoaded;
    void loadAudio();
    void playAudio();
    void pauseAudio();
    
};


