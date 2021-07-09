FROM debian:buster
LABEL maintainer="orri93@gmail.com"

# Update and install dependencies
RUN apt-get update
RUN apt-get --assume-yes install build-essential libssl-dev python ninja-build cmake git
# Get the open62541
WORKDIR /opt/src
RUN git clone https://github.com/open62541/open62541.git
WORKDIR /opt/src/open62541
RUN git checkout v1.2.2
RUN git submodule update --init --recursive
# Build the open62541
RUN cmake -E make_directory /opt/build/open62541
RUN cmake -E chdir /opt/build/open62541 cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/install/open62541 -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=On -DCMAKE_BUILD_TYPE=release /opt/src/open62541
RUN cmake --build /opt/build/open62541 --target install --config release
# Copy the source to image
WORKDIR /opt/src/opcuaex
COPY . .
# Create directories with CMake
RUN cmake -E make_directory /opt/build/opcuaex
# Create the build
RUN cmake -E chdir /opt/build/opcuaex cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/install/opcuaex -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=On -DCMAKE_BUILD_TYPE=release -Dopen62541_DIR:PATH=/opt/install/open62541/lib/cmake/open62541 /opt/src/opcuaex
# Build with CMake
RUN cmake --build /opt/build/opcuaex --target install --config release

EXPOSE 4840
CMD ["/opt/install/opcuaex/bin/opcuaexserver"]