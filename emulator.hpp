#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

using namespace std;


class Emulator {
    public:
        Emulator();
        ~Emulator();
        void loadGame(string filename);
        void emulationCycle();
        uint16_t get_pc();
        typedef void (Emulator::*func_t)();
        void call();
        void jump();
        void flow();
        void rtonum_equal();
        void rtonum_not_equal();
        void rtor_equal();
        void set_rtonum();
        void add_numtor();
        void oper();
        void rtor_not_equal();
        void set_itonum();
        void flow_v0_num();
        void set_rand();
        void draw();
        void keyop();
        void miscop();

        uint8_t gfx[64*32];
        uint8_t key[16];
        bool drawFlag;


    private:
        uint8_t internalMemory[4096];
        uint8_t V[16];
        uint16_t opcode;

        uint16_t I;
        uint16_t pc, stack[16], sp;

        uint8_t delay_timer;
        uint8_t sound_timer;

        vector<func_t> funcs;
        void init_states();
        void init_funcs();

};








