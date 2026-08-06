.PHONY: run debug

run:
	cmake -S . -B build
	cmake --build build
	open -n build/geometric-mechanics-sandbox.app

debug:
	cmake --build build
	./build/geometric-mechanics-sandbox.app/Contents/MacOS/geometric-mechanics-sandbox
