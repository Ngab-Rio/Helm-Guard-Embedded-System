# HELM GUARD

## Embedded System Specification

**Version:** 0.1
**Platform:** ESP32
**Domain:** Embedded System / IoT
**Project:** Helm Guard — Smart Helmet Emergency System

---

# 1. Embedded System Overview

## 1.1 Purpose

Helm Guard adalah sistem embedded yang dipasang pada helm untuk membantu mendeteksi kondisi darurat saat digunakan berkendara.

Sistem memiliki dua mekanisme utama untuk mendeteksi keadaan darurat:

1. **Automatic Crash Detection** menggunakan MPU6050.
2. **Manual SOS Trigger** menggunakan tombol SOS.

Ketika kondisi darurat terdeteksi, ESP32 bertugas:

* menentukan jenis kejadian;
* memperoleh lokasi dari GPS;
* memberikan peringatan lokal menggunakan buzzer;
* membentuk data event;
* mengirimkan data ke backend melalui koneksi Internet.

## 1.2 Core Concept

```text
MPU6050  → WHAT HAPPENED?
GPS      → WHERE?
SOS      → USER NEEDS HELP
ESP32    → WHAT SHOULD THE SYSTEM DO?
Buzzer   → LOCAL FEEDBACK
API      → WHERE SHOULD THE DATA GO?
```

## 1.3 Main System Flow

```text
                    HELM GUARD DEVICE
                           │
          ┌────────────────┼────────────────┐
          │                │                │
          ▼                ▼                ▼
         GPS            MPU6050        SOS Button
          │                │                │
          └────────────────┼────────────────┘
                           ▼
                         ESP32
                           │
                ┌──────────┴──────────┐
                │                     │
                ▼                     ▼
           Local Alert           WiFi / Internet
                │                     │
                ▼                     ▼
             Buzzer                   API
                                      │
                                      ▼
                                   Backend
```

---

# 2. Hardware Architecture

ESP32 menjadi pusat kendali seluruh perangkat.

```text
                         ESP32
                           │
       ┌───────────────────┼───────────────────┐
       │                   │                   │
      UART                I2C                GPIO
       │                   │                   │
       ▼                   ▼             ┌─────┴─────┐
      GPS                MPU6050         SOS        Buzzer
```

## 2.1 Controller

ESP32 bertanggung jawab terhadap:

* membaca sensor;
* memproses data;
* menjalankan crash detection;
* membaca tombol SOS;
* mengendalikan buzzer;
* mengelola koneksi WiFi;
* membentuk JSON;
* mengirim data ke API;
* mengelola state perangkat.

## 2.2 Communication Interfaces

| Interface | Device      | Fungsi                              |
| --------- | ----------- | ----------------------------------- |
| UART      | GPS         | Menerima data NMEA                  |
| I2C       | MPU6050     | Membaca accelerometer dan gyroscope |
| GPIO      | SOS Button  | Input trigger manual                |
| GPIO      | Buzzer      | Output peringatan                   |
| WiFi      | Backend API | Komunikasi Internet                 |

---

# 3. Hardware Components

## 3.1 Current Hardware

| Component      | Role                     | Status              |
| -------------- | ------------------------ | ------------------- |
| ESP32          | Main controller          | Available           |
| GPS Module     | Location tracking        | Available           |
| MPU6050        | Motion / crash detection | Available           |
| SOS Button     | Manual emergency trigger | Planned             |
| Buzzer         | Local alert              | Planned / available |
| Battery System | Power source             | Planned             |

## 3.2 Optional Hardware

Komponen berikut dapat ditambahkan pada tahap selanjutnya:

* OLED display;
* LED indicator;
* battery voltage monitor;
* vibration motor;
* dedicated charging circuit;
* cellular/GSM/LTE module.

Komponen optional tidak menjadi bagian MVP sebelum kebutuhan dan hardware final ditentukan.

---

# 4. Pin & Interface Mapping

Pin mapping menjadi sumber konfigurasi hardware utama firmware.

## 4.1 GPS

Konfigurasi yang digunakan pada tahap awal:

| GPS | ESP32                  |
| --- | ---------------------- |
| TX  | GPIO 17                |
| RX  | GPIO 18                |
| GND | GND                    |
| VCC | Sesuai kebutuhan modul |

Interface:

