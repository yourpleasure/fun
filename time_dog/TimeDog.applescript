#!/usr/bin/env osascript

-- Time Dog 启动脚本

on run
    set scriptPath to POSIX path of (path to me as text) & "../time_dog.py"
    do shell script "python3 " & quoted form of scriptPath & " --monitor"
end run
