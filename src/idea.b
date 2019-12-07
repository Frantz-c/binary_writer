
[A]	usage
	./binw	[file.bwl]			-> generate binary using BWL file
	./binw	--convert [file]	-> convert binary to BWL
	./binw	-h					-> display help
	./binw						-> direct input mode


§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§


[B]	BWL (binary writting language)


1) basic input

	hexa:	xFF, hFF, 0xFF
	deci:	255, +255
	octal:	0377, o377
	bin:	$1111_1111, $101011		// '_' is ignored
	string:	"string"				// utf-8
	char:	'c'

	example:
			0 255 x12 b100 017 "titi" '\n'

2) lock base

	<BASE>
	BASE = 'hexa', 'bin' or 'binary', 'oct' or 'octal', '*' or 'all'
	(strings, characters and arguments are not affected)

	example:
			<hexa> 00 02 1A <*> 255 0xAC b111
			<bin> 10000 1111_1111 <octal> 77 122 <all>
	result:
			x00 x02 x1A 255 xAC b111
			b10000 b11111111 077 0122

3) loop

	().n
	n = number of laps

	example:
			(x01 x02).4
	result:
			x01 x02 x01 x02 x01 x02 x01 x02

4) auto-padding block

	[*].n,*=p	or	[*].n,p
	* = padding zone
	n = total size
	p = padding byte

	example:
			[* "a string"].2048,*=0
	result:
			(0).2040 "a string"

5) macro

	%macro(NAME,content)
	NAME = [A-Z][A-Z0-9_]*
	content = bwl code, but macro are forbidden

	example:
			%macro(
				HEADER,
				[x01 xEA xFF *].1024,*=xFF
			)

6) include

	%include("filename")	or
	%include "filename"

	example:
			%include("header.bwl")

