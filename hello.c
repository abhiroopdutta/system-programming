
	#include <stdio.h>

	/* symbolic constants not variables, so do not appear in declarations, usually written in upper case to distinguish from variables  */
	#define LOWER 0
	#define UPPER 300
	#define Step 20

	/* print Fahrenheit-Celsius table for fahr = 0, 20, ..., 300 */

	main()
	{
		printf("Temperature Table Fahrenheit, Celsius\n");
		float fahr, celsius;
		int lower, upper, step;

		lower = 0;	/* lower limit of temperature table */
		upper = 300;	/* upper limit */
		step = 20;	/* step size */
		
		fahr = lower;
		while (fahr <= upper)
	       	{
			celsius = (5.0/9.0) * (fahr-32.0) ;	/* multiplying by 5/9 directly would result 0, as integer division truncates  */
			/* if a statement involves a floating point and integer then the integer would be converted to floating point  */
			/*  %0 for octal, %x for hexadecimal, %c for character, %8 for character string, and %% for itself */


			printf("%3.0f\t%6.1f\n", fahr, celsius);
		       	/* 3%d - print at least 3 characters wide, %.2f - print 2 characters after decimal point  */
			
			fahr = fahr + step;
		}
	}

	
