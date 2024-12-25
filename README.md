# MultiMQ
Custom Multi-threaded Messaging Queue system

---

## Setup Instructions

### 1. Create a Dockerfile
Create a Dockerfile with gRPC and CMake installed.

```dockerfile
FROM ubuntu:latest

RUN apt-get update && apt-get install -y cmake build-essential git

WORKDIR /deps

EXPOSE 9999

RUN git clone --recurse-submodules -b v1.66.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc

RUN mkdir -p /deps/grpc/build && cd /deps/grpc/build && \
        cmake -DgRPC_INSTALL=ON \
        -DgRPC_BUILD_TESTS=OFF \
        .. && \
        make -j8 install
```

---

### 2. Build the Docker Container
Build the container.

```bash
docker build -t myapp .
```

---

### 3. Run the Docker Container
Run the Docker container. 

```bash
docker run -p localhost:9999 -it myapp
```

---

### 4. Clone the Repository
Clone the `MultiMQ` repository into a directory (e.g., `/root/`):

```bash
git clone https://github.com/SaiTarun314/MultiMQ.git
```

---

### 5. Create and Navigate to the Build Folder
Create a build folder in the `MultiMQ` directory and navigate into it:

```bash
mkdir build && cd build
```

---

### 6. Run CMake and Make
Generate the gRPC code, stubs, and other dependencies by running:

```bash
cmake ..
make
```

---

### 7. Run the Executable
Run the generated executable file:

```bash
./multimq
```

---

### 8. Test the Application
Use an API testing application (e.g., Postman) to test the application endpoints. 

> **Note**: Make sure to import the `.proto` files into Postman to define the gRPC service properly.