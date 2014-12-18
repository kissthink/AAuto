ttf2eot: OpenTypeUtilities.o ttf2eot.o
	g++ -shared -static-libgcc -static-libstdc++ -o ttf2eot.dll $^ #-o $@ $^

clean:
	rm -f *.o *.core *.obj *.exe  *.dll core ttf2eot
