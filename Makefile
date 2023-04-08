.PHONY: all build run clean

all: build
	@cmake -S . -B build
	@cmake --build build

run:
	@./build/hmanage

clean:
	rm -rf build

build:
	@mkdir -p $@