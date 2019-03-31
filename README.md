# binary_writer

easily write binary files

syntax:

	write each byte in hexadecimal: 00 0f ff f1 ...
	                   decimal    : 234d 512d ...
					   octal      : 14o 1605o ...
					   binary     : 110101b 111b ...
	comments start with '#' and end at the end of line.
	simple loops are enable: (00 00).3 = 00 00 00 00 00 00
	                         (00 (01 02).2 ).3 = 00 01 02 01 02 00 01 02 01 02 00 01 02 01 02
	easily print charaters: 'c' '\n'
	            or strings: "it's a String !"

programme usage:
	./binw [file]?

	you can write the code into a file or use stdin.
	program put the result in stdout, and errors in stderr.

	example: ./binw file > file.bin
