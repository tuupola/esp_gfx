## Graphics speed tests for ESP32

![Circles](https://appelsiini.net/img/2020/pod-draw-circle.png)

[HAGL](https://github.com/tuupola/hagl) graphics library speed tests for ESP32 based boards. See the accompanying [blog post](https://appelsiini.net/2020/embedded-graphics-library/). Ready made config files for M5Stack, TTGO T-Display and TGO T4 V13. For example to compile and flash for M5Stack run the following.

```
$ git clone https://github.com/tuupola/esp_gfx.git --recursive
$ cd esp_gfx
$ cp sdkconfig.m5stack sdkconfig
$ make -j8 flash
```

If you have some other board or display run menuconfig yourself.

```
$ git clone https://github.com/tuupola/esp_gfx.git --recursive
$ cd esp_gfx
$ make menuconfig
$ make -j8 flash
```

Or if you are using the new build system.

```
$ git clone https://github.com/tuupola/esp_gfx.git --recursive
$ cd esp_gfx
$ idf.py menuconfigs
$ idf.py build flash
```

## Speed

Below testing was done with the [Waveshare ESP32-S3-LCD-1.47](https://www.waveshare.com/wiki/ESP32-S3-LCD-1.47). Buffered refresh rate was set to 33 frames per second. Number represents operations per seconsd ie. bigger number is better.

|                               | Single | Double  | Triple  |
| ----------------------------- | ------ | ------- | ------- |
| hagl_put_pixel()              | 10340  | 1495646 | 1495443 |
| hagl_draw_line()              | 301    | 76827   | 76879   |
| hagl_draw_hline()             | 9075   | 592837  | 592845  |
| hagl_draw_vline()             | 8447   | 339555  | 339553  |
| hagl_draw_circle()            | 66     | 18076   | 18077   |
| hagl_fill_circle()            | 129    | 6661    | 6661    |
| hagl_draw_ellipse()           | 74     | 18862   | 18861   |
| hagl_fill_ellipse()           | 106    | 6455    | 6454    |
| hagl_draw_triangle()          | 137    | 36338   | 36337   |
| hagl_fill_triangle()          | 297    | 17178   | 17180   |
| hagl_draw_rectangle()         | 2724   | 147899  | 147897  |
| hagl_fill_rectangle()         | 410    | 27837   | 27838   |
| hagl_draw_rounded_rectangle() | 201    | 55798   | 55800   |
| hagl_fill_rounded_rectangle() | 280    | 22021   | 22021   |
| hagl_draw_polygon()           | 116    | 28044   | 28044   |
| hagl_fill_polygon()           | 245    | 10310   | 10305   |
| hagl_put_char()               | 4599   | 60465   | 60466   |
| hagl_put_text()               | 328    | 4047    | 4049    |

## License

The MIT No Attribution License (MIT-0). Please see [LICENSE](LICENSE) for more information.
