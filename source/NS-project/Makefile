SRCDIR = ../src

format:
	./astyle.sh FORMAT $(SRCDIR)

format-check:
	./astyle.sh CHECK $(SRCDIR)

power:
	bash -c "./build.sh"

main:
	bash -c "./build_main.sh"

list:
	bash -c "ls /opt/"

clean:
	rm ./bin/*
	rm ./objects/*
