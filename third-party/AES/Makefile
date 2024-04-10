FLAGS = -Wall -Wextra

build_debug:
	docker-compose exec aes g++ $(FLAGS) -g ./src/AES.cpp ./dev/main.cpp -o bin/debug

build_release:
	docker-compose exec aes g++ $(FLAGS) -O2 ./src/AES.cpp ./dev/main.cpp -o bin/release

clean:
	rm -rf bin/*