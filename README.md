# Real-Time Environmental Monitoring System
**Versi:** ESP32 IoT  
**Lisensi:** MIT License

Sistem monitoring lingkungan real-time berbasis ESP32 dengan dashboard web interaktif

**Demo** • **Fitur** • **Instalasi** • **Penggunaan** • **Hardware** • **Troubleshooting**

## Deskripsi Proyek

Real-Time Environmental Monitoring System adalah sistem IoT yang dirancang untuk memantau parameter lingkungan seperti suhu, kelembaban, getaran, dan intensitas cahaya secara real-time. Sistem ini menggunakan mikrokontroler ESP32 yang terhubung ke berbagai sensor dan menampilkan data melalui dashboard web yang dapat diakses dari browser mana pun.

## Tujuan

- Memantau kondisi lingkungan secara real-time
- Memberikan visualisasi data yang intuitif
- Sistem yang mudah diimplementasikan dan scalable
- Antarmuka yang responsive untuk berbagai perangkat

## Fitur

### Monitoring Real-Time
- **Suhu**: Monitoring suhu ambient dengan sensor DHT11
- **Kelembaban**: Pengukuran kelembaban udara relatif
- **Getaran**: Deteksi getaran atau vibrasi lingkungan
- **Cahaya**: Monitoring intensitas cahaya dengan LDR

### Dashboard Web
- **Grafik Real-Time**: Visualisasi data dengan Chart.js
- **Responsive Design**: Tampilan optimal di desktop dan mobile
- **Auto-Refresh**: Update data otomatis setiap 2 detik
- **User-Friendly**: Antarmuka intuitif dan mudah digunakan

### Teknologi
- **ESP32**: Mikrokontroler dengan WiFi built-in
- **Web Server**: HTTP server embedded pada ESP32
- **REST API**: Endpoint JSON untuk data sensor
- **Chart.js**: Visualisasi data real-time

## Persyaratan Hardware

### Komponen yang Dibutuhkan

| Komponen | Jumlah | Keterangan |
|----------|--------|------------|
| ESP32 Development Board | 1 | Mikrokontroler utama |
| Sensor DHT11 | 1 | Suhu & Kelembaban |
| Sensor Getar | 1 | Deteksi vibrasi |
| LDR (Light Dependent Resistor) | 1 | Sensor cahaya |
| Resistor 10kΩ | 1 | Pull-up untuk LDR |
| Breadboard | 1 | Tempat merangkai |
| Kabel Jumper | Secukupnya | Koneksi antar komponen |

### Diagram Rangkaian

**ESP32 Pinout:**
```
├── GPIO 4 → DATA DHT11
├── GPIO 2 → OUT Sensor Getar  
├── GPIO 34 → LDR (dengan resistor 10kΩ ke GND)
├── 3.3V → VCC semua sensor
└── GND → Ground semua sensor
```

**Skema Koneksi:**

**DHT11:**
```
VCC   → 3.3V
GND   → GND
DATA  → GPIO 4
```

**Sensor Getar:**
```
VCC   → 3.3V
GND   → GND
OUT   → GPIO 2
```

**LDR:**
```
Kaki 1 → 3.3V
Kaki 2 → GPIO 34 + Resistor 10kΩ → GND
```

## Persyaratan Software

### Software Development
- Arduino IDE (versi 1.8.x atau lebih baru)
- ESP32 Board Package

### Library yang Diperlukan
Install melalui Arduino IDE Library Manager:

1. WiFi (bawaan Arduino)
2. WebServer (bawaan ESP32) 
3. DHT Sensor Library oleh Adafruit
4. ArduinoJson oleh Benoit Blanchon

## Instalasi

### 1. Persiapan Development Environment
**Install ESP32 Board Package:**

1. Buka Arduino IDE
2. File → Preferences
3. Tambahkan URL berikut di "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Tools → Board → Board Manager → cari "ESP32" → Install

### 2. Install Library
**Melalui Library Manager:**

1. Sketch → Include Library → Manage Libraries
2. Cari dan install library berikut:
   - "DHT sensor library" oleh Adafruit
   - "ArduinoJson" oleh Benoit Blanchon

### 3. Konfigurasi Kode
Edit konfigurasi WiFi:

```cpp
// Pada file utama, ubah bagian berikut:
const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";
```

### 4. Upload Kode
1. Pilih board: ESP32 Dev Module
2. Pilih port COM yang sesuai
3. Upload kode ke ESP32
4. Buka Serial Monitor (115200 baud) untuk monitoring

## Penggunaan

### 1. Inisialisasi Sistem
1. Hubungkan semua komponen sesuai diagram rangkaian
2. Power on ESP32 (via USB atau external power)
3. Buka Serial Monitor untuk melihat status koneksi

