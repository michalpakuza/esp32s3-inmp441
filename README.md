# ESP32-S3 Voice Streamer with RGB Control

## Project Description

A real-time audio streaming system from ESP32-S3 to UDP network with remote RGB LED control. The device listens for network commands to change the LED color while simultaneously streaming audio to a specified receiver.

## Features

- **Audio capture** via I2S microphone (16 kHz, 32-bit)
- **Signal processing**: DC offset removal, 32-bit → 16-bit conversion, gain adjustment
- **UDP transmission** – audio is sent only when volume exceeds a threshold (RMS detection)
- **Remote RGB control** – NeoPixel LED (GPIO 48) changes color based on UDP commands
- **Automatic LED timeout** – turns off after 2 seconds of inactivity

## UDP Control Commands

| Command | Color     |
|---------|-----------|
| L       | Green     |
| R       | Red       |
| B       | Blue      |
| V       | Purple    |
| Y       | Yellow    |
| X       | Beige     |
| W       | White     |
| F       | Brown     |

## Hardware Requirements

- **ESP32-S3** board (or compatible)
- **I2S microphone** (e.g., INMP441, ICS-43434)
- **Wi-Fi 2.4 GHz** connection

## Pinout

| Component  | ESP32 Pin |
|------------|-----------|
| I2S WS     | 17        |
| I2S SD     | 47        |
| I2S SCK    | 16        |
| NeoPixel   | 48        |

## Configuration

1. **Change Wi-Fi credentials** in the code:
   const char* ssid = "your_wifi";
   const char* password = "your_password";

2. const char* udpAddress = "192.168.100.100"; // change to your server IP
   const int udpPort = 4444;

3. if (rms > 300) { // increase for less sensitivity, decrease for more





# ESP32-S3 Voice Streamer with RGB Control

## Opis projektu

System do strumieniowania dźwięku w czasie rzeczywistym z ESP32-S3 do sieci UDP oraz zdalnego sterowania diodą RGB. Urządzenie nasłuchuje komend sieciowych, które zmieniają kolor podświetlenia, jednocześnie przesyłając strumień audio do wskazanego odbiornika.

## Funkcjonalności

- **Przechwytywanie dźwięku** przez mikrofon podłączony do interfejsu I2S (16 kHz, 32-bit)
- **Przetwarzanie sygnału**: redukcja offsetu DC, konwersja 32-bit → 16-bit, regulacja wzmocnienia
- **Przesyłanie UDP** – dźwięk wysyłany jest tylko gdy poziom głośności przekroczy próg (detekcja RMS)
- **Zdalne sterowanie RGB** – dioda NeoPixel (GPIO 48) zmienia kolor na podstawie komend UDP
- **Automatyczne wygaszanie** diody po 2 sekundach bez aktywności

## Komendy sterujące (UDP)

| Komenda | Kolor |
|---------|-------|
| L | Zielony |
| R | Czerwony |
| B | Niebieski |
| V | Fioletowy |
| Y | Żółty |
| X | Beżowy |
| W | Biały |
| F | Brązowy |

## Wymagania sprzętowe

- Płytka **ESP32-S3** (lub kompatybilna)
- Mikrofon **I2S** (np. INMP441, ICS-43434)
- Połączenie Wi-Fi 2.4 GHz

## Pinout

| Komponent | Pin ESP32 |
|-----------|-----------|
| I2S WS    | 17        |
| I2S SD    | 47        |
| I2S SCK   | 16        |
| NeoPixel  | 48        |

## Konfiguracja

1. **Zmień dane Wi-Fi** w kodzie:
   
   const char* ssid = "twoja_siec";
   const char* password = "twoje_haslo";

2. const char* udpAddress = "192.168.100.100"; #zmień na swój adres serwera
   const int udpPort = 4444;

3. if (rms > 300) { // zwiększ dla mniejszej czułości

