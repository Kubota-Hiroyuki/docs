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
  set FILENAME=%~n1
  rem 接頭の1文字を除いた残りの文字列
  set FILENAME=%FILENAME:~1%
  set EXTENSION=%~x1

  call set AFTER=%FILENAME%_(%YYMMDD%_%MMDD%)%EXTENSION%
  echo %AFTER%
  ren "%BEFORE%" "%AFTER%"

  exit /b

BATファイルで文字列の切り出し
書式	意味
%V%	変数Vの値全体
%V:~m%	m文字目から、最後まで
%V:~m,n%	m文字目から、n文字分
%V:~m,-n%	m文字目から、最後のn文字分を除いたもの
%V:~-m%	後ろからm文字目から、最後まで
%V:~-m,n%	後ろからm文字目から、n文字分
%V:~-m,-n%	後ろからm文字目から、最後のn文字分を除いたもの
%V:c1=c2%	文字c1を文字c2に置換する。それぞれ複数の文字を指定することも可能

C:\>SET STR=abcdefg
C:\>ECHO %STR%
abcdefg

C:\>ECHO %STR:~2%
cdefg

C:\>ECHO %STR:~2,3%
cde

C:\>ECHO %STR:~2,-3%
cd

C:\>ECHO %STR:~-3%
efg

C:\>ECHO %STR:~-3,2%
ef

C:\>ECHO %STR:~-3,-2%
e

C:\>ECHO %STR:~-3,-1%
ef

