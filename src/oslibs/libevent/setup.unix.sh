if [ ! -d libevent-2.0.21-stable ]; then
	tar xvf libevent-2.0.21-stable.tar.gz;

	cp bufferevent_sock.c libevent-2.0.21-stable/;
	cp evutil.c libevent-2.0.21-stable/;
	cp listener.c libevent-2.0.21-stable/;
	cp util-internal.h libevent-2.0.21-stable/;
	cp config.h.in libevent-2.0.21-stable/;
	cp configure libevent-2.0.21-stable/;
fi

cd libevent-2.0.21-stable;

./configure;
make;

mkdir -p ../../../../../dependencies/lib/gcc;
cp .libs/libevent.so ../../../../../dependencies/lib/gcc/;
cp .libs/libevent_core*.so* ../../../../../dependencies/lib/gcc/;
cp .libs/libevent_extra*.so* ../../../../../dependencies/lib/gcc/;
cp .libs/libevent_pthreads*.so* ../../../../../dependencies/lib/gcc/;

mkdir -p ../../../../../dependencies/inc/libevent;
cp -r include/* ../../../../../dependencies/inc/libevent/;

mkdir -p ../../../../../dependencies/inc/libevent/WIN32-Code;
cp -r WIN32-Code/* ../../../../../dependencies/inc/libevent/WIN32-Code/;

cd ..;