
walk:	
	ccpsx -O3 -Xo$80100000 walk.c onpol.c -owalk.cpe,walk.sym

for_menu:
        ccpsx -O3 -Xo$80100000 walk.c onpol.c \psx\lib\none2.obj -owalk.cpe
        cpe2x walk.cpe
        del walk.cpe

load:
	pqbload venus.pmd 80040000
	#pqbload m2.pmd 80040000
	pqbload c0sv1.pmd 80080000
	pqbload chara0.sv 80090000
	pqbload chara1.sv 800a0000
	pqbload chara2.sv 800b0000
	pqbload chara3.sv 800c0000

