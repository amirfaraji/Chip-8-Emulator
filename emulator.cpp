/* EMULATOR */
#include "emulator.hpp"
#include <string.h>

unsigned char chip8_fontset[80] =
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

void Emulator::call(){
    cout << __func__ << endl;
    if ((opcode & 0x00FF) == 0x00EE){
        pc = stack[--sp];
        pc += 2;
    }
    else if ((opcode & 0x00FF) == 0x00E0){
        // clear screen
        memset(gfx, 0, sizeof(gfx));
        drawFlag = true;
        pc += 2;
    }
    else {
        printf("Error unknown opcode: 0x%4x \n", opcode);
        exit(1);
    }
}

void Emulator::jump(){
    cout << __func__ << endl;
    pc = opcode & 0x0FFF;
}

void Emulator::flow(){
    cout << __func__ << endl;
    stack[sp] = pc;
    sp++;
    pc = opcode & 0x0FFF;
}

void Emulator::rtonum_equal(){
    cout << __func__ << endl;
    if (V[(opcode >> 8) & 0x0F] == (opcode & 0x00FF)){
        pc += 2; // Skip next instruction
    }
    pc += 2;
}

void Emulator::rtonum_not_equal(){
    cout << __func__ << endl;
    if (V[(opcode >> 8) & 0x0F] != (opcode & 0x00FF)){
        pc += 2; // Skip next instruction
    }
    pc += 2;
}

void Emulator::rtor_equal(){
    cout << __func__ << endl;
    if (V[(opcode >> 8) & 0x0F] == V[(opcode >> 4) & 0x00F]){
        pc += 2; // Skip next instruction
    }
    pc += 2;
}

void Emulator::set_rtonum(){
    cout << __func__ << endl;
    V[(opcode >> 8) & 0x0F] = opcode & 0x00FF;
    pc += 2;
}

void Emulator::add_numtor(){
    cout << __func__ << endl;
    V[(opcode >> 8) & 0x0F] += opcode & 0xFF;
    pc += 2;
}

void Emulator::oper(){
    cout << __func__ << endl;
    switch(opcode & 0x000F){
        case 0x0: 
            V[(opcode >> 8) & 0x0F] = V[(opcode >> 4) & 0x00F];
            break;
        case 0x1:
            V[(opcode >> 8) & 0x0F] |= V[(opcode >> 4) & 0x00F];
            break;
        case 0x2:
            V[(opcode >> 8) & 0x0F] &= V[(opcode >> 4) & 0x00F];
            break;
        case 0x3:
            V[(opcode >> 8) & 0x0F] ^= V[(opcode >> 4) & 0x00F];
            break;
        case 0x4:
            V[(opcode >> 8) & 0x0F] += V[(opcode >> 4) & 0x00F];
            if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
                V[0xF] = 1; //carry
            else
                V[0xF] = 0;
            break;
        case 0x5:
            if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
                V[0xF] = 0; // there is a borrow
            else
                V[0xF] = 1;
            V[(opcode >> 8) & 0x0F] -= V[(opcode >> 4) & 0x00F];
            break;
        case 0x6:
            V[0xF] = V[(opcode >> 8) & 0x0F] & 0x0001;
            V[(opcode >> 8) & 0x0F] = V[(opcode >> 8) & 0x0F] >> 1;
            break;
        case 0x7:
            if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])	// VY-VX
                V[0xF] = 0; // there is a borrow
            else
                V[0xF] = 1;
            V[(opcode >> 8) & 0x0F] = V[(opcode >> 4) & 0x00F] - V[(opcode >> 8) & 0x0F];
            break;
        case 0xE:
            V[0xF] = V[(opcode >> 8) & 0x0F] >> 7;
            V[(opcode >> 8) & 0x0F] = V[(opcode >> 8) & 0x0F] << 1;
            break;
        default:
        break;
    }
    pc += 2;
}

void Emulator::rtor_not_equal(){
    cout << __func__ << endl;
    if (V[(opcode >> 8) & 0x0F] != V[(opcode >> 4) & 0x00F]){
        pc += 2; // Skip next instruction
    }
    pc += 2;
}

void Emulator::set_itonum(){
    cout << __func__ << endl;
    I = opcode & 0x0FFF;
    pc += 2;
}

void Emulator::flow_v0_num(){
    cout << __func__ << endl;
    pc = V[0] + (opcode & 0x0FFF);
}

void Emulator::set_rand(){
    cout << __func__ << endl;
    V[(opcode >> 8) & 0x0F] = (rand() % 256) & (opcode & 0x00FF);
    pc += 2;
}

void Emulator::draw(){
    cout << __func__ << endl;
    unsigned short x = V[(opcode >> 8) & 0x0F];
    unsigned short y = V[(opcode >> 4) & 0x00F];
    unsigned short height = opcode & 0x000F;
    unsigned short pixel;

    V[0xF] = 0;
    for (int yline = 0; yline < height; yline++)
    {
        pixel = internalMemory[I + yline];
        for(int xline = 0; xline < 8; xline++)
        {
            if((pixel & (0x80 >> xline)) != 0)
            {
                if(gfx[(x + xline + ((y + yline) * 64))] == 1)
                {
                    V[0xF] = 1;
                }
                gfx[x + xline + ((y + yline) * 64)] ^= 1;
            }
        }
    }

    drawFlag = true;
    pc += 2;
}

