# 🕵️‍♂️ HTTP Sniffer (C++ Version)

HTTP Sniffer ตัวนี้ถูกพัฒนาด้วยภาษา C++ ใช้ `libpcap` ในการดักจับ packet บนระบบเครือข่าย โดยสามารถดัก HTTP request ได้แบบ real-time พร้อม log IP address, hostname และรายละเอียด request ลงไฟล์ log ได้อย่างมีประสิทธิภาพ

## 📁 โครงสร้างโปรเจกต์
```
http-sniffer/
├── include/
│   └── utils.h              # ฟังก์ชันช่วยต่างๆ (เช่น แปลง IP → hostname)
├── src/
│   ├── main.cpp             # จุดเริ่มต้นของโปรแกรม (main function)
│   └── utils.cpp            # นิยามของฟังก์ชันช่วยเหลือ
├── http_sniffer.log         # ไฟล์ log ที่โปรแกรมเขียนเข้าไป
├── Makefile                 # สคริปต์สำหรับ build โปรเจกต์
└── README.md                # คู่มือใช้งาน
```

## ⚙️ วิธี Build

### 🧱 ข้อกำหนด
- ระบบ: Linux-based (Debian, Ubuntu, CentOS, etc.)
- ต้องติดตั้ง `libpcap` และ g++
```bash
sudo apt update
sudo apt install build-essential libpcap-dev -y
```

### 🔨 Build ด้วย Makefile
```bash
make
```
จะได้ไฟล์ `sniffer` ที่สามารถรันได้ทันที

## 🚀 วิธีใช้งาน
```bash
sudo ./sniffer
```
- จำเป็นต้องใช้ `sudo` เพื่อให้เข้าถึง network interface ได้
- โปรแกรมจะเลือก interface ตัวแรกที่พบโดยอัตโนมัติ
- ข้อมูล HTTP request จะถูก log ลงในไฟล์ `http_sniffer.log`

## 🧪 การทดสอบ

### 1. รัน Sniffer
```bash
sudo ./sniffer
```

### 2. รัน HTTP Server ทดสอบ (บน port 80)
```bash
cd /tmp
sudo python3 -m http.server 80
```

### 3. ส่ง HTTP Request ไปยังเซิร์ฟเวอร์ทดสอบ
```bash
curl http://127.0.0.1/
```

### 4. ตรวจสอบ Log
```bash
cat http_sniffer.log
```

## 📝 ตัวอย่าง Log
```text
=== 2025-05-04 22:10:12 ===
From: 127.0.0.1 (localhost):51234
To  : 127.0.0.1 (localhost):80
GET / HTTP/1.1
Host: 127.0.0.1
User-Agent: curl/7.81.0
...
```

## 🧠 ฟีเจอร์
- ✅ ดักจับเฉพาะ HTTP Request (GET, POST, HEAD)
- ✅ แสดงทั้ง IP และ Hostname
- ✅ รองรับ real-time logging
- ✅ เขียนด้วย C++ แยกโมดูลชัดเจน พัฒนา/ปรับปรุงง่าย
- ✅ ประสิทธิภาพสูง เหมาะกับงานระดับ production

## 📎 หมายเหตุ
- ไม่สามารถอ่านเนื้อหา HTTPS ได้ (เพราะถูกเข้ารหัส)
- หากมีหลาย interface คุณสามารถปรับให้เลือก interface ด้วยตนเองได้ใน `main.cpp`

## 👨‍💻 ผู้พัฒนา
- 💼 โดยคุณ owariz
- 📧 ติดต่อ: [support@fulltechx.com]
- 🌐 GitHub: [https://github.com/FullTechX/]
