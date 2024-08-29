@echo off
:begin
cls

echo --------------------------------------------------
echo		Tata340PP40 Microcontroller Menu
echo --------------------------------------------------
echo		@AUTHOR : Bhavay Sen [ ASL-240 ]
echo		Company : Aartech Solonics Limited
echo		Date : 26-08-2024
echo --------------------------------------------------
echo Please load the bootloader if not yet loaded.
echo Note : This is an one-time process.
echo --------------------------------------------------
echo Enter the complete path to the .hex file (including the file name and extension).
set /p "hexfile=Enter the path to the .hex file: "

:: Check if the entered .hex file exists
if not exist "%hexfile%" (
    echo Hex file not found! Please enter a valid path and file name.
    pause
    goto begin
)

:detectmcu
cls

echo Detecting microcontroller type...
echo.

:: Detect the microcontroller signature using avrdude
avrdude -c usbasp -p m328p -v > mcu_detect.log 2>&1

:: Read the detected signature from the log file
findstr /C:"Device signature" mcu_detect.log > nul

if errorlevel 1 (
    echo Error: Unable to detect microcontroller. Please check your connection.
    pause
    goto begin
)

:: Check for ATmega328P signature (0x1E950F)
findstr /C:"Device signature = 0x1e950f" mcu_detect.log > nul
if not errorlevel 1 (
    set mcu_model=m328p
    goto program
)

:: Check for ATmega328PB signature (0x1E9516)
findstr /C:"Device signature = 0x1e9516" mcu_detect.log > nul
if not errorlevel 1 (
    set mcu_model=m328pb
    goto program
)

:: Error if neither signature is detected
echo Error: Unsupported microcontroller. Ensure you are using ATmega328P or ATmega328PB.
pause
goto begin

:program
cls
echo Programming the microcontroller %mcu_model% with %hexfile%...
echo.

:: Program the microcontroller with the selected hex file
avrdude -c usbasp -p %mcu_model% -U flash:w:%hexfile%:i

:: Check if programming was successful
if errorlevel 1 (
    echo.
    echo Error: Programming failed!
) else (
    echo.
    echo Programming successful!
)

:continue
echo.
echo --------------------------------------------------
echo Do you want to program another microcontroller?
echo [1] Yes
echo [2] No (Exit)
set /p "choice=Enter your choice [1 or 2]: "

if "%choice%"=="1" (
    goto begin
) else if "%choice%"=="2" (
    echo Exiting...
    exit
) else (
    echo Invalid choice. Please select 1 or 2.
    pause
    goto continue
)
