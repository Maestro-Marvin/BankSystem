FROM ubuntu:latest

RUN apt-get update && apt-get install -y build-essential cmake

RUN mkdir /app
COPY . /app
WORKDIR /app

RUN chmod -R 777 /src/Interface
RUN chmod +rwx /src/Interface/info.txt

RUN mkdir build && cd build && cmake ../src && cmake --build .

CMD ["./build/BankSystem"]
