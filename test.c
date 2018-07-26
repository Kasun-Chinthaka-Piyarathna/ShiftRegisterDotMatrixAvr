#define F_CPU 10000000UL
#include <avr/io.h>
#include <util/delay.h>

#define HC595_PORT_ROW   PORTB
#define HC595_DDR_ROW    DDRB
#define HC595_DS_POS_ROW PB0         //Data pin (DS) pin location
#define HC595_SH_CP_POS_ROW PB1      //Shift Clock (SH_CP) pin location
#define HC595_ST_CP_POS_ROW PB2      //Store Clock (ST_CP) pin location

#define HC595_PORT_COL   PORTD
#define HC595_DDR_COL    DDRD
#define HC595_DS_POS_COL PD0         //Data pin (DS) pin location
#define HC595_SH_CP_POS_COL PD1      //Shift Clock (SH_CP) pin location
#define HC595_ST_CP_POS_COL PD2      //Store Clock (ST_CP) pin location



// change data (DS)lines
#define HC595DataHigh_ROW() (HC595_PORT_ROW|=(1<<HC595_DS_POS_ROW))
#define HC595DataLow_ROW() (HC595_PORT_ROW&=(~(1<<HC595_DS_POS_ROW)))

#define HC595DataHigh_COL() (HC595_PORT_COL|=(1<<HC595_DS_POS_COL))
#define HC595DataLow_COL() (HC595_PORT_COL&=(~(1<<HC595_DS_POS_COL)))


void shiftInit(){
	//Make the Data(DS), Shift clock (SH_CP), Store Clock (ST_CP) lines output
	HC595_DDR_ROW|=((1<<HC595_SH_CP_POS_ROW)|(1<<HC595_ST_CP_POS_ROW)|(1<<HC595_DS_POS_ROW));
	HC595_DDR_COL|=((1<<HC595_SH_CP_POS_COL)|(1<<HC595_ST_CP_POS_COL)|(1<<HC595_DS_POS_COL));
}

void shiftPulseRow(){
	//Pulse the Shift Clock
	HC595_PORT_ROW|=(1<<HC595_SH_CP_POS_ROW);              //HIGH
	HC595_PORT_ROW&=(~(1<<HC595_SH_CP_POS_ROW));            //LOW
}

void shiftPulseCol(){
	//Pulse the Shift Clock
	HC595_PORT_COL|=(1<<HC595_SH_CP_POS_COL);                //HIGH
	HC595_PORT_COL&=(~(1<<HC595_SH_CP_POS_COL));            //LOW
}

void shiftLatchRow(){
	//Pulse the Store Clock
	HC595_PORT_ROW|=(1<<HC595_ST_CP_POS_ROW);//HIGH
	//_delay_ms(1);
	HC595_PORT_ROW&=(~(1<<HC595_ST_CP_POS_ROW));//LOW
	//_delay_ms(1);
}

void shiftLatchCol(){
	//Pulse the Store Clock
	HC595_PORT_COL|=(1<<HC595_ST_CP_POS_COL);//HIGH
	//_delay_ms(1);
	HC595_PORT_COL&=(~(1<<HC595_ST_CP_POS_COL));//LOW
	//_delay_ms(1);
}

void shiftWriteRow(uint8_t data){
	//Send each 8 bits serially
	//Order is MSB first
	for(uint8_t i=0;i<8;i++){
		//Output the data on DS line according to the
		//Value of MSB
		if(data & 0b10000000){
			//MSB is 1 so output high
			HC595DataHigh_ROW();
		}
		else{
			//MSB is 0 so output high
			HC595DataLow_ROW();
		}
		shiftPulseRow();  //Pulse the Clock line
		data=data<<1;  //Now bring next bit at MSB position
	}
	//Now all 8 bits have been transferred to shift register
	//Move them to output latch at one
	shiftLatchRow();
}

void shiftWriteCol(uint8_t data){
	//Send each 8 bits serially
	//Order is MSB first
	for(uint8_t i=0;i<8;i++){
		//Output the data on DS line according to the
		//Value of MSB
		if(data & 0b10000000){
			//MSB is 1 so output high
			HC595DataHigh_COL();
		}
		else{
			//MSB is 0 so output high
			HC595DataLow_COL();
		}
		shiftPulseCol();  //Pulse the Clock line
		data=data<<1;  //Now bring next bit at MSB position
	}
	//Now all 8 bits have been transferred to shift register
	//Move them to output latch at one
	shiftLatchCol();
}


