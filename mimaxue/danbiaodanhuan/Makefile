all:keygen encryptdbdh decryptdbdh calcFreq getkey
keygen:keygen.c
	gcc $< -o $@
encryptdbdh:encryptdbdh.c
	gcc $< -g -o $@
decryptdbdh:decryptdbdh.c
	gcc $< -g -o $@

calcFreq:calcFreq.c
	gcc $< -g -o $@
getkey:getkey.c	
	gcc $< -g -o $@

clean:
	rm -rf keygen encryptdbdh decryptdbdh calcFreq getkey
