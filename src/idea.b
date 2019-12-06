
[A]	usage
	./binw	[file.bwl]			-> generate binary using BWL file
	./binw	--convert [file]	-> convert binary to BWL
	./binw	-h					-> display help
	./binw						-> direct input mode


§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§


[B]	BWL (binary writting language)


1) basic input

	hexa:	xFF, hFF, 0xFF
	deci:	255
	octal:	0377, o377
	bin:	b11111111, b1111_1111	// '_' is ignored
	string:	"string"				// utf-8
	char:	'c'

	example:
			0 255 x12 b100 017 "titi" '\n'

2) lock base

	<BASE>
	BASE = 0x, x, h, 0, o, b, *
	* = all bases
	(strings and characters are not affected)

	example:
			<0x> 00 02 1A <*> 255 0xAC b111
			<b> 10000 11__1111_1111 <0> 77 122 <*>
	result:
			x00 x02 x1A 255 xAC b111
			b10000 b11_11111111 077 0122

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

