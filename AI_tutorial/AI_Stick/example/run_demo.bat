CLS
@echo off

rem change to current dir
cd /d %~dp0
set "TMP_SCRIPT=tmp.bat"
set "WGET=..\tool\wget.exe"

rem check wget is exist
If not exist %WGET% (
  echo %WGET% is needed. Please copy tool directory from RK1808.
  goto error
)

if exist %TMP_SCRIPT% (
	del %TMP_SCRIPT%
)
%WGET% http://repo.rock-chips.com/rk1808/run_demo.bat -O %TMP_SCRIPT%

call %TMP_SCRIPT%
