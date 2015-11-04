//constants for the FSM
#define FETCH 0
#define DECODE 1
#define EXECUTE 2

//general exit Microstate
#define EXIT INT_MAX //large number higher than all microstates


//FETCH microstates
#define FETCH1 0
#define FETCH2 1
#define FETCH3 2
#define FETCH4 3

//DECODE microstate
#define DECODE1 0

//EXECUTE microstates
#define LDI 0x1
#define LD 0x2 
//#define LD-2 3
#define ST 0x04
#define MOV 0x05
#define PUSH 0x06
#define POP 0x07
#define ADD 0x08
#define SUB 0x09
#define AND 0x0A
#define OR 0x0B
#define NOT 0x0C
#define XOR 0x0D
#define SHL 0x0E
#define SHR 0x0F
#define BR 0x10
#define BRZ 0x11
#define BRN 0x12
#define BRC 0x13
#define BRO 0x14
#define JSR 0x15
#define JSRZ 0x16
#define JSRN 0x17
#define DI 0x18
#define EI 0x19
#define RET 0x1A
#define RETI 0x1B
#define TRAP 0x1C
#define HALT 0x1D
#define NOP 0x00

//struct for a CPU
typedef struct CPU{
	//PC
	unsigned int PC;
	//MAR
	unsigned int MAR;
	//MDR 
	unsigned int MDR;
	//IR
	unsigned int IR;
	//SW, bits 32...28 Condition Flags Z, N, C, O
	unsigned int SW;
	//ALU Registers
	unsigned int ALUA;
	unsigned int ALUB;
	unsigned int ALUResult;
	//array of registers 0-15
	unsigned int registers[16];
} CPU;

//int variables for registers
unsigned int RD = 0; 	//destination register
unsigned int RS1 = 0; 	//source register 1
unsigned int RS2 = 0; 	//source register 2
unsigned int RB = 0; 	//base register
unsigned int immediate = 0;
unsigned int SP = 0; 	//stack pointer

unsigned int instruction = 0;
unsigned int opcode = 0;

	
//initialize state
int state = FETCH;
int microstate = FETCH1;
	
//array of system memory
unsigned int systemMemory[100];


//***Function prototypes

//prototype to create a CPU
struct CPU *create_CPU();

//prototype to destroy the CPU
void CPU_Destroy(struct CPU *theCPU);

//prototype methods for decoding instructions format 1-4
void DecodeFormat1(unsigned int instr);
void DecodeFormat2(unsigned int instr);
void DecodeFormat3(unsigned int instr);
void DecodeFormat4(unsigned int instr);

//prototype for printing the debug monitor
void printDebugMonitor(struct CPU *theCPU);

//prototype for loading the program hex values into memory
void loadProgramToMemory();
