@echo off
rem set CSC="C:\Windows\Microsoft.NET\Framework\v4.0.30319\csc.exe"
rem set CSC="C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\MSBuild\15.0\Bin\Roslyn\csc.exe"
set CSC="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\Roslyn\csc.exe"
echo %~dpnx0
del %~dpn0.exe

rem Console用
%CSC% %~dpn0.cs

rem Form用
rem %CSC% /t:winexe %~dpn0.cs

%~dpn0.exe
pause
exit

rem ・参照設定したいとき
rem  1.エイリアス名
rem set REF1=/reference:System.Windows.Forms.DataVisualization.dll
rem  2.フルパス名
rem set REF2=/r:"C:\Windows\Microsoft.NET\Framework64\v4.0.30319\System.Windows.Forms.DataVisualization.dll"
rem  コンパイルオプション
rem %CSC% %REF1% %REF2% %~dpn0.cs