:: This is to allow for one to quickly set up the development environment for OrangeC
:: This consists of: bringing up the MSVC compiler, adding OrangeC's bin to the path
:: and setting the path of OrangeC so that some tools can find it
:: this batch file is used only because the MSVC tools on windows aren't always
:: available to use from the get-go, so we must instantiate them
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
SET PATH=%PATH%;C:\OrangeC\bin
set ORANGEC=c:\ORANGEC