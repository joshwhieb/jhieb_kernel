FROM ubuntu:lunar-20221207

RUN apt update
RUN apt install cmake -y
RUN apt install gcc -y
RUN apt install gdb -y
RUN apt install make -y
RUN apt install nasm -y
RUN apt install qemu-system-x86 -y

ARG USERNAME=administrator
ARG USER_UID=1001
ARG USER_GID=$USER_UID

# Create the user  
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    # [Optional] Add sudo support. Omit if you don't need to install software after connecting.
    && apt-get update \
    && apt-get install -y sudo \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

# ********************************************************
# * Anything else you want to do like clean up goes here *
# ********************************************************

# [Optional] Set the default user. Omit if you want to keep the default as root.
USER $USERNAME
