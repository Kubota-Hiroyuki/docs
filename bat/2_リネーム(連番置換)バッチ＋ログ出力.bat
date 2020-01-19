@echo off
rem ----------------------------------------------------
rem
rem (DD)ファイルリネーム(連番置換)バッチ
rem ログ出力付き
rem hoge.txt, aaa.txt → MyFile000.txt, MyFile001.txt
rem
rem ----------------------------------------------------

setlocal enabledelayedexpansion
rem prefix = 接頭文字
rem pad = 有効桁数
rem num = 初期値
set prefix=MyFile
set pad=3
set num=1

  echo 開始>log.txt

:LOOP
  if "%~1" == "" goto END
  set padnum=00000000!num!
  call :CHIKAN %1 %padnum%
  set /a num+=1
  shift
  goto LOOP

  endlocal


:END
  echo 終了>>log.txt
  log.txt
  pause
  exit


:CHIKAN
  echo %1
  set BEFORE=%~1
  set PADN=%~2
  set EXT=%~x1
  set FILENAME=%~nx1
  echo ["%BEFORE%"]	[!prefix!!PADN:~-%pad%!!EXT!]>>log.txt
  ren "%BEFORE%" !prefix!!PADN:~-%pad%!!EXT!

  exit /b