```text
ESP32 UART2
Baud Rate: 9600
```

Data utama yang diterima:

```text
NMEA
```

## 4.2 MPU6050

Konfigurasi I2C:

| MPU6050 | ESP32                  |
| ------- | ---------------------- |
| SDA     | GPIO 21                |
| SCL     | GPIO 22                |
| GND     | GND                    |
| VCC     | Sesuai kebutuhan modul |

## 4.3 SOS Button

Pin akan ditentukan setelah pemasangan hardware final.

Konsep:

```text
Button
   │
   ▼
ESP32 GPIO
```

## 4.4 Buzzer

Pin akan ditentukan berdasarkan konfigurasi hardware final.

Konsep:

```text
ESP32 GPIO
   │
   ▼
Buzzer
```

---

# 5. Firmware Architecture

Firmware menggunakan pendekatan modular dan layered architecture.

```text
src/
├── main.cpp
│
├── app/
│   ├── app.cpp
│   ├── app.h
│   ├── state_machine.cpp
│   └── state_machine.h
│
├── drivers/
│   ├── gps/
│   ├── imu/
│   ├── button/
│   └── buzzer/
│
├── services/
│   ├── crash_detection/
│   └── emergency/
│
├── communication/
│   ├── wifi/
│   ├── json/
│   └── api/
│
└── system/
    ├── logger/
    └── time/
```

## 5.1 Layer

### Driver Layer

Berkomunikasi langsung dengan hardware.

```text
GPS
MPU6050
Button
Buzzer
```

### Service Layer

Mengolah data dan menjalankan logic sistem.

```text
Crash Detection
Emergency Manager
Location Service
```

### Communication Layer

Menangani komunikasi keluar perangkat.

```text
WiFi
JSON
HTTP API
```

### Application Layer

Mengatur keseluruhan perilaku perangkat.

```text
NORMAL
WARNING
EMERGENCY
OFFLINE
```

## 5.2 Main Entry Point

`main.cpp` diusahakan hanya menjadi entry point:

```cpp
void setup()
{
    appInit();
}

void loop()
{
    appUpdate();
}
```

Detail hardware dan business logic tidak ditempatkan langsung di `main.cpp`.

---

# 6. Sensor & Actuator Specification

## 6.1 Sensors / Inputs

### GPS

Menghasilkan:

* latitude;
* longitude;
* speed;
* altitude;
* timestamp;
* satellites;
* HDOP;
* fix status.

### MPU6050

Menghasilkan:

* accelerometer X/Y/Z;
* gyroscope X/Y/Z.

### SOS Button

Menghasilkan:

```text
PRESSED
RELEASED
```

## 6.2 Actuator

### Buzzer

Memberikan feedback kepada pengguna.

Contoh kondisi:

```text
NORMAL
    → OFF

GPS SEARCHING
    → Status pattern

CRASH CANDIDATE
    → Warning pattern

EMERGENCY
    → Emergency pattern
```

---

# 7. GPS Subsystem

## 7.1 Purpose

GPS digunakan untuk menentukan posisi perangkat.

GPS menjawab:

> Where is the device?

## 7.2 Data Flow

```text
GPS Module
    │
    │ NMEA
    ▼
ESP32 UART
    │
    ▼
TinyGPSPlus
    │
    ▼
Structured GPS Data
```

## 7.3 GPS Data

Data yang digunakan sistem:

```text
latitude
longitude
speed
altitude
timestamp
satellites
hdop
gps_fix
```

## 7.4 GPS Status

```text
GPS OFFLINE
     │
     ▼
NO DATA

GPS ONLINE
     │
     ▼
SEARCHING
     │
     ▼
VALID FIX
```

## 7.5 GPS Transmission Policy

| Mode      | Interval |
| --------- | -------: |
| NORMAL    | 30 detik |
| WARNING   |  5 detik |
| EMERGENCY |  2 detik |

Jika GPS belum mendapatkan fix:

```json
{
  "gps_fix": false
}
```

Sistem tidak mengirim koordinat palsu seperti:

```text
0,0
```

sebagai lokasi valid.

---

# 8. IMU / MPU6050 Subsystem

## 8.1 Purpose

MPU6050 digunakan untuk mendeteksi perubahan gerakan yang dapat mengindikasikan kecelakaan.

MPU6050 menjawab:

> What happened to the device?

