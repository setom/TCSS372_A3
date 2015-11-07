/*

	Matthew Seto
	TCSS 372
	Assignment 3
	
	Thanks to Geoffrey for helping me to unravel the masks for the opcodes
	
	Converting the controller to a new ISA
	
*/

/*Note about SW Overflow:
Overflow:
	1 - If the signs of the two operands are DIFFERENT before the op, there is NEVER OVERFLOW
	2 - If the signs are the same before the op, IF THE OUTPUT SIGN IS OPPOSITE, then there MUST HAVE BEEN OVERFLOW (therefore, set overflow)
	 
*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "TCSS372_A3.h"


int main (int argc, char* argv[]){

	
	//create a pointer to a CPU instance 
	struct CPU *myCPU = create_CPU();
	
	//load the test program into memory
	loadProgramToMemory();

	//while the state is not halt, continue looping through
	 while(state != HALT){
		switch(state){
			case FETCH :
				microstate = FETCH1;
				while(microstate < EXIT) {
					switch(microstate) {
						case FETCH1 :
							//MAR <- PC
							myCPU->MAR = myCPU->PC;
							//A <- PC
							myCPU->ALUA = myCPU->PC;
							
							//increment to the next microstate
							microstate = FETCH2;
							
							break;
						
						case FETCH2 :

							//scan the hex value that defines instruction and specific variables
							// printf("Enter hex instruction: \n");
// 							scanf("%X", &instruction);
							
							//fetch the next instruction from memory
							instruction = systemMemory[myCPU->PC];
							printf("Next instruction: %08X\n", instruction);
							
							//IR <- M[MAR] //input a number from 0-7 representing an instruction
							myCPU->IR = instruction;
							
							//increment to next microstate
							microstate = FETCH3;
							
							break;
							
						case FETCH3 :
							//printf("FETCH3\n");
							//PC<- A +1 increment the PC
							myCPU->PC = myCPU->ALUA+1;
							myCPU->ALUA = 0;
							
							//increment the microstate
							microstate = FETCH4;
							
							break;
						
						case FETCH4 :
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
					
					//shift the instruction to read only the first 5 digits
					opcode = myCPU->IR >> 27;
					
					//switch through the appropriate microstate
					switch(opcode) {
						case 0x01 :
							microstate = LDI;
							DecodeFormat1(myCPU->IR);
							break;
						case 0x02 :
							microstate = LD;
							DecodeFormat2(myCPU->IR);
							break; 
						//case 0x03 :
						//	microstate = ST;
						//	break;
						case 0x04 :
							microstate = ST;
							DecodeFormat2(myCPU->IR);
							break;
						case 0x05 :
							microstate = MOV;
							DecodeFormat3(myCPU->IR);
							break;
						case 0x06 :
							microstate = PUSH;
							DecodeFormat1(myCPU->IR);
							break;
						case 0x07 :
							microstate = POP;
							DecodeFormat1(myCPU->IR);
							break;
						case 0x08 :
							microstate = ADD;
							DecodeFormat3(myCPU->IR);
							break;
						case 0x09 :
							microstate = SUB;
							DecodeFormat3(myCPU->IR);
							break;
						case 0x0A :
							microstate = AND;
							DecodeFormat3(myCPU->IR);
							break;
						case 0x0B :
							microstate = OR;
							DecodeFormat3(myCPU->IR);
							break;		
						case 0x0C :
							microstate = NOT;
							DecodeFormat2(myCPU->IR);
							break;
						case 0x0D :
							microstate = XOR;
							DecodeFormat3(myCPU->IR);
							break;
						case 0x0E :
							microstate = SHL;
							DecodeFormat3(myCPU->IR);
							break;
						case 0x0F :
							microstate = SHR;
							DecodeFormat3(myCPU->IR);
							break;
						case 0x10 :
							microstate = BR;
							DecodeFormat4(myCPU->IR);
							break;
						case 0x11 :
							microstate = BRZ;
							DecodeFormat4(myCPU->IR);
							break;
						case 0x12 :
							microstate = BRN;
							DecodeFormat4(myCPU->IR);
							break;
						case 0x13 :
							microstate = BRC;
							DecodeFormat4(myCPU->IR);
							break;			
						case 0x14 :
							microstate = BRO;
							DecodeFormat4(myCPU->IR);
							break;
						case 0x15 :
							microstate = JSR;
							DecodeFormat1(myCPU->IR);
							break;
						case 0x16 :
							microstate = JSRZ;
							DecodeFormat1(myCPU->IR);
							break;
						case 0x17 :
							microstate = JSRN;
							DecodeFormat1(myCPU->IR);
							break;
						case 0x18 :
							microstate = DI;
							break;
						case 0x19 :
							microstate = EI;
							break;
						case 0x1A :
							microstate = RET;
							break;
						case 0x1B :
							microstate = RETI;
							break;			
						case 0x1C :
							microstate = TRAP;
							break;
						case 0x1D :
							microstate = HALT;
							break;
						case 0x00 :
							microstate = NOP;
							break;	
					}
					
					state = EXECUTE;
							
				
			case EXECUTE :
				while(microstate < EXIT){
					
					//switch statement to handle the appropriate microstate
					 switch(microstate) {
						case LDI :
							printf("In the LDI microstate\n");
							myCPU->registers[RD] = immediate;
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;	
						case LD :
							printf("In the LD microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case ST :
							printf("In the ST microstate\n");
							myCPU->MDR = myCPU->registers[RS1];
							myCPU->MAR = myCPU->registers[RD] + immediate;
							systemMemory[myCPU->MAR] = myCPU->MDR;
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case MOV :
							printf("In the MOV microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case PUSH :
							printf("In the PUSH microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case POP :
							printf("In the POP microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case ADD :
							printf("In the ADD microstate\n");
							//reset the flags
							myCPU->SW = 0x00000000;
							myCPU->ALUA = systemMemory[myCPU->registers[RS1]];
							myCPU->ALUB = systemMemory[myCPU->registers[RS2]];
							//check the signs (most significant bit, 2s compliment)
							//if the signs are 1s
							if((myCPU->ALUA & 0x80000000) && (myCPU->ALUB & 0x80000000)){
								//do the add
								myCPU->ALUResult = myCPU->ALUA + myCPU->ALUB;
								//if the result is not negative, there must have been overflow
								if(myCPU->ALUResult & 0x00000000){
									//set the carry flag
									myCPU->SW = 0x20000000;
								}
							} 
							//do the same for negatives
							else if ((myCPU->ALUA | 0x00000000) && (myCPU->ALUB | 0x00000000)){
								//do the add
								myCPU->ALUResult = myCPU->ALUA + myCPU->ALUB;
								//if the result is not negative, there must have been overflow
								if(myCPU->ALUResult & 0x00000000){
									//set the carry flag
									myCPU->SW = 0x20000000;
								}
							}
							//else the signs must have been different, therefore never overflow
							else {
								myCPU->ALUResult = myCPU->ALUA + myCPU->ALUB;
								myCPU->SW = 0x00000000;
							}
							//set the Z flag if appropriate
							if(myCPU->ALUResult == 0x00000000){
								myCPU->SW += 0x80000000;
							}
							//set the N flag if appropriate
							if(myCPU->ALUResult & 0x80000000){
								myCPU->SW += 0x40000000;
							}
							//set the output register on the way out
							myCPU->registers[RD] = myCPU->ALUResult;
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case SUB :
							printf("In the SUB microstate\n");
							//reset the flags
							myCPU->SW = 0x00000000;
							myCPU->ALUA = systemMemory[myCPU->registers[RS1]];
							myCPU->ALUB = systemMemory[myCPU->registers[RS2]];
							//check the signs (most significant bit, 2s compliment)
							//if the signs are 1s
							if((myCPU->ALUA & 0x80000000) && (myCPU->ALUB & 0x80000000)){
								//do the sub
								myCPU->ALUResult = myCPU->ALUB - myCPU->ALUA;
								//if the result is not negative, there must have been overflow
								if(myCPU->ALUResult & 0x00000000){
									//set the overflow flag
									myCPU->SW = 0x10000000;
								}
							} 
							//do the same for negatives
							else if ((myCPU->ALUA | 0x00000000) && (myCPU->ALUB | 0x00000000)){
								//do the add
								myCPU->ALUResult = myCPU->ALUB - myCPU->ALUA;
								//if the result is not negative, there must have been overflow
								if(myCPU->ALUResult & 0x00000000){
									//set the overflow flag
									myCPU->SW = 0x10000000;
								}
							}
							//else the signs must have been different, therefore never overflow
							else {
								myCPU->ALUResult = myCPU->ALUA - myCPU->ALUB;
								myCPU->SW = 0x00000000;
							}
							//set the Z flag if appropriate
							if(myCPU->ALUResult == 0x00000000){
								myCPU->SW += 0x80000000;
							}
							//set the N flag if appropriate
							if(myCPU->ALUResult & 0x80000000){
								myCPU->SW += 0x40000000;
							}
							//set the output register on the way out
							myCPU->registers[RD] = myCPU->ALUResult;
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case AND :
							printf("In the AND microstate\n");
							myCPU->ALUA = systemMemory[myCPU->registers[RS1]];
							myCPU->ALUB = systemMemory[myCPU->registers[RS2]];
							myCPU->ALUResult = myCPU->ALUA & myCPU->ALUB;
							myCPU->registers[RD] = myCPU->ALUResult;
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case OR :
							printf("In the OR microstate\n");
							myCPU->ALUA = systemMemory[myCPU->registers[RS1]];
							myCPU->ALUB = systemMemory[myCPU->registers[RS2]];
							myCPU->ALUResult = myCPU->ALUA | myCPU->ALUB;
							myCPU->registers[RD] = myCPU->ALUResult;
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case NOT :
							printf("In the NOT microstate\n");
							myCPU->ALUA = myCPU->registers[RS1];
							myCPU->ALUResult = ~myCPU->ALUA;
							myCPU->registers[RD] = myCPU->ALUResult;
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case XOR :
							printf("In the XOR microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case SHL :
							printf("In the SHL microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case SHR :
							printf("In the SHR microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case BR :
							printf("In the BR microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case BRZ :
							printf("In the BRZ microstate\n");
							if(myCPU->SW == 0x80000000){
								myCPU->PC = myCPU->PC + immediate;
							} 
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case BRN :
							printf("In the BRN microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case BRC :
							printf("In the BRC microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case BRO :
							printf("In the BRO microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case JSR :
							printf("In the JSR microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case JSRZ :
							printf("In the JSRZ microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case JSRN :
							printf("In the JSRN microstate\n");
							state = FETCH;	
							instruction = 0;
							microstate = EXIT;
							break;
						case HALT :
							printf("In the HALT microstate\n");
							state = HALT;	
							instruction = 0;
							microstate = EXIT;
							CPU_Destroy(myCPU);
							exit(0);
					}
					
					printDebugMonitor(myCPU);
					state = FETCH;
					microstate = EXIT;
				}		
		}		
	}
	printf("\n");
	CPU_Destroy(myCPU);
	return (0);
	}
	
	
	//*** FUNCTIONS
	
	//function to create a CPU struct
	struct CPU *create_CPU(){

		struct CPU *theCPU = malloc(sizeof(struct CPU));
		theCPU->PC = 1;
		theCPU->MAR = 0;
		theCPU->MDR = 0;
		theCPU->IR = 0;
		theCPU->ALUA = 0;
		theCPU->ALUB = 0;
		theCPU->ALUResult = 0;
		theCPU->registers[0] = 0x00000000;
		if (theCPU != NULL){
			return theCPU;
		} else {
			printf("Error Creating CPU Object!\n");
			exit(1);
		}
	}
	
	//function to destroy a CPU struct
	void CPU_Destroy(struct CPU *theCPU){
		free(theCPU);
	}	
	
	//function to decode format 1 instructions
	void DecodeFormat1(unsigned int instr){
		//*** FORMAT 1 ***
		//to get RD, mask with 0x07800000
		RD = ((instr & 0x07800000) >> 23);
		//mask the opcode and register, leave only the immediate
		// immediate = (instr & 0x007FFFFF);
		immediate = signExtend24(instr);
	}
	
	//function to decode format 2 instructions
	void DecodeFormat2(unsigned int instr){
		//*** FORMAT 2 ***
		//to get RD, mask with 0x07800000
		RS1 = ((instr & 0x07800000) >> 23);
		//to get RS1, mask with 0x00780000
		RD = ((instr & 0x00780000) >> 19);
		//mask the opcode, register and base register, leave the imm.
		//immediate = (instruction & 0x0007FFFF);
		immediate = signExtend19(instr);
	}
	
	//function to decode format 3 instructions
	void DecodeFormat3(unsigned int instr){
		//****FORMAT 3
		//to get RD, mask with 0x07800000
		RD = ((instr & 0x07800000) >> 23);
		//to get RS1, mask with 0x00780000
		RS1 = ((instr & 0x00780000) >> 19);
		//to get RS2, mask with 0x00078000
		RS2 = ((instr & 0x00078000) >> 15);
	}
	
	//function to decode format 4 instructions
	void DecodeFormat4(unsigned int instr){
		// *** FORMAT 4 ***
		//mask to preserve bits 24-0
		//immediate = (instruction & 0x07FFFFFF);
		immediate = signExtend27(instr);
	}
	
	//function to sign extend the immediate 27 bit
	int signExtend27(unsigned int instr){
		if (instr & 0x04000000){
			instr |= 0xF8000000;
		} else {
			instr = instr & 0x07FFFFFF;
		}
		return instr;
	}
	
	//function to sign extend the immediate 24 bit
	int signExtend24(unsigned int instr){
		if(instr & 0x00400000){
			instr |= 0xff800000;
		} else {
			instr = instr & 0x007FFFFF; 
		}
		return instr;
	}
	
	//function to sign extend the immediate 19 bit
	int signExtend19(unsigned int instr){
		if (instr & 0x00040000){
			instr |= 0xFFF80000;
		} else {
			instr = instr & 0x0007FFFF;
		}
		return instr;
	}
	

	
	//function to display the debug monitor
	void printDebugMonitor(struct CPU *theCPU){
		int j, k;
		printf("\tSC-4 Debug Monitor\n\n");
		printf("Instruction registers: RD: %d, RS1: %d, RS2: %d\n", RD, RS1, RS2);
		printf("Immediate: %d\n", immediate);
		printf("Register File:\t\t\t\t\t Memory Dump:\n");
		k = theCPU->PC-5;
		for(j=0; j<17; j++){
				printf("%d:\t%08X\t\t\t\t ", j, theCPU->registers[j]);
				if (k < 0) {
					printf("        --------:\t--------");
				} else if (k == theCPU->PC-1){
					printf("PC----> %08d:\t%08X", k, systemMemory[k]);
				} else {
					printf("        %08d:\t%08X", k, systemMemory[k]);
				}
				printf("\n");
				k++;
		}
		printf("\n");
		printf("PC: %08X\t IR: %08X\t SW: %08X\t\n", theCPU->PC, theCPU->IR, theCPU->SW);
		printf("MAR: %08X\t MDR: %08X\t ALU.A: %08X\t ALU.B: %08X\t ALU.R: %08X", theCPU->MAR, theCPU->MDR, theCPU->ALUA, theCPU->ALUB, theCPU->ALUResult);
		
		printf("\n\n");
		printf("Press Any Key to Continue: \n");
		
		//getch();
		//getch() is not available on OSX, this workaround was found on stack overflow
		// http://stackoverflow.com/questions/267250/equivalent-to-windows-getch-for-mac-linux-crashes
		system("read -n1 -p ' ' key"); 
		printf("\n");
	
	}
	
	//prototype for loading the program hex values into memory
	void loadProgramToMemory(){
		
		//Memory Diagram
		////////////////
		//    LOW     //
		//            //
		//  my code   //
		//            //
		//------------//
		// literals   // 
		//------------//
		//            //
		//            //
		//    heap    //
		//            //
		//------------//
		//            // 
		//            //
		//   stack    //
		//            //
		//    HIGH    // 
		////////////////
		
		//hard code the program
		
		//LDI R1, Data1
		systemMemory[1] = 0x0880014;
		//LDI R2, Data 2
		systemMemory[2] = 0x09000015;
		//LDI R5, @result
		systemMemory[3] = 0x0A800016;
		//ADD R3, R1, R2
		systemMemory[4] = 0x41890000;
		//ST R3, R5 #0
		systemMemory[5] = 0x21A80000;
		//OR R3, R1, R2
		systemMemory[6] = 0x59890000;
		//ST R3, R5, #1
		systemMemory[7] = 0x21A80001;
		//AND R3, R1, R2
		systemMemory[8] = 0x53120000;
		//ST R3, R5, #2
		systemMemory[9] = 0x21A80002;
		//LDI R1, #1
		systemMemory[10] = 0x08800001;
		//LDI R2, #1
		systemMemory[11] = 0x09000001;
		//SUB R3,R1,R2
		systemMemory[12] = 0x49890000;
		//BRZ Finish (Branch to Finish if zero)
		systemMemory[13] = 0x8800000C;
		//LDI R2, #10
		systemMemory[14] = 0x09000010;
		//ST R2, R5, #3
		systemMemory[15] = 0x21298000;
		//BR Done
		systemMemory[16] = 0x8000000C;
		
		
		//Literals (Data locations)
		//DATA1
		systemMemory[20] = 0x00000064;
		//DATA2
		systemMemory[21] = 0x0000000A;
		//RESUlT
		systemMemory[22] = 0x33333333;
		//Three other results accessed by offsets
		systemMemory[23] = 0x44444444;
		systemMemory[24] = 0x44444444;
		systemMemory[25] = 0x44444444;
		//FINISH
		//LDI R2, 0x10
		systemMemory[26] = 0x09000020;
		//ST R2, R5, #3
		systemMemory[27] = 0x21280003;
		//DONE (HALT)
		systemMemory[28] = 0xE8000000;
		
		
	}