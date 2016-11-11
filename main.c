//----------------------------------------------------------------------------
// C main line
//----------------------------------------------------------------------------

#include <m8c.h>        // part specific constants and macros.
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules.
#include "stdio.h"

unsigned int result[],samples[];
unsigned int sum=0,avg=0;
//int mode_flag = 0;
unsigned int i = 0,counter;
unsigned int check = 0;
unsigned sound_flag=0,traditional_flag=0;
unsigned int temp=0,max,min,average,m,accuracy;
unsigned int accuracyState = 1,mode_flag = 0;
unsigned msecDelay = 0;
int ch = 0;
void Accuracy_mode(){
	clear_LCD();
	LCD_Position(0,0);
	LCD_PrCString("Accuracy Mode");
	while (1){
		if (accuracyState ==1){
			LCD_Position(1,0);
			LCD_PrCString("1.0");
		}else if (accuracyState ==2){
			LCD_Position(1,0);
			LCD_PrCString("0.5");
		}else if (accuracyState ==3){
			LCD_Position(1,0);
			LCD_PrCString("0.1");
		}
		if(PRT0DR & 0x01){
			check_press();
		}
		if (ch==1){
			accuracyState +=1;
			if (accuracyState >3){
				accuracyState =1;
			}
			ch =0;
		}else if (ch >=2) {
			ch =0;
			mode_flag=2;
			break;
		}
}
}


void display(){

	int i=0,j=0,count=0,z=0;
	int hr=0,min=0,sec=0,msec=0;
	
	//LCD_Start();
	//LCD_Position(0,0);
	//LCD_PrCString("Traditional Mode");
	LCD_Position(1,0);
	LCD_PrHexByte(hr);
	LCD_PrCString(":");
	LCD_PrHexByte(min);
	LCD_PrCString(":");
	LCD_PrHexByte((sec));
	if (accuracyState !=1){
		LCD_PrCString(":");
		LCD_PrHexByte((msec));
		if (accuracyState ==2){
		msecDelay =5000;
		}else if (accuracyState ==3 ){
			msecDelay =16000;
		}
	}
	//LCD_Position(1,6);
	
	while(sec<=89){
	      LCD_Position(1,6);
			LCD_PrHexByte((sec));
		if (accuracyState ==1){
			delay(1);
		}else {
			msec=0;
			while (msec <=153){
				LCD_Position(1,9);
				LCD_PrHexByte(msec);
				for (z=0;z<msecDelay;z++){}
				if (accuracyState ==3){
					msec++;
				}else {
					msec +=16;
				}
				 
				if (msec==10 ||msec==26|| msec==42||msec==58 || msec==74 ||msec==90 || msec==106 ||msec==122 || msec==138 ){
						msec +=6;
				}
				
			}		
		}	
			
			sec++;
		   count++;
		if (sec==10 ||sec==26|| sec==42||sec==58 || sec==74 ){
			sec +=6;
		}
			if (sec==89){
				sec=0;
				min++;
				if (min==10 ||min==26|| min==42||min==58 || min==74 ){
			sec +=6;
		}
				LCD_Position(1,3);
				LCD_PrHexByte(min);
				if (min == 89){
					min = 0;
					LCD_Position(1,0);
				LCD_PrHexByte(min);
				for (i=0;i<20000;i++){}
				}
			
			}
			if(PRT0DR & 0x01 || DUALADC_iGetData1()>110){
				//delay(10);
				samples[counter] =--count;
				counter ++;
				sound_flag =1;
				break;
			}	
	}		
}

void PSOC_Innitialize(){
	
	PGA_1_Start(PGA_1_HIGHPOWER);
	PGA_2_Start(PGA_2_HIGHPOWER);
	LPF2_3_Start(LPF2_3_HIGHPOWER);
    M8C_EnableGInt;                     // Enable global interrupts
	DUALADC_Start(DUALADC_HIGHPOWER); // Turn on Analog section
    DUALADC_SetResolution(7);         // Set resolution to 7 Bits
    DUALADC_GetSamples(0);
	LCD_Start();
}


