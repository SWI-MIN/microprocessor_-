//
// GPIO_7seg_keypad : 3x3 keypad inpt and display on 7-segment LEDs
//
#include <stdio.h>
#include <stdlib.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Seven_Segment.h"
#include "Scankey.h"

uint8_t SEG[10] = {0x82, 0xEE, 0x07, 0x46, 0x6A, 0x52, 0x12, 0xE6, 0x02, 0x62}; 
int pc_Open = 1;
int pc_Close = 0;
int pe_Open = 0;
int pe_Close = 1;
/*
PC = 1 is OPEN,PE = 0 is OPEN
void Test_7_segment(uint16_t value){
	PC4 = 0; // right to left 0
	PC5 = 0; // 1
	PC6 = 0; // 2
	PC7 = 0; // 3
	
	PE0 = 1; // bottom right
	PE1 = 1; // dot
	PE2 = 1; // upper left
	PE3 = 1; // upper 
	PE4 = 1; // upper right
	PE5 = 1; // bottom 
	PE6 = 1; // bottom left
	PE7 = 1; // center
	CLK_SysTickDelay(50000);
}
*/

// display an integer on four 7-segment LEDs
void Q1_Show(uint16_t value){
	PC4=1;
	uint8_t temp;
	temp=SEG[value];
	for(int i=0;i<8;i++){
		if((temp&0x01)==0x01){	   		   
			switch(i){
			case 0: PE0=1; break;
			case 1: PE1=1; break;
			case 2: PE2=1; break;
			case 3: PE3=1; break;
			case 4: PE4=1; break;
			case 5: PE5=1; break;
			case 6: PE6=1; break;
			case 7: PE7=1; break;
			}
		}else{
			switch(i) {
			case 0: PE0=0; break;
			case 1: PE1=0; break;
			case 2: PE2=0; break;
			case 3: PE3=0; break;
			case 4: PE4=0; break;
			case 5: PE5=0; break;
			case 6: PE6=0; break;
			case 7: PE7=0; break;
			}	  
		}
		temp=temp>>1;
	}
}

int Q2_find_pc(int *pc){
	for(int i=0; i<4; i++){
		if(pc[i] == 1){
			return i;
		}
	}
}
int Q2_find_pe(int *pe){
	for(int i=0; i<8; i++){
		if(pe[i] == 0){
			return i;
		}
	}
}

void Q2_Show(int pcc, int pee){
	switch(pcc){
		case 0: PC4=1; break;
		case 1: PC5=1; break;
		case 2: PC6=1; break;
		case 3: PC7=1; break;
	}
	switch(pee){
		 case 0: PE0=0; break;
		 case 1: PE1=0; break;
		 case 2: PE2=0; break;
		 case 3: PE3=0; break;
		 case 4: PE4=0; break;
		 case 5: PE5=0; break;
		 case 6: PE6=0; break;
		 case 7: PE7=0; break;
	}
}

void Q2_Close(void){
	PC4 = 0; // right to left 0
	PC5 = 0; // 1
	PC6 = 0; // 2
	PC7 = 0; // 3
	
	PE0 = 1; // bottom right
	PE1 = 1; // dot
	PE2 = 1; // upper left
	PE3 = 1; // upper 
	PE4 = 1; // upper right
	PE5 = 1; // bottom 
	PE6 = 1; // bottom left
	PE7 = 1; // center
	CLK_SysTickDelay(5000);
}


