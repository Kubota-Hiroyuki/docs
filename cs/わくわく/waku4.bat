@echo off
rem set CSC="C:\Windows\Microsoft.NET\Framework\v4.0.30319\csc.exe"
set CSC="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\Roslyn\csc.exe"
echo %~dpnx0
del %~dpn0.exe
del Win32WrapLib.dll

rem Console�p
rem %CSC% %~dpn0.cs

rem dll�r���h
%CSC% -target:library Win32WrapLib.cs
set REF1=/r:Win32WrapLib.dll
rem Form�p
%CSC% %REF1% /t:winexe %~dpn0.cs

rem Form(WPF)�p
rem set opt=/nologo /r:WPF\PresentationCore.dll;WPF\PresentationFramework.dll;WPF\WindowsBase.dll;System.Xaml.dll
rem %CSC% /t:winexe %opt% %~dpn0.cs

%~dpn0.exe
pause
exit

rem �E�Q�Ɛݒ肵�����Ƃ�
rem  1.�G�C���A�X��
rem set REF1=/reference:System.Windows.Forms.DataVisualization.dll
rem  2.�t���p�X��
rem set REF2=/r:"C:\Windows\Microsoft.NET\Framework64\v4.0.30319\System.Windows.Forms.DataVisualization.dll"
rem  �R���p�C���I�v�V����
rem %CSC% %REF1% %REF2% %~dpn0.cs