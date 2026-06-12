## Sistem Monitoring Kenyamanan Ruang Belajar Berbasis IoT

Project ini merupakan implementasi Internet of Things (IoT) untuk memonitor tingkat kenyamanan ruang belajar secara real-time menggunakan ESP32 dan beberapa sensor lingkungan. Sistem dirancang untuk membantu pengguna memahami kondisi ruangan berdasarkan parameter suhu, kelembapan, intensitas cahaya, kualitas udara, dan tingkat kebisingan. Dengan adanya monitoring ini, pengguna dapat mengetahui apakah lingkungan belajar berada dalam kondisi nyaman atau tidak nyaman.

Dalam pengembangannya, sistem mengintegrasikan sensor DHT22 untuk membaca suhu dan kelembapan, BH1750 untuk mengukur intensitas cahaya, MQ-135 untuk mendeteksi kualitas udara, serta KY-037 untuk memantau tingkat kebisingan. Seluruh data sensor diproses oleh ESP32 sebagai mikrokontroler utama yang memiliki kemampuan konektivitas WiFi sehingga data dapat dikirim dan divisualisasikan melalui dashboard Blynk secara real-time.

Selain menampilkan hasil monitoring pada dashboard, sistem juga menggunakan indikator LED sebagai penanda kondisi ruangan. LED hijau akan menyala ketika kondisi lingkungan berada dalam kategori nyaman, sedangkan LED merah akan menyala apabila kondisi lingkungan tidak memenuhi parameter kenyamanan yang telah ditentukan. Penentuan kenyamanan dilakukan berdasarkan beberapa parameter, seperti suhu 22–27°C, kelembapan 40–60% RH, dan intensitas cahaya minimal 300 lux.

Project ini juga memperhatikan perbedaan sampling rate antar sensor. Untuk mengatasi perbedaan frekuensi pembacaan data, sistem menggunakan metode polling dengan interval tetap agar seluruh data dapat dianalisis secara sinkron dan konsisten. Pendekatan ini memungkinkan sistem memberikan hasil monitoring yang lebih stabil dan akurat.

Melalui project ini, konsep IoT berhasil diterapkan dalam sistem monitoring lingkungan belajar berbasis multi-sensor. Sistem tidak hanya mampu melakukan pemantauan kondisi ruangan secara komprehensif, tetapi juga memberikan visualisasi data secara interaktif sehingga dapat membantu menciptakan lingkungan belajar yang lebih nyaman dan optimal.

## Components
- ESP32
- DHT22 Sensor
- BH1750 Sensor
- MQ-135 Sensor
- KY-037 Sensor
- LED Indicator
- Blynk Dashboard

## Authors
Kelompok 9  
Program Studi Sains Data  
UPN “Veteran” Jawa Timur
