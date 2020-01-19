@echo off
rem ----------------------------------------------------
rem 
rem (DD)ファイルリネーム(文字列置換)バッチ
rem 
rem 
rem 
rem ----------------------------------------------------
rem ポイント1
rem 置換前後の文字列に文字列リテラルを指定する場合、
rem 「set target=%target:置換前文字列=置換後文字列%」
rem 変数展開後の指揮をsetコマンドに渡す必要があるので、
rem 「call set target=%%target:%OLD%=%NEW%%%」
rem ----------------------------------------------------
rem ポイント2
rem 変数から「"」を外す場合、「%HOGE:"=%」
rem ----------------------------------------------------

set OLD=MyFile
set NEW=うんこ

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
  set FILENAME=%~nx1
  call set AFTER=%%FILENAME:%OLD%=%NEW%%%
  echo %AFTER%
  ren "%BEFORE%" "%AFTER%"

  exit /b