void Emulator::keyop(){
    cout << __func__ << endl;
    if ((opcode & 0x00FF) == 0x00A1){
        if (key[V[(opcode >> 8) & 0x0F]] == 0)
            pc += 2;

        pc += 2;
    }
    else if ((opcode & 0x00FF) == 0x009E){
        if (key[V[(opcode >> 8) & 0x0F]] != 0)
            pc += 2;
        
        pc += 2;
    }
    else {
        printf("Error Keyop, unknown opcode: 0x%4x \n", opcode);
        exit(1);
    }
}

void Emulator::miscop(){
    cout << __func__ << endl;
    if ((opcode & 0x00FF) == 0x0007){
        V[(opcode >> 8) & 0x0F] = delay_timer;
    }
    else if ((opcode & 0x00FF) == 0x000A) {
        bool pressedFlag = false;
        for (int i = 0; i < 16; i++){
            if(key[i] != 0){
                V[(opcode >> 8) & 0x0F] = i;
                pressedFlag = true;
            }
        }
        if(!pressedFlag){
            // Halt
            return;
        }
    }
    else if ((opcode & 0x00FF) == 0x0015) {
        delay_timer = V[(opcode >> 8) & 0x0F];
    }
    else if ((opcode & 0x00FF) == 0x0018) {
        sound_timer = V[(opcode >> 8) & 0x0F];
    }
    else if ((opcode & 0x00FF) == 0x001E) {
        if(I + V[(opcode >> 8) & 0x0F] > 0xFFF)
            V[0xF] = 1;
        else
            V[0xF] = 0;
        I += V[(opcode >> 8) & 0x0F];
    }
    else if ((opcode & 0x00FF) == 0x0029) {
        I = V[(opcode >> 8) & 0x0F] * 0x5;
    }
    else if ((opcode & 0x00FF) == 0x0033) {
        internalMemory[I] = V[(opcode >> 8) & 0x0F]/100;
        internalMemory[I+1] = (V[(opcode >> 8) & 0x0F]/10)%10;
        internalMemory[I+2] = V[(opcode >> 8) & 0x0F]%10;
    }
    else if ((opcode & 0x00FF) == 0x0055) {
        for(int i = 0; i < 16; i++){
            internalMemory[I+i] = V[i];
        }
        // Wiki: "but I itself is left unmodified."
    }
    else if ((opcode & 0x00FF) == 0x0065) {
        for(int i = 0; i < 16; i++){
            V[i] = internalMemory[I+i];
        }
        // Wiki: "but I itself is left unmodified."
    }
    else{
        printf("Error miscop, unknown opcode: 0x%4x \n", opcode);
        exit(1);
    }

    pc += 2;
}


Emulator::Emulator() {
    
    init_states();
    init_funcs();
}

Emulator::~Emulator() {

}

void Emulator::loadGame(string filename) {
    

	ifstream file(filename, ios::in | ios::binary | ios::ate);
	if (!file.is_open()) {
		cout << "Error in reading the rom\n";
		exit(1);
	}
	streampos size = file.tellg();
	file.seekg(0, ios::beg);
    char* buffer = new char [size];
	file.read((char*)buffer, size);
    
    if ((4096-512) > size){
        for (int i = 0; i < size; ++i) {
            //printf("0x%2x \n", (uint8_t)buffer[i]);
            internalMemory[i + 512] = (uint8_t)buffer[i]; 
                                                        
        }
    }

	file.seekg(0, ios::beg);
	file.close();
}

void Emulator::emulationCycle() {
    // Fetch
    opcode = (this->internalMemory[pc]  << 8 ) | this->internalMemory[pc+1];
    // Decode
    printf("0x%4x : 0x%4x ", pc, opcode);
    // Execute
    (this->*(funcs[(opcode>> 12) & 0xF ]))();


    if (delay_timer > 0)
        delay_timer--;
    if (sound_timer > 0)
        if(sound_timer == 1);
            sound_timer--;

    
}

void Emulator::init_states(){
    pc = 0x200;
    opcode = 0;  	
    I = 0;      
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;
    memset(internalMemory, 0, 4096);
    for (int i = 0; i < sizeof(chip8_fontset); i++) {
        internalMemory[i] = chip8_fontset[i];
    }
    
    memset(gfx, 0, sizeof(gfx));
    for (int i = 0; i < 16; i++){
        stack[i] = 0;
        V[i] = 0;
        key[i] = 0;
    }
}

void Emulator::init_funcs(){
    funcs.push_back(&Emulator::call);
    funcs.push_back(&Emulator::jump);
    funcs.push_back(&Emulator::flow);
    funcs.push_back(&Emulator::rtonum_equal);
    funcs.push_back(&Emulator::rtonum_not_equal);
    funcs.push_back(&Emulator::rtor_equal);
    funcs.push_back(&Emulator::set_rtonum);
    funcs.push_back(&Emulator::add_numtor);
    funcs.push_back(&Emulator::oper);
    funcs.push_back(&Emulator::rtor_not_equal);
    funcs.push_back(&Emulator::set_itonum);
    funcs.push_back(&Emulator::flow_v0_num);
    funcs.push_back(&Emulator::set_rand);
    funcs.push_back(&Emulator::draw);
    funcs.push_back(&Emulator::keyop);
    funcs.push_back(&Emulator::miscop); 
}

uint16_t Emulator::get_pc() {
    return pc;
}