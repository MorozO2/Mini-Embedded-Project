#include "lcd.h"
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdlib.h>

//LCD PINS
#define rs_pin 30
#define e_pin 21
#define p_1 22
#define p_2 23
#define p_3 24
#define p_4 25
#define sda_p 8
#define scl_p 9


int main()
{
	
    char ip_addr[12];
    char temp[3];
    int lcd = 0;
	int fd = 0;
	int data = 0;
	int sda = 0;
	struct ifreq ifr;
	
	
	
	//GET THE IP ADDRESS
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
    memcpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    strcpy(ip_addr, inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));

	//INITIALIZE wiringPi
	wiringPiSetup();
	
	//SETUP LED PIN
	pinMode(0,OUTPUT);
	digitalWrite(0,0);
	
	//I2C READ ADDRESS
	sda = wiringPiI2CSetup(0x4a);
	
	//INITIALIZE AND CLEAR LCD
	lcd = lcdInit(2, 16, 4, rs_pin, e_pin, p_1, p_2, p_3, p_4, 0,0,0,0);
	lcdClear(lcd);
	lcdPosition(lcd, 0, 0);
	
	//PRINT THE DEVICE IP ADDRESS
	lcdPrintf(lcd, "IP: %s", ip_addr);
	
	while(1)
	{
		//CLEAR PART OF THE LOWER HALF OF THE SCREEN
		lcdPosition(lcd, 8, 1);
		lcdPuts(lcd, "    ");
		
		//TURN OFF LED
		digitalWrite(0,0);
		
		//GET DATA FROM I2C
		data = wiringPiI2CRead(sda);
		
		//ADJUST TO GET NEGATIVE TEMPERATURE VALUE
		if((data&0x80) != 0)
		{
				data = data & 0x7F;
				data = data - 128;
		}
		
		//CONVERT DATA TO STRING
		sprintf(temp, "%d", data);
		
		//PRINT TEMPERATURE
		lcdPosition(lcd, 0, 1);
		lcdPrintf(lcd,"Temp.: %s", temp);
		
		//TURN ON LED
		if(data <= 0)
		{
			digitalWrite(0,1);
		}
		//else {digitalWrite(0,0);}
		//DELAY
		delay(700);		
	}
	
	return 0;
}
