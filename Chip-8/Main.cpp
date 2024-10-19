#include <iostream>
#include "Chip8.h"
#include <chrono>
#include <thread>



int main() {
	Chip8 chip8 = Chip8();
	
	if (chip8.load("1-chip8-logo.ch8")) {
		for (int i = 0; i < 40; i++)
		{
			chip8.execution_cycle();
			std::this_thread::sleep_for(std::chrono::microseconds(5000));
		}
	}
	
	
	
	return 0;
}