## 8.2 Sensor Data

Accelerometer:

```text
Ax
Ay
Az
```

Gyroscope:

```text
Gx
Gy
Gz
```

## 8.3 Acceleration Magnitude

Sistem dapat menghitung magnitude:

```text
A = √(Ax² + Ay² + Az²)
```

Magnitude digunakan sebagai salah satu parameter dalam crash detection.

## 8.4 Gyroscope

Gyroscope digunakan untuk mendapatkan informasi mengenai perubahan rotasi perangkat.

Data gyro tidak digunakan sendirian untuk menentukan kecelakaan.

---

# 9. SOS Subsystem

SOS merupakan mekanisme emergency manual.

## 9.1 Flow

```text
SOS Button Pressed
        │
        ▼
ESP32
        │
        ▼
SOS Event
        │
        ▼
Emergency Manager
        │
        ▼
EMERGENCY State
```

## 9.2 Event Type

```text
SOS
```

SOS berbeda dari automatic crash detection:

```text
SOS Button
    ↓
SOS Event
```

sedangkan:

```text
MPU6050
    ↓
Crash Detection
    ↓
CRASH Event
```

Keduanya memiliki tujuan akhir yang sama:

```text
Emergency Event
```

---

# 10. Local Alert System

Buzzer digunakan sebagai indikator lokal sehingga pengguna tetap mendapatkan feedback tanpa bergantung pada aplikasi.

## 10.1 Alert States

```text
NORMAL
    → Buzzer OFF

WARNING
    → Warning pattern

EMERGENCY
    → Emergency pattern
```

## 10.2 Design Principle

Buzzer tidak menentukan kondisi sistem.

Buzzer hanya menerima perintah dari application/service layer.

```text
System State
     │
     ▼
Alert Manager
     │
     ▼
Buzzer Driver
```

---

# 11. Crash Detection System

Crash detection dilakukan oleh ESP32 berdasarkan data MPU6050.

## 11.1 Detection Concept

Sistem tidak menggunakan satu nilai sensor sebagai keputusan langsung.

```text
Accelerometer
      │
      ├──────────────┐
      │              │
Gyroscope            │
      │              │
      └──────┬───────┘
             ▼
       Data Analysis
             │
             ▼
      Crash Candidate
             │
             ▼
         Validation
             │
        ┌────┴────┐
        │         │
      FALSE      TRUE
        │         │
        ▼         ▼
      NORMAL   EMERGENCY
```

## 11.2 Detection Parameters

Parameter yang dapat digunakan:

* acceleration magnitude;
* gyro magnitude;
* perubahan orientasi;
* durasi kejadian;
* pola gerakan;
* konteks kecepatan GPS jika tersedia.

Threshold belum ditetapkan secara final.

Threshold akan ditentukan berdasarkan hasil pengujian data sensor nyata.

---

# 12. Device State Machine

State utama perangkat:

```text
NORMAL
WARNING
EMERGENCY
OFFLINE
```

## 12.1 State Diagram

```text
                  ┌─────────┐
                  │ NORMAL  │
                  └────┬────┘
                       │
              Impact Detected
                       │
                       ▼
             ┌─────────────────┐
             │ CRASH CANDIDATE │
             └────────┬────────┘
                      │
                  Validation
                 ┌────┴────┐
                 │         │
              Cancel     Valid
                 │         │
                 ▼         ▼
              NORMAL    EMERGENCY
                            ▲
                            │
                       SOS Button
```

GPS/WiFi failure dapat menghasilkan kondisi:

```text
OFFLINE
```

namun emergency state tetap memiliki prioritas lebih tinggi daripada kondisi konektivitas biasa.

---

# 13. Emergency Handling

Emergency dapat berasal dari:

```text
CRASH
SOS
```

## 13.1 Emergency Flow

```text
TRIGGER
   │
   ▼
EMERGENCY
   │
   ├── Obtain GPS
   │
   ├── Build Event
   │
   ├── Local Alert
   │
   ├── Send API
   │
   └── Continue Location Update
```

## 13.2 Emergency Event Lifecycle

```text
TRIGGERED
    ↓
SENT
    ↓
RECEIVED
    ↓
NOTIFIED
    ↓
ACKNOWLEDGED
```

Status tersebut dikelola oleh backend, sedangkan ESP32 bertanggung jawab terutama terhadap tahap trigger dan pengiriman data.

