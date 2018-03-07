build:
	@ make -C ./bsp/STM32F103RCT6/ > /dev/null
	@ echo Done
download:
	@ st-flash write ./bsp/STM32F103RCT6/rtthread.bin 0x8000000 > /dev/null
clean:
	@ make -C ./bsp/STM32F103RCT6/ clean > /dev/null
	@ echo Done
erase:
	st-flash erase > /dev/null
run:
	minicom
re:
	make clean
	make build
	make download
