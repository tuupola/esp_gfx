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

Below testing was done with the [Waveshare ESP32-S3-Touch-LCD-2.8](https://docs.waveshare.com/ESP32-S3-Touch-LCD-2.8). Buffered refresh rate was set to 33 frames per second. Number represents operations per seconsd ie. bigger number is better.

|                               | Single | Double  |
| ----------------------------- | ------ | ------- |
| hagl_put_pixel()              | 11010  | 1510788 |
| hagl_draw_line()              | 306    | 77646   |
| hagl_draw_hline()             | 10217  | 594441  |
| hagl_draw_vline()             | 9304   | 309445  |
| hagl_draw_circle()            | 65     | 17840   |
| hagl_fill_circle()            | 151    | 6729    |
| hagl_draw_ellipse()           | 74     | 18608   |
| hagl_fill_ellipse()           | 125    | 6547    |
| hagl_draw_triangle()          | 139    | 36665   |
| hagl_fill_triangle()          | 321    | 17863   |
| hagl_draw_rectangle()         | 2966   | 135545  |
| hagl_fill_rectangle()         | 462    | 27883   |
| hagl_draw_rounded_rectangle() | 201    | 55350   |
| hagl_fill_rounded_rectangle() | 312    | 22231   |
| hagl_draw_polygon()           | 115    | 28487   |
| hagl_fill_polygon()           | 278    | 10783   |
| hagl_put_char()               | 4785   | 60200   |
| hagl_put_text()               | 318    | 3991    |

## License

The MIT No Attribution License (MIT-0). Please see [LICENSE](LICENSE) for more information.
