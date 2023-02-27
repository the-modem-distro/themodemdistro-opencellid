all: clean ocidconv

ocidconv:
	@${CC} ${LDFLAGS} -Wall -O2 ocid_conv.c -o ocidconv
	@chmod +x ocidconv

clean:
	@rm -rf ocidconv
