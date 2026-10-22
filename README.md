## Helm Guard

Firmware PlatformIO untuk ESP32 Dev Module. Data GPS dikirim ke API setiap 30
detik saat perangkat memiliki GPS fix dan koneksi Wi-Fi.

### Setup konfigurasi lokal

1. Salin `include/config/secrets.h.example` menjadi
	`include/config/secrets.h`.
2. Isi SSID, password Wi-Fi, URL API, dan ID perangkat.
3. Jangan commit `include/config/secrets.h`; file tersebut sudah masuk
	`.gitignore`.
4. Build dengan `pio run`, lalu upload dengan `pio run --target upload`.

Tanpa `secrets.h`, firmware tetap dapat dikompilasi tetapi tidak akan mencoba
terhubung ke Wi-Fi.

### Struktur utama

- `src/main.cpp`: lifecycle firmware dan interval pengiriman.
- `src/drivers/gps`: pembacaan GPS melalui UART2.
- `src/communications/wifi`: koneksi dan retry Wi-Fi.
- `src/communications/json`: serialisasi payload GPS.
- `src/communications/api`: pengiriman HTTP ke server.
- `include/config`: pin dan konfigurasi lokal.
# Helm-Guard-Embedded-System
