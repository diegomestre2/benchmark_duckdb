.PHONY: all opt unit clean debug release

all: release


ifeq ($(GEN),ninja)
	GENERATOR=-G "Ninja"
	FORCE_COLOR=-DFORCE_COLORED_OUTPUT=1
endif


clean:
	rm -rf build

debug:
	mkdir -p build/debug && \
	cd build/debug && \
	cmake  -DCMAKE_BUILD_TYPE=Debug ../.. && \
	cmake --build .


release:
	mkdir -p build/release && \
	cd build/release && \
	cmake -DCMAKE_BUILD_TYPE=Release ../.. && \
	cmake --build .