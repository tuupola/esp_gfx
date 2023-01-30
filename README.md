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

Below testing was done with the [TTGO T-Display](http://www.lilygo.cn/prod_view.aspx?Id=1126). Buffered refresh rate was set to 33 frames per second. Number represents operations per seconsd ie. bigger number is better.

|                               | Single | Double    |Triple   |
|-------------------------------|--------|-----------|---------|
| hagl_put_pixel()              |  14526 |    682850 |  683024 |
| hagl_draw_line()              |    171 |     15290 |   15264 |
| hagl_draw_vline()             |  10293 |    132997 |  132980 |
| hagl_draw_hline()             |  10276 |    132993 |  132997 |
| hagl_draw_circle()            |    170 |     17475 |   17473 |
| hagl_fill_circle()            |    270 |      9256 |    9259 |
| hagl_draw_ellipse()           |    100 |      9110 |    9110 |
| hagl_fill_ellipse()           |    127 |      3408 |    3407 |
| hagl_draw_triangle()          |     57 |      5130 |    5129 |
| hagl_fill_triangle()          |    103 |      1975 |    1975 |
| hagl_draw_rectangle()         |   2752 |     37910 |   37916 |
| hagl_fill_rectangle()         |    165 |      4574 |    4566 |
| hagl_draw_rounded_rectangle() |    482 |     26120 |   26110 |
| hagl_fill_rounded_rectangle() |    156 |      4287 |    4301 |
| hagl_draw_polygon()           |     35 |      3088 |    3091 |
| hagl_fill_polygon()           |     62 |      1073 |    1071 |
| hagl_put_char()               |        |           |         |

## License

The MIT No Attribution License (MIT-0). Please see [LICENSE](LICENSE) for more information.