int main(){
	uint8_t led[8]={
		0b00000000,
		0b11111101,
		0b11111011,
		0b11110111,
		0b11101111,
		0b11011111,
		0b10111111,
		0b00000000
	};
	
    static int ALPHA[26][8]={{0,0b01111111,0b11111111,0b11001100,0b11001100,0b11001100,0b11111111,0b01111111},

    {0,0b00111100,0b01111110,0b11011011,0b11011011,0b11011011,0b11111111,0b11111111},

    {0,0b11000011,0b11000011,0b11000011,0b11000011,0b11100111,0b01111110,0b00111100},

    {0,0b01111110,0b10111101,0b11000011,0b11000011,0b11000011,0b11111111,0b11111111},

    {0,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11111111,0b11111111},

    {0,0b11011000,0b11011000,0b11011000,0b11011000,0b11011000,0b11111111,0b11111111},

    {0b00011111,0b11011111,0b11011000,0b11011011,0b11011011,0b11011011,0b11111111,0b11111111},

    {0,0b11111111,0b11111111,0b00011000,0b00011000,0b00011000,0b11111111,0b11111111},

    {0b11000011,0b11000011,0b11000011,0b11111111,0b11111111,0b11000011,0b11000011,0b11000011},

    {0b11000000,0b11000000,0b11000000,0b11111111,0b11111111,0b11000011,0b11001111,0b11001111},

    {0,0b11000011,0b11100111,0b01111110,0b00111100,0b00011000,0b11111111,0b11111111},

    {0b00000011,0b00000011,0b00000011,0b00000011,0b00000011,0b00000011,0b11111111,0b11111111},

    {0b11111111,0b11111111,0b01100000,0b01110000,0b01110000,0b01100000,0b11111111,0b11111111},

    {0b11111111,0b11111111,0b00011100,0b00111000,0b01110000,0b11100000,0b11111111,0b11111111},

    {0b01111110,0b11111111,0b11000011,0b11000011,0b11000011,0b11000011,0b11111111,0b01111110},

    {0,0b01110000,0b11111000,0b11001100,0b11001100,0b11001100,0b11111111,0b11111111},

    {0b01111110,0b11111111,0b11001111,0b11011111,0b11011011,0b11000011,0b11111111,0b01111110},

    {0b01111001,0b11111011,0b11011111,0b11011110,0b11011100,0b11011000,0b11111111,0b11111111},

    {0b11001110,0b11011111,0b11011011,0b11011011,0b11011011,0b11011011,0b11111011,0b01110011},

    {0b11000000,0b11000000,0b11000000,0b11111111,0b11111111,0b11000000,0b11000000,0b11000000},

    {0b11111110,0b11111111,0b00000011,0b00000011,0b00000011,0b00000011,0b11111111,0b11111110},

    {0b11100000,0b11111100,0b00011110,0b00000011,0b00000011,0b00011110,0b11111100,0b11100000},

    {0b11111110,0b11111111,0b00000011,0b11111111,0b11111111,0b00000011,0b11111111,0b11111110},

    {0b01000010,0b11100111,0b01111110,0b00111100,0b00111100,0b01111110,0b11100111,0b01000010},

    {0b01000000,0b11100000,0b01110000,0b00111111,0b00111111,0b01110000,0b11100000,0b01000000},

    {0b11000011,0b11100011,0b11110011,0b11111011,0b11011111,0b11001111,0b11000111,0b11000011}};
				
				
				
	
	uint8_t row[8]={
		0b00000001,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b10000000
	};
	
	shiftInit();                  //Initialize
	
	int arr[5] = {7,4,11,11,14};
	while(1){
	
	for(uint8_t j=0;j<5;j++){
	
			for(int d = 0;d<50;d++){
		    for(uint8_t i=0;i<8;i++){
		    shiftWriteCol(0b11111111);
			shiftWriteRow(row[7-i]); 
			shiftWriteCol(~ALPHA[arr[j]][i]);
    //Write the data to shift register
			//shiftWriteCol(led[i]);
			_delay_ms(0.2);
			
				}
		}
		
		 shiftWriteCol(0b11111111);
		 _delay_ms(10);	
	
	}
	
	}
}