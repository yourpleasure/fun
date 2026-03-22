#!/bin/bash

# 安装脚本

# 脚本路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIPT_PATH="$SCRIPT_DIR/time_dog.py"
APPLESCRIPT_PATH="$SCRIPT_DIR/TimeDog.applescript"

# 设置脚本执行权限
chmod +x "$SCRIPT_PATH"
chmod +x "$APPLESCRIPT_PATH"

# 创建应用程序包
APP_DIR="$HOME/Applications/TimeDog.app"
rm -rf "$APP_DIR"
mkdir -p "$APP_DIR/Contents/MacOS"
mkdir -p "$APP_DIR/Contents/Resources"

# 创建Info.plist文件
cat > "$APP_DIR/Contents/Info.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>TimeDog</string>
    <key>CFBundleIdentifier</key>
    <string>com.timedog</string>
    <key>CFBundleName</key>
    <string>Time Dog</string>
    <key>CFBundleVersion</key>
    <string>1.0</string>
    <key>LSUIElement</key>
    <true/>
</dict>
</plist>
EOF

# 创建启动脚本
cat > "$APP_DIR/Contents/MacOS/TimeDog" << EOF
#!/bin/bash
python3 "$SCRIPT_PATH" --monitor
EOF

# 设置执行权限
chmod +x "$APP_DIR/Contents/MacOS/TimeDog"

# 添加到登录项
osascript << EOF
tell application "System Events"
    if not (exists login item "Time Dog") then
        make new login item at end with properties {
            name:"Time Dog",
            path:"$APP_DIR",
            hidden:true
        }
    end if
end tell
EOF

# 启动应用程序
open -a "$APP_DIR"

echo "Time Dog 已安装成功！"
echo "程序将在系统启动时自动运行。"
echo "使用方法："
echo "1. 复制一个10位或13位的数字时间戳到剪贴板"
echo "2. 程序会自动检测并转换为可读时间"
echo "3. 转换结果会通过通知显示"
