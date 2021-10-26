#include<reg51.h>
#include<stdio.h>

sfr lcd_port = 0xA0;
sbit rs = P3^5;
sbit rw = P3^6;
sbit en = P3^7;

sbit r0 = P1^0;
sbit r1 = P1^1;
sbit r2 = P1^2;
sbit r3 = P1^3;
sbit c0 = P1^4;
sbit c1 = P1^5;
sbit c2 = P1^6;
sbit c3 = P1^7;

char s1[] = "Enter 1st no ";
char s2[] = "Enter operator ";
char s3[] = "Enter 2nd no ";

void delay(unsigned long count);
void lcd_data(unsigned char value);
void lcd_cmd(unsigned char command);
void setup_lcd();
char scan_key();
short int power(short int a,short int b);
void display(char* str);
void num_to_str(long int num);
void calculate_result(long int a,long int b,unsigned char c);
long get_num();
 

void main(){
	while(1){
	unsigned char op;
	long int num1,num2;
	setup_lcd();
	display(s1);
	lcd_cmd(0x01);
	num1 = get_num();
 
	display(s2);
	op = scan_key();
	lcd_cmd(0x01);
		
	display(s3);
	lcd_cmd(0x01);
	num2 = get_num();
 
	calculate_result(num1,num2,op);
	}
}

void delay(unsigned long count){
	unsigned char i;
	while(count){
		i = 115;
		while(i>0){
			i--;
		}
		count--;
	}
}


void lcd_cmd(unsigned char command){
	delay(1000);
	lcd_port = command;
	rs = 0;
	rw = 0;
	en = 1;
	delay(50);
	en = 0;
	delay(50);
}

void lcd_data(unsigned char value){
	lcd_port = value;
	rs = 1;
	rw = 0;
	en = 1;
	delay(50);
	en = 0;
	delay(50);
}

void setup_lcd(){
	delay(200);
	lcd_cmd(0x30);
	delay(200);
	lcd_cmd(0x30);
	delay(200);
	lcd_cmd(0x38);
	lcd_cmd(0x0F);
	lcd_cmd(0x01);
	lcd_cmd(0x06);
	lcd_cmd(0x80);
}


char scan_key(){
	unsigned char k;
	k = 'm';
	while(!(k== '0' || k == '1' || k== '2' || k=='3' || k=='4' || k=='5' || k== '6' || k=='7' || k=='8' || k=='9' || k=='/' || k=='*' || k=='+' || k=='-' || k=='c' || k == '='))
	{
		r0 = 0;r1=1;r2=1;r3 =1;
		if(r0 ==0 && c0 ==0){lcd_data('7');delay(500);return k= '7';}
		if(r0==0 && c1==0){ lcd_data('8'); delay(500); return k = '8';}
		if(r0==0 && c2 ==0){lcd_data('9'); delay(500); return k = '9';}
		if(r0==0 && c3 ==0){lcd_data('/');delay(500); return k = '/';}
		
		r0=1;r1=0;r2=1;r3=1;
		if(r1==0 && c0 ==0){lcd_data('4');delay(500);return k= '4';}
		if(r1==0 && c1==0){lcd_data('5'); delay(500);return k='5';}
		if(r1==0 && c2 == 0){lcd_data('6');delay(500);return k='6';}
		if(r1==0 && c3 == 0){lcd_data('*');delay(500);return k='*';}
		
		r0=1;r1=1;r2=0;r3=1;
		if(r2==0 && c0==0){lcd_data('1');delay(500);return k='1';}
		if(r2==0 && c1 ==0){lcd_data('2');delay(500);return k='2';}
		if(r2==0 && c2==0){lcd_data('3');delay(500);return k='3';}
		if(r2==0 && c3==0){lcd_data('-');delay(500); return k='-';}
		
		r0=1;r1=1;r2=1;r3=0;
		if(r3==0 && c0==0){lcd_data('c');delay(500);return k='c';}
		if(r3==0 && c1==0){lcd_data('0');delay(500);return k='0';}
		if(r3==0 && c2==0){lcd_data('=');delay(500);return k='=';}
		if(r3==0 && c3==0){lcd_data('+');delay(500);return k='+';}
	}
		return 0;
}
			
void display(char *str){
	while(*str != '\0'){
		lcd_data(*str);
		str++;
	}
}


long get_num(){
	char key = scan_key();
	char arr[8];
	long sum=0 ;
	short i=0,k;
	while(!(key == '+' || key =='-' || key =='/' || key=='*' || key=='c' || key=='=' )){
		arr[i] = key;
		i++;
		key = scan_key();
		delay(30);
	}
	for(k=0;k<i;k++){
		sum+= (arr[i-k-1] - '0')*power(10,k);
	}
	lcd_cmd(0x01);
	return sum;
}
 

short int power(short int a,short int b){
	short int result = 1;
	short int count;
	for(count=0;count<b;count++){
		result *= a;
	}
	return result;
}
 

void num_to_str(long int real_num){
	long int num;
	char temp[15];
  char i=0,k;
	char var;
	if(real_num>=0){
		num = real_num;
	}
	else{
		num = -1*real_num;
		lcd_data('-');
	}
	
  while(num>0){
			temp[i] = ((num%10) + '0');
		  i++ ;
		  num = num/10;
	}
	if(i==0){
		lcd_data('0');
		return;
	}
	temp[i] = '\0';
	for(k=0;k<i/2;k++){
		var = temp[k];
		temp[k] = temp[i-k-1];
		temp[i-k-1] = var;
	}
	display(temp);
}

	
void calculate_result(long int a,long int b,unsigned char c){
	long int result = 0,remain;
	switch(c){
		case '+':
		{
			result = a +b;
			break;
		}
		case '-':
		{
			result = a -b;
			break;
		}
		case '/':
		{
			result = a/b;
			remain = a%b;
			num_to_str(result);
			lcd_data(' ');
			num_to_str(remain);
			return;
			break;
		}
		case '*':
		{
			result = a*b;
			break;
		}
	}
   num_to_str(result);
	 
}