void sound_mode(){
	//clearLCD();
	LCD_Position(0,0);
	LCD_PrCString("              ");
	LCD_Position(0,0);
	LCD_PrCString("Sound Mode");
	
	while (1){
	     int currentdata = 0;
    	if(DUALADC_fIsDataAvailable()) // Wait for data to 
		{   //avg=120;                             // be ready.
        	//currentdata = DUALADC_iGetData1();    // Get Data		
			if (DUALADC_iGetData1ClearFlag() > 110){
			
			   display();
			   if (sound_flag == 1){
			      break;
			   }
			}
		}


    }

}

void microphone_sensitivity(void)
{   
	LCD_Position(0,0);
	LCD_PrCString("              ");
	LCD_Position(0,0);
	LCD_PrCString("Calibration");

	i =0;
    while (i <20)// Main loop 
    {   
	 	if(DUALADC_fIsDataAvailable()) // Wait for data to 
		{   //avg=120;                             // be ready.
        	result[i] = DUALADC_iGetData1();    // Get Data		
			DUALADC_ClearFlag();
			sum += result[i];
			i++;
		}
	}
	 avg = sum/20;
	 LCD_Position(1,0);
      LCD_PrCString("            ");

}
	

void measurement_mode()
{
	

	LCD_Position(0,0);
	LCD_PrCString("Traditional Mode");
	LCD_Position(1,8);
	LCD_PrCString("    ");
	display();
   	

}

 void memory_mode(){
	max = 0;
	min = 0;
	temp = 0;
	average = 0;
	//clearLCD();
	for (m=0;m<counter-1;m++){
		temp += samples[m];
	}
	average = temp/(counter-1);
	max = min = samples[0];
	for (m=1;m<counter-1;m++){
		if (max<samples[m])
			max = samples[m];
		if (min>samples[m])
			min = samples[m];
	}
	LCD_Position(0,0);
	LCD_PrCString("                  ");
	LCD_Position(0,0);
	LCD_PrCString("Max:");
	LCD_Position(0,4);
	LCD_PrHexByte(max);
	LCD_Position(0,6);
	LCD_PrCString("  ");
	LCD_Position(0,8);
	LCD_PrCString("Min:");
	LCD_Position(0,12);
	LCD_PrHexByte(min);
	LCD_Position(0,15);
	LCD_PrCString("  ");
	LCD_Position(1,0);
	LCD_PrCString("                 ");
	LCD_Position(1,0);
	LCD_PrCString("Avg:");
	LCD_Position(1,4);
	LCD_PrHexByte(average);
	LCD_Position(1,6);
	LCD_PrCString("            ");
	

}

void delay(int count){
	long  i=0,j=0;
	for(i=0;i<count;i++){
		for(j=0;j<160000;j++){}
    
    }

}
void check_press()
{	
	ch =0;
	while (PRT0DR & 0x01 ){
		delay(1);
		ch++;
	}
}
	

void check_long_press(){
	check =0;
	while (PRT0DR & 0x01 ){
		delay(1);
		check++;
	}

}

void check_mode(){
	if (check==1 && mode_flag ==0){
      mode_flag = 1;	
	
	}else if (check>=2){
		mode_flag += 1;
		if (mode_flag >5){
			mode_flag = 1;
		}
     }
}	

void clear_LCD(){
   LCD_Position(0,0);
   LCD_PrCString("                    ");
   LCD_Position(1,0);
   LCD_PrCString("                    ");	
}

	
void main(void)
{
	// M8C_EnableGInt ; // Uncomment this line to enable Global Interrupts
	// Insert your main routine code here.*/
	PSOC_Innitialize();
	accuracyState=1,accuracy=1;
	while (1){
		check_long_press();
		check_mode();
		if (mode_flag ==1){
			Accuracy_mode();
		}
	   
		else if (mode_flag==2) //Checks the state of the button. If the button is pressed then it will execute.
		{    
			measurement_mode();
			delay(3);	
		}
		else if (mode_flag ==3){
			memory_mode();
		}
		else if(mode_flag == 4){
			
			microphone_sensitivity();
		
		}
		else if (mode_flag == 5){
			
			sound_mode();
			
			
	    }
		
	
	
    }	
}
	
