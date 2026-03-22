#!/usr/bin/env python3
import time
import re
import subprocess
import argparse

# 从剪贴板获取内容
def get_clipboard():
    try:
        result = subprocess.run(['pbpaste'], capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError:
        return ""

# 发送通知并复制到剪贴板
def send_notification(title, message, original_timestamp=None):
    try:
        # 发送通知
        subprocess.run([
            'osascript', '-e',
            f'display notification "{message}" with title "{title}"'
        ], check=True)
        # 提取时间部分
        time_part = message.split(': ', 1)[1] if ': ' in message else message
        # 构建复制内容，包含原始时间戳和转换后的时间
        if original_timestamp:
            copy_content = f"{original_timestamp} → {time_part}"
        else:
            copy_content = time_part
        # 复制到剪贴板
        subprocess.run(['pbcopy'], input=copy_content.encode('utf-8'), check=True)
    except subprocess.CalledProcessError:
        pass

# 处理输入的时间戳
def process_timestamp(input_str):
    # 去除空格和逗号
    cleaned = re.sub(r'[\s,]', '', input_str)
    
    # 检查是否为纯数字
    if not cleaned.isdigit():
        return None
    
    timestamp = int(cleaned)
    
    # 判断时间戳长度
    if len(cleaned) == 10:
        # 10位时间戳（秒）
        return time.localtime(timestamp), 0
    elif len(cleaned) == 13:
        # 13位时间戳（毫秒）
        return time.localtime(timestamp / 1000), timestamp % 1000
    elif len(cleaned) == 16:
        # 16位时间戳（微秒）
        return time.localtime(timestamp / 1000000), int((timestamp % 1000000) / 1000)
    else:
        return None

# 监控剪贴板变化
def monitor_clipboard():
    last_content = ""
    while True:
        current_content = get_clipboard()
        if current_content != last_content:
            last_content = current_content
            result = process_timestamp(current_content)
            if result:
                time_struct, milliseconds = result
                time_str = time.strftime('%Y-%m-%d %H:%M:%S', time_struct)
                if milliseconds > 0:
                    time_str += f'.{milliseconds:03d}'
                send_notification('Time Dog', f'Converted time: {time_str}', current_content)
        time.sleep(1)  # 每秒检查一次

# 主函数
def main():
    parser = argparse.ArgumentParser(description='Convert timestamp to readable time')
    parser.add_argument('-c', '--clipboard', action='store_true', help='Read from clipboard')
    parser.add_argument('-m', '--monitor', action='store_true', help='Monitor clipboard changes')
    parser.add_argument('timestamp', nargs='?', help='Timestamp to convert')
    args = parser.parse_args()
    
    if args.monitor:
        monitor_clipboard()
    elif args.clipboard or args.timestamp is None:
        input_str = get_clipboard()
        if not input_str:
            send_notification('Time Dog', 'No input found')
            return
        result = process_timestamp(input_str)
        if result:
            time_struct, milliseconds = result
            time_str = time.strftime('%Y-%m-%d %H:%M:%S', time_struct)
            if milliseconds > 0:
                time_str += f'.{milliseconds:03d}'
            send_notification('Time Dog', f'Converted time: {time_str}', input_str)
        else:
            send_notification('Time Dog', 'Invalid timestamp format')
    else:
        input_str = args.timestamp
        result = process_timestamp(input_str)
        if result:
            time_struct, milliseconds = result
            time_str = time.strftime('%Y-%m-%d %H:%M:%S', time_struct)
            if milliseconds > 0:
                time_str += f'.{milliseconds:03d}'
            send_notification('Time Dog', f'Converted time: {time_str}', input_str)
        else:
            send_notification('Time Dog', 'Invalid timestamp format')

if __name__ == '__main__':
    main()
