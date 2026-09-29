CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRC_APP = src/driver_adc.c src/driver_pwm.c src/app_light_ctrl.c src/app_telemetry.c main.c
SRC_TEST = src/driver_adc.c src/driver_pwm.c src/app_light_ctrl.c tests/test_light_ctrl.c

all: slsc_app.exe test_runner.exe

slsc_app.exe: $(SRC_APP)
	$(CC) $(CFLAGS) $(SRC_APP) -o slsc_app.exe

test_runner.exe: $(SRC_TEST)
	$(CC) $(CFLAGS) $(SRC_TEST) -o test_runner.exe

test: test_runner.exe
	./test_runner.exe

clean:
	del /Q *.exe 2>nul || rm -f *.exe
