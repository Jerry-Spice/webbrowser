all:
	g++.exe \
	    src/main.cpp \
	    src/Requests/Request.cpp \
	    src/Requests/Response.cpp \
	    src/Requests/Sockets.cpp \
	    -o Browser.exe \
	    -I"C:/Program Files/OpenSSL-Win64/include" \
	    "C:/Program Files/OpenSSL-Win64/lib/VC/x64/MD/libssl.lib" \
	    "C:/Program Files/OpenSSL-Win64/lib/VC/x64/MD/libcrypto.lib" \
	    -lws2_32
