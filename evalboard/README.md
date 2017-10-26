# README

Thermostat build upon blue pill:

http://wiki.stm32duino.com/index.php?title=Blue_Pill

USART1 : one wire interface to ds18b20

USART2 : serial out for debug

USART3 : rs485 connection

How to compile:

git clone https://github.com/janeczkos/thermostat.git
cd thermostat
git submodule init
git submodule update
cd libopencm3
//Compiling libraries only for fresh start
make -j4

cd ../evalboard
//Compiling actual program every time you change something
make


