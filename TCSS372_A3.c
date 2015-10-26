/*

	Matthew Seto
	TCSS 372
	Assignment 3
	
	Converting the controller to a new ISA
	
*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
//#include <curses.h>

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
#define ADD 0
#define NAND 1 
#define ADDI 2
#define LW 3
#define SW 4
#define BEQ 5
#define JALR 6
#define HALT 7

//prototype for getting user to press a key to continue
int pressAnyKey();


int main (int argc, char* argv[]){

	//int variables for registers
	int PC = 0;
	int IR = 0;
	int MAR = 0;
	int A = 0;
	int B = 0;
	int RX = 0;
	int RY = 0;
	int RZ = 0;
	int immediate = 0;
	int state = FETCH;
	int microstate = FETCH1;
	
	int instruction = 0;
	int opcode = 0;

	//while the state is not halt, continue looping through
	 while(state != HALT){
		switch(state){
			case FETCH :
				microstate = FETCH1;
				while(microstate < EXIT) {
					switch(microstate) {
						case FETCH1 :
							pressAnyKey();
							//printf("FETCH1\n");
							//MAR <- PC
							MAR = PC;
							//A <- PC
							A = PC;
							
							//increment to the next microstate
							microstate = FETCH2;
							
							break;
						
						case FETCH2 :
							pressAnyKey();
							//printf("FETCH2\n");
							//load IR with instruction
							//scan the hex value that defines instruction and specific variables
							printf("Enter hex instruction: \n");
							scanf("%x", &instruction);
							//printf("%x\n", instruction);
							
							//IR <- M[MAR] //input a number from 0-7 representing an instruction
							IR = instruction;
							
							//increment to next microstate
							microstate = FETCH3;
							
							break;
							
						case FETCH3 :
							pressAnyKey();
							//printf("FETCH3\n");
							//PC<- A +1 increment the PC
							PC = A+1;
							
							//increment the microstate
							microstate = FETCH4;
							
							break;
						
						case FETCH4 :
							pressAnyKey();
							//printf("FETCH4\n");
							//increment the state
							state = DECODE;
							
							//change the microstate to EXIT STATE
							microstate = EXIT;
					}
					
				}
				//printf("EXITED FETCH\n");		
			case DECODE :
				//reset the microstate
				microstate = DECODE1;
				//while(microstate < EXIT) {
					//printf("ENTERED DECODE\n");
					
					
					//switch statement for j-type/i-type etc...instructions
					//decode what type of instruction you are in
					//mask the instruction hex (8 hex digits) with F000000 (7 hex digits)
					//to get only the first digit
					opcode = instruction & 0xF0000000;
					//shift to the least significant space
					opcode = opcode >> 28;
					printf("Opcode: %x\n", opcode);
					
					//switch through the appropriate microstate
					switch(opcode) {
						case 0x0 :
							microstate = ADD;
							RX = ((instruction & 0x0F000000) >> 24);
							RY = ((instruction & 0x00F00000) >> 20);
							RZ = instruction & 0x0000000F;
							break;
						case 0x1 :
							microstate = NAND;
							RX = ((instruction & 0x0F000000) >> 24);
							RY = ((instruction & 0x00F00000) >> 20);
							RZ = instruction & 0x0000000F;
							break;
						case 0x2 :
							microstate = ADDI;
							RX = ((instruction & 0x0F000000) >> 24);
							RY = ((instruction & 0x00F00000) >> 20);
							immediate = 0x000FFFFF;
							break;
						case 0x3 :
							microstate = LW;
							RX = ((instruction & 0x0F000000) >> 24);
							RY = ((instruction & 0x00F00000) >> 20);
							immediate = 0x000FFFFF;
							break;
						case 0x4 :
							microstate = SW;
							RX = ((instruction & 0x0F000000) >> 24);
							RY = ((instruction & 0x00F00000) >> 20);
							immediate = 0x000FFFFF;
							break;
						case 0x5 :
							microstate = BEQ;
							RX = ((instruction & 0x0F000000) >> 24);
							RY = ((instruction & 0x00F00000) >> 20);
							immediate = 0x000FFFFF;
							break;
						case 0x6 :
							microstate = JALR;
							RX = ((instruction & 0x0F000000) >> 24);
							RY = ((instruction & 0x00F00000) >> 20);
							break;
						case 0x7 :
							microstate = HALT;
							break;							
					}
					//set the appropriate register values (taken from the input)
					
					//increment the state and microstate
					state = EXECUTE;
					//microstate = EXIT;
				//}
				//printf("EXITED DECODE\n");		
				
			case EXECUTE :
				while(microstate < EXIT){
					//printf("ENTERED EXECUTE\n");
					
					//switch statement to handle the appropriate microstate
					switch(microstate) {
						case ADD :
							printf("In the ADD microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case NAND :
							printf("In the NAND microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case ADDI :
							printf("In the ADDI microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case LW :
							printf("In the LW microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case SW :
							printf("In the SW microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case BEQ :
							printf("In the BEQ microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case JALR :
							printf("In the JALR microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case HALT :
							printf("In the HALT microstate\n");
							state = HALT;
							microstate = EXIT;
							break;
					}

					printf("PC: %d,\t IR: %x,\t MAR: %d,\t A: %d,\t B: %d,\t RX: %d,\t RY: %d,\t RZ: %d,\n", PC, IR, MAR, A, B, RX, RY, RZ);	
				}		
		}		
	}
	printf("\n");
	return (0);
	}
	
	
	//function to get user to press a key before continuing
	int pressAnyKey() {
		printf("Press Any Key to Continue: \n");
		
		//getch();
		//getch() is not available on OSX, this workaround was found on stack overflow
		// http://stackoverflow.com/questions/267250/equivalent-to-windows-getch-for-mac-linux-crashes
		system("read -n1 -p ' ' key"); 
		
		return 0;
	}