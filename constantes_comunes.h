#ifndef CONSTANTES_COMUNES_H
#define CONSTANTES_COMUNES_H

	typedef enum {
		FALSE = 0, TRUE = 1
	}boolean;
	
	#define SEGUNDO 1000		// 1000 ms
	#define NEW_LINE				0x0A
	#define TAB							0x09
	
	__inline int to_uint(char value)
	{
		switch(value)
		{
			case '0':
				return 0;

			case '1':
				return 1;

			case '2':
				return 2;

			case '3':
				return 3;

			case '4':
				return 4;

			case '5':
				return 5;

			case '6':
				return 6;

			case '7':
				return 7;

			case '8':
				return 8;

			case '9':
				return 9;

			default:
				return -1;
		}
	}
	
	__inline int to_string(int val)
	{
		int retVal = ' ';
		switch(val)
		{
			case 0:			
				retVal = ' ';
				break;
			
			case 1:
				retVal = '1';
				break;
				
			case 2:
				retVal = '2';
				break;
				
			case 3:
				retVal = '3';
				break;
				
			case 4:
				retVal = '4';
				break;
				
			case 5:
				retVal = '5';
				break;
				
			case 6:
				retVal = '6';
				break;
				
			case 7:
				retVal = '7';
				break;
				
			case 8:
				retVal = '8';
				break;
				
			case 9:
				retVal = '9';
				break;
		}
		
		return retVal;
	}

	
#endif // CONSTANTES_COMUNES_H

