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
| hagl_put_pixel()              | 11021  | 1499285 |
| hagl_draw_line()              | 306    | 57305   |
| hagl_draw_vline()             | 9315   | 298878  |
| hagl_draw_hline()             | 10256  | 594243  |
| hagl_draw_circle()            | 66     | 16922   |
| hagl_fill_circle()            | 107    | 5951    |
| hagl_draw_ellipse()           | 74     | 18189   |
| hagl_fill_ellipse()           | 126    | 6637    |
| hagl_draw_triangle()          | 138    | 27450   |
| hagl_fill_triangle()          | 338    | 12324   |
| hagl_draw_rectangle()         | 2977   | 131637  |
| hagl_fill_rectangle()         | 464    | 27721   |
| hagl_draw_rounded_rectangle() | 203    | 53933   |
| hagl_fill_rounded_rectangle() | 313    | 22193   |
| hagl_draw_polygon()           | 115    | 21623   |
| hagl_fill_polygon()           | 289    | 7653    |
| hagl_put_char()               | 4263   | 59648   |
| hagl_put_text()               | 293    | 3908    |

## License

The MIT No Attribution License (MIT-0). Please see [LICENSE](LICENSE) for more information.
