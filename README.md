## ESP Fire Effect

![ESP32 fullscreen fire](https://appelsiini.net/img/2020/esp-fire.jpg)

[Copepod](https://github.com/tuupola/copepod) graphics library speed tests for ESP32 based boards. See the accompanying [blog post](https://appelsiini.net/2020/embedded-graphics-library/). Ready made config files for M5Stack, TTGO T-Display and TGO T4 V13. For example to compile and flash for M5Stack run the following.

```
$ git clone git@github.com:tuupola/esp_gfx.git --recursive
$ cd esp_gfx
$ cp sdkconfig.m5stack sdkconfig
$ make -j8 flash
```

If you have some other board or display run menuconfig yourself.

```
$ git clone git@github.com:tuupola/esp_gfx.git --recursive
$ cd esp_gfx
$ make menuconfig
$ make -j8 flash
```

