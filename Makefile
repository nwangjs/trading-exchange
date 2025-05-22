.PHONY: install build test lint-check format-check format clean

install:
	conan install . --build=missing

build: install
	cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -G Ninja
	cd build && cmake --build . -j

test: build
	cd build && ./trading-exchange_tests

lint-check:
	run-clang-tidy -j $(shell nproc) -p build

format-check:
	find src tst -name '*.cpp' -o -name '*.hpp' | xargs clang-format --style=file --Werror --dry-run

format:
	find src tst -name '*.cpp' -o -name '*.hpp' | xargs clang-format --style=file -i
	run-clang-tidy -fix -j $(shell nproc) -p build

clean:
	rm -rf build

