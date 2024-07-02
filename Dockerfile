# Use the official Ubuntu base image
FROM ubuntu:22.04
ENV TZ=Iran/Tehran \
    DEBIAN_FRONTEND=noninteractive
# Set the maintainer label
LABEL maintainer="m.salamati2000@gmail.com"

# Update the package list and install dependencies
RUN apt-get update && apt-get install -y \
build-essential \
    # qt5-default 
    g++ \
    qtcreator \
    qtbase5-dev \
     qt5-qmake\
      cmake \
     qtbase5-dev \
     qttools5-dev \
      qttools5-dev-tools \
       qtwebengine5-dev \
        libqt5svg5-dev \
         libqt5websockets5-dev \
         libqt5webkit5-dev 
# Copy the project files to the container

COPY . /app
WORKDIR /app
# Set the working directory

# Build the Qt project
RUN ls
RUN cd qamqp-master 
RUN ls
RUN qmake ./qamqp-master/
RUN make ./qamqp-master/
RUN make install ./qamqp-master/
RUN qmake ./QxOrm/ && make ./QxOrm/ && make install ./QxOrm/
RUN qmake && make
ENV MONGODB_HOST=mongo
ENV MONGODB_PORT=27017
# Set the entry point to run the Qt application
CMD ["./webCrawler"]