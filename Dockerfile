FROM gcc:11.2.0

RUN apt-get update && apt-get install -y cmake time build-essential gdb

WORKDIR /usr/src/myapp
COPY Makefile Makefile.googletest /usr/src/myapp/
RUN make googletest

COPY test_all /usr/src/myapp/
COPY src /usr/src/myapp/src/
COPY include /usr/src/myapp/include/

RUN make all
ENV PATH="/usr/src/myapp/bin:${PATH}"
CMD [ "/bin/bash" "-i" ]