void Q2_Change_PC_PE(int *pc, int *pe, uint16_t value){
	int pcc = Q2_find_pc(pc);
	int pee = Q2_find_pe(pe);
	if(value==2){
		if(pee == 4){
			pe[4]=pe_Close;pe[3]=pe_Open;
		}else if(pee == 0){
			pe[0]=pe_Close;pe[4]=pe_Open;
		}else if(pee == 3){
			pe[3]=pe_Close;pe[5]=pe_Open;
		}else if(pee == 7){
			pe[7]=pe_Close;pe[3]=pe_Open;
		}else if(pee == 5){
			pe[5]=pe_Close;pe[7]=pe_Open;
		}else if(pee == 2){
			pe[2]=pe_Close;pe[3]=pe_Open;
		}else if(pee == 6){
			pe[6]=pe_Close;pe[2]=pe_Open;
		}
	}
	else if(value==8){
		if(pee == 4){
			pe[4]=pe_Close;pe[0]=pe_Open;
		}else if(pee == 0){
			pe[0]=pe_Close;pe[5]=pe_Open;
		}else if(pee == 3){
			pe[3]=pe_Close;pe[7]=pe_Open;
		}else if(pee == 7){
			pe[7]=pe_Close;pe[5]=pe_Open;
		}else if(pee == 5){
			pe[5]=pe_Close;pe[3]=pe_Open;
		}else if(pee == 2){
			pe[2]=pe_Close;pe[6]=pe_Open;
		}else if(pee == 6){
			pe[6]=pe_Close;pe[5]=pe_Open;
		}
	}
	else if(value==6){
		if(pee == 4){
			if(pcc == 0){
				pc[0]=pc_Close;pc[3]=pc_Open;
				pe[4]=pc_Close;pe[2]=pe_Open;
			}else if(pcc == 1){
				pc[1]=pc_Close;pc[0]=pc_Open;
				pe[4]=pe_Close;pe[2]=pe_Open;
			}else if(pcc == 2){
				pc[2]=pc_Close;pc[1]=pc_Open;
				pe[4]=pe_Close;pe[2]=pe_Open;
			}else if(pcc == 3){
				pc[3]=pc_Close;pc[2]=pc_Open;
				pe[4]=pe_Close;pe[2]=pe_Open;
			}
		}else if(pee == 0){
			if(pcc == 0){
				pc[0]=pc_Close;pc[3]=pc_Open;
				pe[0]=pe_Close;pe[6]=pe_Open;
			}else if(pcc == 1){
				pc[1]=pc_Close;pc[0]=pc_Open;
				pe[0]=pe_Close;pe[6]=pe_Open;
			}else if(pcc == 2){
				pc[2]=pc_Close;pc[1]=pc_Open;
				pe[0]=pe_Close;pe[6]=pe_Open;
			}else if(pcc == 3){
				pc[3]=pc_Close;pc[2]=pc_Open;
				pe[0]=pe_Close;pe[6]=pe_Open;
			}
		}else if(pee == 3){
			pe[3]=pe_Close;pe[4]=pe_Open;
		}else if(pee == 7){
			if(pcc == 0){
				pc[0]=pc_Close;pc[3]=pc_Open;
			}else if(pcc == 1){
				pc[1]=pc_Close;pc[0]=pc_Open;
			}else if(pcc == 2){
				pc[2]=pc_Close;pc[1]=pc_Open;
			}else if(pcc == 3){
				pc[3]=pc_Close;pc[2]=pc_Open;
			}
		}else if(pee == 5){
			pe[5]=pe_Close;pe[0]=pe_Open;
		}else if(pee == 2){
			pe[2]=pe_Close;pe[3]=pe_Open;
		}else if(pee == 6){
			pe[6]=pe_Close;pe[5]=pe_Open;
		}
	}
	else if(value==4){
		if(pee == 4){
			pe[4]=pe_Close;pe[3]=pe_Open;
		}else if(pee == 0){
			pe[0]=pe_Close;pe[5]=pe_Open;
		}else if(pee == 3){
			pe[3]=pe_Close;pe[2]=pe_Open;
		}else if(pee == 7){
			if(pcc == 0){
				pc[0]=pc_Close;pc[1]=pc_Open;
			}else if(pcc == 1){
				pc[1]=pc_Close;pc[2]=pc_Open;
			}else if(pcc == 2){
				pc[2]=pc_Close;pc[3]=pc_Open;
			}else if(pcc == 3){
				pc[3]=pc_Close;pc[0]=pc_Open;
			}
		}else if(pee == 5){
			pe[5]=pe_Close;pe[6]=pe_Open;
		}else if(pee == 2){
			if(pcc == 0){
				pc[0]=pc_Close;pc[1]=pc_Open;
				pe[2]=pe_Close;pe[4]=pe_Open;
			}else if(pcc == 1){
				pc[1]=pc_Close;pc[2]=pc_Open;
				pe[2]=pe_Close;pe[4]=pe_Open;
			}else if(pcc == 2){
				pc[2]=pc_Close;pc[3]=pc_Open;
				pe[2]=pe_Close;pe[4]=pe_Open;
			}else if(pcc == 3){
				pc[3]=pc_Close;pc[0]=pc_Open;
				pe[2]=pe_Close;pe[4]=pe_Open;
			}
		}else if(pee == 6){
			if(pcc == 0){
				pc[0]=pc_Close;pc[1]=pc_Open;
				pe[6]=pe_Close;pe[0]=pe_Open;
			}else if(pcc == 1){
				pc[1]=pc_Close;pc[2]=pc_Open;
				pe[6]=pe_Close;pe[0]=pe_Open;
			}else if(pcc == 2){
				pc[2]=pc_Close;pc[3]=pc_Open;
				pe[6]=pe_Close;pe[0]=pe_Open;
			}else if(pcc == 3){
				pc[3]=pc_Close;pc[0]=pc_Open;
				pe[6]=pe_Close;pe[0]=pe_Open;
			}
		}
	}
}


int main(void){
	SYS_Init();
	OpenSevenSegment();
	CloseSevenSegment();
	OpenKeyPad();
	
	uint16_t i;
	int question = 2;
	
	if(question == 1){
		while(1){
			i = ScanKey();
			Q1_Show(i);
		}
	}
	else if(question == 2){
		int pc[4] = {1, 0, 0, 0};
		int pe[8] = {1, 1, 1, 1, 0, 1, 1, 1};
		while(1){
			
			i = ScanKey();
			if(i==2 || i == 4 || i == 6 || i == 8){
				Q2_Close();
				Q2_Change_PC_PE(pc, pe, i);
				CLK_SysTickDelay(1000000);
			}
			int pcc = Q2_find_pc(pc);
			int pee = Q2_find_pe(pe);
			Q2_Show(pcc, pee);
		}
	}
}