---

# 14. Data Processing

Data mentah dari sensor tidak langsung dikirim ke backend.

## 14.1 GPS

```text
Raw NMEA
   ↓
Parser
   ↓
Structured GPS Data
   ↓
JSON
```

## 14.2 MPU6050

```text
Raw Sensor Data
   ↓
Processing
   ↓
Acceleration / Gyro Parameters
   ↓
Crash Detection
```

## 14.3 Principle

```text
Raw Data
   ↓
Processing
   ↓
Meaningful Data
   ↓
Communication
```

ESP32 menjadi tempat utama untuk melakukan preprocessing dan keputusan real-time.

---

# 15. IoT Communication

Komunikasi awal menggunakan:

```text
ESP32
  │
  ▼
WiFi
  │
  ▼
Internet
  │
  ▼
HTTPS
  │
  ▼
Backend API
```

## 15.1 Communication Responsibility

ESP32 bertanggung jawab terhadap:

* koneksi WiFi;
* connection retry;
* pembuatan payload;
* HTTP request;
* response handling;
* error handling.

Backend bertanggung jawab terhadap:

* authentication;
* validation;
* business logic;
* database;
* notification;
* event management.

---

# 16. Device → API Contract

JSON digunakan sebagai format komunikasi antara ESP32 dan API.

## 16.1 Location Payload

```json
{
  "device_id": "HG-001",
  "gps_fix": true,
  "latitude": -7.123456,
  "longitude": 110.123456,
  "speed": 42.5,
  "altitude": 450.2,
  "satellites": 9,
  "hdop": 1.2
}
```

## 16.2 Emergency Payload

```json
{
  "device_id": "HG-001",
  "event_type": "CRASH",
  "timestamp": "2026-10-07T15:00:00Z",
  "latitude": -7.123456,
  "longitude": 110.123456,
  "speed": 42.5,
  "satellites": 9
}
```

## 16.3 Communication Architecture

```text
ESP32
  │
  │ JSON
  ▼
API
  │
  ├── Authenticate
  ├── Validate
  ├── Process
  └── Store
        │
        ▼
    Database
```

ESP32 tidak melakukan koneksi langsung ke database.

---

# 17. Power Management

Power management diperlukan karena perangkat akan digunakan sebagai perangkat mobile.

## 17.1 Power Components

```text
Battery
   │
   ▼
Power System
   │
   ▼
ESP32
   ├── GPS
   ├── MPU6050
   ├── Buzzer
   └── Communication
```

## 17.2 Future Considerations

* battery level monitoring;
* low battery warning;
* charging status;
* power optimization;
* sleep mode;
* sensor duty cycle;
* WiFi power consumption.

Power architecture final ditentukan setelah jenis baterai dan charging circuit ditetapkan.

---

# 18. Error Handling & Fail-Safe

Karena Helm Guard merupakan sistem emergency, kegagalan komponen harus ditangani secara eksplisit.

## 18.1 GPS Failure

```text
GPS unavailable
     ↓
gps_fix = false
     ↓
Continue system operation
```

Emergency tetap dapat dipicu meskipun GPS belum mendapatkan fix.

## 18.2 WiFi Failure

```text
WiFi disconnected
      ↓
Reconnect
      ↓
Retry
```

Data emergency yang gagal dikirim dapat dirancang untuk masuk ke queue lokal pada tahap pengembangan berikutnya.

## 18.3 API Failure

```text
HTTP Request
     ↓
Failed
     ↓
Retry
```

Jumlah retry dan strategi queue akan ditentukan pada implementasi communication layer.

## 18.4 Sensor Failure

Firmware harus dapat mengetahui kondisi sensor yang tidak tersedia atau tidak merespons.

Contoh:

```text
MPU6050 unavailable
       ↓
Crash detection unavailable
       ↓
System status degraded
```

Sistem tidak boleh menganggap sensor yang gagal sebagai sensor normal.

---

# 19. Firmware Testing

Testing dilakukan secara bertahap.

## 19.1 Driver Test

```text
GPS
 ├── UART test
 ├── NMEA test
 └── Fix test

MPU6050
 ├── I2C detection
 ├── Accelerometer test
 └── Gyroscope test

Button
 └── Press / release test

Buzzer
 └── Pattern test
```

