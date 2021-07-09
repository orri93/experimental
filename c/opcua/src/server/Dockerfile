FROM debian:buster
# Update and install dependencies
RUN apt-get update
RUN apt-get --assume-yes install build-essential libssl-dev python ninja-build cmake git
# Get the open62541
WORKDIR /opt/src
RUN git clone https://github.com/open62541/open62541.git
WORKDIR /opt/src/open62541
RUN git checkout v1.2.2
# Build the open62541
RUN cmake -E make_directory /opt/build/open62541
RUN cmake -E chdir /opt/build/open62541 cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/install/open62541 -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=On -DCMAKE_BUILD_TYPE=release /opt/src/open62541
# Copy the source to image
#WORKDIR /opt/opcuaex
#COPY . .
# Create directories with CMake
#RUN cmake -E make_directory /opt/build/release
# Create the build
#RUN cmake -E chdir /opt/build/release cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/artifacts/release -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=On -DCMAKE_BUILD_TYPE=release -DEM_TARGET:BOOL=Off -DBUILD_TESTS:BOOL=On /opt/static/
# Build with CMake
#RUN cmake --build /opt/build/release --target all --config release
