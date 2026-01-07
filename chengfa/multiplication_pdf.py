#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import random
from reportlab.lib.pagesizes import A4
from reportlab.platypus import SimpleDocTemplate, Paragraph, Table, TableStyle, KeepTogether
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.lib.units import mm
from reportlab.lib import colors
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont

# 注册系统中可用的中文字体
pdfmetrics.registerFont(TTFont('ArialUnicode', '/Library/Fonts/Arial Unicode.ttf'))


def generate_multiplication():
    """生成三位数乘以两位数的乘法算式"""
    # 生成三位数的第一个数
    num1 = random.randint(100, 999)
    # 生成两位数的第二个数
    num2 = random.randint(10, 99)
    return f"{num1} × {num2} = "


def create_pdf(output_path):
    """创建A4大小的PDF，每行显示两个乘法算式，按中线分开，每两行用横线隔开，上方添加日期"""
    # 设置A4纸张大小
    doc = SimpleDocTemplate(output_path, pagesize=A4, topMargin=20*mm, bottomMargin=20*mm, leftMargin=20*mm, rightMargin=20*mm)
    styles = getSampleStyleSheet()
    content = []
    
    # 获取页面宽度和可用宽度（减去边距）
    page_width, page_height = A4
    available_width = page_width - 20*mm*2
    
    # 设置每个算式的样式
    style = styles['Normal']
    style.fontName = 'ArialUnicode'
    style.fontSize = 14
    style.leading = 20
    
    # 设置日期样式
    date_style = styles['Normal']
    date_style.fontName = 'ArialUnicode'
    date_style.fontSize = 12
    date_style.leading = 16
    date_style.alignment = 0  # 左对齐
    
    # 生成多行乘法算式，每两行一组，包含日期和横线
    for group in range(16):  # 生成16组，每组包含日期+2行算式+横线，确保只生成一页
        # 每个组的内容列表
        group_content = []
        
        # 添加日期
        date_text = "日期：<u>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</u>"
        date_para = Paragraph(date_text, date_style)
        group_content.append(date_para)
        
        # 生成两行乘法算式
        for _ in range(2):
            # 生成两个乘法算式
            expr1 = generate_multiplication()
            expr2 = generate_multiplication()
            
            # 创建两个段落
            p1 = Paragraph(expr1, style)
            p2 = Paragraph(expr2, style)
            
            # 创建表格，一行两列，按中线分开
            data = [[p1, p2]]
            table = Table(data, colWidths=[available_width/2, available_width/2])
            
            # 设置表格样式，无边框，单元格居中对齐
            table.setStyle(TableStyle([
                ('ALIGN', (0, 0), (0, 0), 'LEFT'),  # 左侧算式左对齐
                ('ALIGN', (1, 0), (1, 0), 'LEFT'),  # 右侧算式左对齐
                ('VALIGN', (0, 0), (1, 0), 'TOP'),
                ('BOTTOMPADDING', (0, 0), (1, 0), 3*mm),  # 极小的底部边距，减小行间距
                ('LEFTPADDING', (0, 0), (1, 0), 0),
                ('RIGHTPADDING', (0, 0), (1, 0), 0),
                ('TOPPADDING', (0, 0), (1, 0), 0),
            ]))
            
            group_content.append(table)
        
        # 添加横线
        line_data = [[Paragraph("", style), Paragraph("", style)]]
        line_table = Table(line_data, colWidths=[available_width/2, available_width/2])
        line_table.setStyle(TableStyle([
            ('LINEBELOW', (0, 0), (1, 0), 1, colors.black),
            ('BOTTOMPADDING', (0, 0), (1, 0), 5*mm),
        ]))
        group_content.append(line_table)
        
        # 将整个组包裹在KeepTogether中，确保在同一页显示
        content.append(KeepTogether(group_content))
    
    # 构建PDF
    doc.build(content)


if __name__ == "__main__":
    output_file = "multiplication_exercises.pdf"
    create_pdf(output_file)
    print(f"乘法算式PDF已生成：{output_file}")
