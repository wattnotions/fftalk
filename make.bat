xc16-gcc main.c -mcpu=30F4011 -Wl,--script=p30F4011.gld
if errorlevel 0 xc16-bin2hex a.out