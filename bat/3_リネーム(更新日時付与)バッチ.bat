@echo off
rem ----------------------------------------------------
rem 
rem ----------------------------------------------------

:LOOP
  if "%~1" == "" goto END
  call :CHIKAN %1
  shift
  goto LOOP

:END
  pause
  exit

:CHIKAN
  echo %1
  set BEFORE=%~1
  set DATE=%~t1
  echo %DATE%
  set YYMMDD=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%
  set MMDD=%DATE:~11,2%%DATE:~14,2%
  set FILENAME=%~n1
  set EXTENSION=%~x1

  call set AFTER=%FILENAME%_(%YYMMDD%_%MMDD%)%EXTENSION%
  echo %AFTER%
  ren "%BEFORE%" "%AFTER%"

  exit /b
