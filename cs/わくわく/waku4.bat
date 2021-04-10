@echo off
rem set CSC="C:\Windows\Microsoft.NET\Framework\v4.0.30319\csc.exe"
set CSC="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\Roslyn\csc.exe"
echo %~dpnx0
del %~dpn0.exe
del Win32WrapLib.dll

rem Console用
rem %CSC% %~dpn0.cs

rem dllビルド
%CSC% -target:library Win32WrapLib.cs
set REF1=/r:Win32WrapLib.dll
rem Form用
%CSC% %REF1% /t:winexe %~dpn0.cs

rem Form(WPF)用
rem set opt=/nologo /r:WPF\PresentationCore.dll;WPF\PresentationFramework.dll;WPF\WindowsBase.dll;System.Xaml.dll
rem %CSC% /t:winexe %opt% %~dpn0.cs

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