## 19.2 Integration Test

```text
GPS + ESP32
MPU6050 + ESP32
SOS + ESP32
Buzzer + ESP32
WiFi + ESP32
ESP32 + API
```

## 19.3 System Test

Skenario utama:

### Scenario 1 — Normal

```text
Device ON
    ↓
Sensors active
    ↓
GPS available
    ↓
NORMAL
```

### Scenario 2 — GPS Searching

```text
GPS data received
    ↓
No valid fix
    ↓
System continues
```

### Scenario 3 — SOS

```text
SOS pressed
    ↓
Emergency
    ↓
GPS location
    ↓
JSON
    ↓
API
```

### Scenario 4 — Crash

```text
Impact
   ↓
Crash Candidate
   ↓
Validation
   ↓
Emergency
   ↓
API
```

### Scenario 5 — Network Failure

```text
Emergency
   ↓
WiFi unavailable
   ↓
Retry / queue strategy
```

---

# 20. Development Roadmap

Pengembangan embedded system dilakukan secara bertahap agar setiap bagian dapat diuji sebelum digabungkan.

## Phase 1 — GPS

**Status: COMPLETED**

* UART communication
* NMEA reception
* TinyGPSPlus parsing
* GPS fix detection
* latitude / longitude
* satellite count
* altitude
* speed
* GPS status indicator

---

## Phase 2 — Firmware Modularization

**Status: CURRENT**

Target:

```text
src/
├── main.cpp
└── drivers/
    └── gps/
        ├── gps.cpp
        └── gps.h
```

Tujuan:

* memisahkan GPS driver;
* membuat `main.cpp` sederhana;
* membangun pola modular untuk sensor berikutnya.

---

## Phase 3 — MPU6050

Target:

* I2C communication;
* sensor detection;
* accelerometer reading;
* gyroscope reading;
* serial data logging;
* memahami karakteristik gerakan perangkat.

Belum menentukan threshold crash sebelum data nyata tersedia.

---

## Phase 4 — Crash Detection

Target:

* acceleration magnitude;
* gyro magnitude;
* orientation change;
* temporal analysis;
* crash candidate;
* validation;
* false-positive reduction.

---

## Phase 5 — Local Emergency

Target:

* SOS button;
* buzzer;
* emergency state;
* local feedback;
* emergency manager.

---

## Phase 6 — WiFi Communication

Target:

* WiFi initialization;
* connection monitoring;
* reconnect;
* connection status.

---

## Phase 7 — JSON Communication

Target:

```text
Sensor Data
    ↓
Structured Data
    ↓
JSON Payload
```

---

## Phase 8 — API Integration

Target:

```text
ESP32
 ↓
HTTPS
 ↓
API
```

Meliputi:

* authentication;
* location endpoint;
* emergency endpoint;
* HTTP response handling;
* retry.

---

## Phase 9 — Full Emergency System

Menggabungkan:

```text
MPU6050
    +
SOS
    +
GPS
    +
Buzzer
    +
WiFi
    +
API
```

Menjadi satu emergency flow.

---

## Phase 10 — System Testing & Optimization

Meliputi:

* crash detection testing;
* GPS reliability;
* WiFi reliability;
* API reliability;
* power consumption;
* false positive analysis;
* recovery after failure;
* long-duration testing.

---

# Embedded System Design Principles

Helm Guard mengikuti beberapa prinsip utama:

### 1. Separation of Concerns

```text
Driver
  ↓
Service
  ↓
Application
  ↓
Communication
```

### 2. Hardware Independence

Logic crash detection tidak boleh bergantung langsung pada implementasi register sensor.

### 3. Non-Blocking Firmware

Firmware menggunakan pendekatan asynchronous/non-blocking dengan `millis()` dan state machine daripada `delay()` untuk proses utama.

### 4. Fail-Safe

Kegagalan satu komponen tidak boleh menyebabkan seluruh sistem berhenti tanpa status yang jelas.

### 5. Data Validation

Data sensor harus diproses dan divalidasi sebelum dikirim keluar perangkat.

### 6. API Boundary

ESP32 tidak mengakses database secara langsung.

```text
ESP32 → API → Backend → Database
```

### 7. Incremental Development

Setiap subsystem harus diuji secara independen sebelum digabungkan menjadi sistem penuh.

---