### 2. Akses Dashboard
1. Dari Serial Monitor, catat IP address yang ditampilkan
2. Buka browser web (Chrome, Firefox, dll.)
3. Akses alamat: `http://[IP_ADDRESS_ESP32]`
   Contoh: `http://192.168.1.100`

### 3. Monitoring Data
- **Data Real-Time**: Nilai sensor terupdate otomatis setiap 2 detik
- **Grafik Trend**: Visualisasi data historis 20 titik terakhir
- **Status Sistem**: Indikator online/offline

## Spesifikasi Teknis

### Spesifikasi Sensor

| Sensor | Range | Akurasi | Update Interval |
|--------|-------|---------|-----------------|
| DHT11 (Suhu) | 0-50°C | ±2°C | 2 detik |
| DHT11 (Kelembaban) | 20-90% RH | ±5% | 2 detik |
| Sensor Getar | Digital (0/1) | - | Real-time |
| LDR | 0-4095 | - | Real-time |

### Spesifikasi Sistem

| Parameter | Spesifikasi |
|-----------|-------------|
| Microcontroller | ESP32-WROOM-32 |
| WiFi | 802.11 b/g/n (2.4 GHz) |
| Power Supply | 5V via USB atau 3.3V external |
| Current Consumption | ~80mA (normal operation) |
| Web Server | HTTP port 80 |
| Data Update | 2000ms interval |

## Struktur Project

```
real-time-environment-monitoring/
│
├── src/
│   └── environmental_monitoring.ino  # Kode utama
│
├── docs/
│   ├── schematic.png                 # Diagram rangkaian
│   └── pinout.md                    # Dokumentasi pin
│
├── libraries/                        # Library dependencies
│
└── README.md                        # Dokumentasi ini
```

## API Endpoints

Sistem menyediakan endpoint berikut:

### GET /
- **Deskripsi**: Dashboard web utama
- **Response**: HTML page

### GET /sensor
- **Deskripsi**: Data sensor dalam format JSON
- **Response**:
```json
{
  "temperature": 25.5,
  "humidity": 60.2,
  "vibration": 0,
  "light": 2048
}
```

### GET /data
- **Deskripsi**: Data sensor format JSON (legacy)
- **Response**: JSON string

## Troubleshooting

### Masalah Umum dan Solusi

| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| ESP32 tidak terkoneksi WiFi | SSID/password salah | Periksa kredensial WiFi |
| Sensor DHT gagal membaca | Koneksi longgar | Periksa kabel data DHT11 |
| Dashboard tidak bisa diakses | IP address berubah | Cek Serial Monitor untuk IP baru |
| Data tidak update | Koneksi jaringan | Restart ESP32 |
| Sensor cahaya tidak akurat | Kalibrasi diperlukan | Adjust nilai mapping LDR |

### Langkah Debugging
1. Buka Serial Monitor (115200 baud)
2. Periksa pesan status koneksi WiFi
3. Verifikasi IP address yang ditampilkan
4. Test koneksi dengan ping ke IP ESP32
5. Periksa kabel dan koneksi hardware

## Pengembangan dan Peningkatan

### Potensi Pengembangan
- **Database Storage**: Integrasi dengan MySQL/InfluxDB
- **Mobile App**: Aplikasi Android/iOS companion
- **Alert System**: Notifikasi via Telegram/Email
- **Multi-Node**: Monitoring beberapa lokasi
- **Data Export**: Export data ke CSV/Excel
- **OTA Updates**: Update firmware wireless

### Sensor Tambahan
- Sensor kualitas udara (MQ-135)
- Sensor tekanan (BMP180)
- Sensor suara (Sound Sensor)
- Sensor hujan (Rain Sensor)

## Kontribusi

Kontribusi dipersilakan! Untuk berkontribusi:

1. Fork project ini
2. Buat feature branch (`git checkout -b feature/FiturAnda`)
3. Commit perubahan (`git commit -m 'Tambahkan FiturAnda'`)
4. Push ke branch (`git push origin feature/FiturAnda`)
5. Buat Pull Request

## Lisensi

Didistribusikan di bawah Lisensi MIT. Lihat LICENSE untuk informasi lebih lanjut.

## Developer

**Tim Pengembang**
- Lead Developer & IoT Specialist

## Tautan Berguna

- [ESP32 Official Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [Arduino IDE Download](https://www.arduino.cc/en/software)
- [DHT11 Sensor Datasheet](https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)
- [Chart.js Documentation](https://www.chartjs.org/docs/latest/)

## Penafian

Project ini ditujukan untuk tujuan edukasi dan prototyping. Untuk implementasi production, pertimbangkan aspek keamanan dan reliability tambahan.
