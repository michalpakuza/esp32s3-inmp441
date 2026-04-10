#include <WiFi.h>
#include <WiFiUdp.h>
#include <driver/i2s.h>
#include <Adafruit_NeoPixel.h>


#define NUMPIXELS      1
#define RGB_PIN 48

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);


const char* ssid = "xxx";
const char* password = "xxx";
const char* udpAddress = "192.168.100.100";
const int udpPort = 4444;
int32_t offset = 0;


#define I2S_WS 17
#define I2S_SD 47
#define I2S_SCK 16


WiFiUDP udp;
int32_t audioBuffer32[320];
int16_t audioBuffer16[320];

unsigned long ledTime = 0;
bool ledOn = false;

void setup() {
  Serial.begin(115200);


  pixels.begin();
  pixels.setBrightness(30);
  pixels.clear();
  pixels.show();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 16,
    .dma_buf_len = 512,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };
  i2s_pin_config_t pin_config = { .bck_io_num = I2S_SCK, .ws_io_num = I2S_WS, .data_out_num = I2S_PIN_NO_CHANGE, .data_in_num = I2S_SD };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_clk(I2S_NUM_0, 16000, I2S_BITS_PER_SAMPLE_32BIT, I2S_CHANNEL_MONO);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_start(I2S_NUM_0);

  udp.begin(udpPort);
  Serial.println("\nReady! Say 'Klara'...");
}

void loop() {
  size_t bytesRead;

  i2s_read(I2S_NUM_0, audioBuffer32, sizeof(audioBuffer32), &bytesRead, portMAX_DELAY);

  int samples = bytesRead / 4;

  for (int i = 0; i < samples; i++) {
    int32_t sample = audioBuffer32[i];

    // DC offset
    sample &= 0xFFFFFFF0;
    
    // conversion 32 → 16 bit
    sample >>= 15;

    // gain
    sample *= 3;

    // clamp
    if (sample > 32767) sample = 32767;
    if (sample < -32768) sample = -32768;

    audioBuffer16[i] = (int16_t)sample;
  }

  int32_t rms = 0;
  for (int i = 0; i < samples; i++) {
    rms += abs(audioBuffer16[i]);
  }
  rms /= samples;


  if (rms > 300) { 
    udp.beginPacket(udpAddress, udpPort);
    udp.write((uint8_t*)audioBuffer16, samples * sizeof(int16_t));
    udp.endPacket();
}


  int packetSize = udp.parsePacket();
  if (packetSize > 0) {
    char cmd = udp.read();

    switch (cmd) {
      case 'L': pixels.setPixelColor(0, pixels.Color(0,255,0)); break;
      case 'R': pixels.setPixelColor(0, pixels.Color(255,0,0)); break;
      case 'B': pixels.setPixelColor(0, pixels.Color(0,0,255)); break;
      case 'V': pixels.setPixelColor(0, pixels.Color(128,0,128)); break;
      case 'Y': pixels.setPixelColor(0, pixels.Color(255,255,0)); break;
      case 'X': pixels.setPixelColor(0, pixels.Color(245,245,220)); break;
      case 'W': pixels.setPixelColor(0, pixels.Color(255,255,255)); break;
      case 'F': pixels.setPixelColor(0, pixels.Color(150,75,0)); break;
    }

    pixels.show();
    ledTime = millis();
    ledOn = true;
  }


  if (ledOn && millis() - ledTime > 2000) {
    pixels.clear();
    pixels.show();
    ledOn = false;
  }
}