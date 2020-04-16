## ESP Fire Effect

![M5Stack fullscreen fire](https://appelsiini.net/img/m5stack-fire1-1400.jpg)

Old school fire effect for ESP32 based boards. Based on Lode's classic [fire tutorial](http://lodev.org/cgtutor/fire.html). Ready made config files for M5Stack, TTGO T-Display and TGO T4 V13. For example to compile and flash for M5Stack run the following.

```
$ git clone git@github.com:tuupola/esp_fire.git --recursive
$ cd esp_fire
$ cp sdkconfig.m5stack sdkconfig
$ make -j8 flash
```

If you have some other board run menuconfig yourself.

```
$ git clone git@github.com:tuupola/esp_fire.git --recursive
$ cd esp_fire
$ make menuconfig
$ make -j8 flash
```

