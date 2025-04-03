# Docker made for testing the gitlab ci/cd pipeline
FROM ubuntu:20.04

RUN apt update
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata

COPY package_list.txt /tmp/package_list.txt
RUN xargs apt-get install -y < /tmp/package_list.txt
RUN rm -rf /var/lib/apt/lists/* \
    && apt autoremove

# if need to build the project
# WORKDIR /home
# RUN git clone https://github.com/n2oblife/CHIP-8_emulator.git
# RUN cd CHIP-8_emulator/ && make all debug && make doc && make coverage

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
# CMD ["tail", "-f", "/dev/null"]
