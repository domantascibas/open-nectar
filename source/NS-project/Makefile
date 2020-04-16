SRCDIR = ../src

format:
	./astyle.sh FORMAT $(SRCDIR)

format-check:
	./astyle.sh CHECK $(SRCDIR)

power:
	bash -c "./build.sh"

power-clean:
	rm -r ./../../BUILD
	bash -c "./build.sh"

main:
	bash -c "./build_main.sh"
	rm -r ./../../BUILD/NUCLEO_F091RC/GCC_ARM/source
	rm -r ./../../BUILD/NUCLEO_F091RC/GCC_ARM/submodules
	rm -f ./../../.mbedignore

main-clean:
	rm -r ./../../BUILD
	bash -c "./build_main.sh"
	rm -r ./../../BUILD/NUCLEO_F091RC/GCC_ARM/source
	rm -r ./../../BUILD/NUCLEO_F091RC/GCC_ARM/submodules
	rm -f ./../../.mbedignore

list:
	bash -c "ls /opt/"

clean:
	rm ./bin/*
	rm ./objects/*
