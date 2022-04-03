	#include <stdio.h>
	/* copy input to output; 2nd version */
	main ()
	{	
		//A char is usually one byte (ASCII-8 bits), getchar should be able to read all these 256 different combinations and in addition
		//should also be able to detect EOF, (EOF is a symbolic constant(-1) defined in stdio.h), so it needs to be able to read 257 values
		//therefore it returns an integer and we should use an int to store getchar instead of char
		//after getchar reads the char, it converts it into unsigned char and then to int, so if char is used to store getchar
		//then the code becomes compiler, architecture dependent, whether char is unsigned or signed, if its signed then 
		//it will be able to recognize EOF(-1), but wont read 0xFF, if its unsigned then it will read 0xFF but won't read EOF
		int c;

		printf("EOF: %d", EOF);
		/* every expression has a value which is the value of LHS after assignment  */
		/* the precedence of != is higher than =  */
		while ((c = getchar()) != EOF)
		putchar(c); 

		printf("we're finished ");
		
	}


//https://stackoverflow.com/questions/29043116/confusing-with-buffer-and-eof-and-getchar

// The standard I/O functions return EOF when the underlying read() system calls return 0 bytes available to be read — or when there's an error.

// When you type abcControl-Z on Windows (you'd type Control-D instead on Unix), then:

//     The terminal driver makes the characters currently in its buffer (abc) available to the read() system call.
//     The read() system call populates the standard I/O buffer with those characters.
//     The successive getchar() calls return a, b and c.
//     The next getchar() waits for more input.

// This wasn't EOF; it was just flushing the characters on the line to the program. To get the effect of EOF, you'd have to type Control-Z twice in a row after the abc. The first would flush the abc; the second would generate a read() of 0 bytes, indicating EOF.
// The terminal driver also makes input available when a newline (Enter) is pressed. If you type Control-Z immediately after typing Enter, all zero of the characters pending input are sent from the terminal to the program, so the read() returns 0, so the standard I/O package reports EOF.
// Note that until you hit enter or the EOF indication, you can edit the data in the line with the backspace and other editing options, including deleting all the data. Once you've hit enter or the EOF indication, you can no longer edit the data that was made available to read().
// The analogous behaviour occurs on Unix; you type the EOF indication twice to terminate the input mid-line; once to terminate the input at the start of a line.