# Docker made for testing the gitlab ci/cd pipeline
# FROM debian:bookworm-slim AS build
FROM debian:bookworm-slim

# Dependencies
RUN apt-get update \
    && apt-get install -y build-essential curl git g++ python3 make cmake ninja-build libsdl2-dev doxygen doxygen-gui doxygen-latex graphviz lcov \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean

WORKDIR /app
RUN git clone https://github.com/n2oblife/CHIP-8_emulator.git

# RUN cd CHIP-8_emulator/ && make all debug && make doc && make coverage

RUN cd CHIP-8_emulator/ && make debug
RUN cd CHIP-8_emulator/ && make coverage
RUN cd CHIP-8_emulator/ && make doc



# Final image (runtime) if need for opitmization
# FROM debian:bookworm-slim
# COPY --from=build /app/bin/ /app/bin/

# Add color to the terminal and aliase
RUN echo "TERM=xterm-256color" >> /etc/bash.bashrc &&\
echo "alias ls='ls --color=auto'" >> /etc/bash.bashrc &&\
echo "alias grep='grep --color=auto'" >> /etc/bash.bashrc &&\
echo "alias fgrep='fgrep --color=auto'" >> /etc/bash.bashrc &&\
echo "alias egrep='egrep --color=auto'" >> /etc/bash.bashrc &&\
echo "alias ll='ls -alF'" >> /etc/bash.bashrc &&\
echo "alias la='ls -A'" >> /etc/bash.bashrc &&\
echo "alias l='ls -CF'" >> /etc/bash.bashrc

CMD ["/bin/bash"]
