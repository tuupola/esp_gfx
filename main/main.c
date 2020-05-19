/*

MIT No Attribution

Copyright (c) 2018-2020 Mika Tuupola

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

SPDX-License-Identifier: MIT-0

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_task_wdt.h>

#include <i2c_helper.h>
#include <axp192.h>

#include "sdkconfig.h"
#include "hagl_hal.h"
#include "bitmap.h"
#include "hagl.h"
#include "font6x9.h"
#include "fps.h"
#include "aps.h"

static const char *TAG = "main";
static char primitive[17][32] = {
    "RGB BARS",
    "PIXELS",
    "LINES",
    "CIRCLES",
    "FILLED CIRCLES",
    "ELLIPSES",
    "FILLED ELLIPSES",
    "TRIANGLES",
    "FILLED TRIANGLES",
    "RECTANGLES",
    "FILLED RECTANGLES",
    "ROUND RECTANGLES",
    "FILLED ROUND RECTANGLES",
    "POLYGONS",
    "FILLED POLYGONS",
    "CHARACTERS",
    "STRINGS"
};

static SemaphoreHandle_t mutex;
static float fb_fps;
static float fx_fps;
static uint16_t current_demo = 0;
static bitmap_t *bb;
static uint32_t drawn = 0;
static axp192_t axp;

/*
 * Flushes the framebuffer to display in a loop. This demo is
 * capped to 30 fps.
 */
void framebuffer_task(void *params)
{
    TickType_t last;
    const TickType_t frequency = 1000 / 30 / portTICK_RATE_MS;

    last = xTaskGetTickCount();

    while (1) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        hagl_flush();
        xSemaphoreGive(mutex);
        fb_fps = fps();
        vTaskDelayUntil(&last, frequency);
    }

    vTaskDelete(NULL);
}

/*
 * Displays the info bar on top of the screen.
 */
void fps_task(void *params)
{
    uint16_t color = hagl_color(0, 255, 0);
    char16_t message[128];

#ifdef HAGL_HAL_USE_BUFFERING
    while (1) {
        fx_fps = aps(drawn);
        drawn = 0;

        hagl_set_clip_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);

        swprintf(message, sizeof(message), L"%.*f %s PER SECOND       ", 0, fx_fps, primitive[current_demo]);
        hagl_put_text(message, 6, 4, color, font6x9);
        swprintf(message, sizeof(message), L"%.*f FPS  ", 1, fb_fps);
        hagl_put_text(message, DISPLAY_WIDTH - 56, DISPLAY_HEIGHT - 14, color, font6x9);

        hagl_set_clip_window(0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
#else
    while (1) {
        fx_fps = aps(drawn);
        drawn = 0;

        swprintf(message,  sizeof(message), L"%.*f %s PER SECOND       ", 0, fx_fps, primitive[current_demo]);
        hagl_set_clip_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);
        hagl_put_text(message, 8, 4, color, font6x9);
        hagl_set_clip_window(0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);


        vTaskDelay(2000 / portTICK_RATE_MS);
    }
#endif
    vTaskDelete(NULL);
}

void switch_task(void *params)
{
    while (1) {
        ESP_LOGI(TAG, "%.*f %s per second, FB %.*f FPS", 0, fx_fps, primitive[current_demo], 1, fb_fps);

        current_demo = (current_demo + 1) % 17;
        hagl_clear_clip_window();
        aps(APS_RESET);
        drawn = 0;

        vTaskDelay(10000 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

void polygon_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x2 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y2 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x3 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y3 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x4 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y4 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};
    hagl_draw_polygon(5, vertices, colour);
}

void fill_polygon_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x2 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y2 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x3 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y3 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x4 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y4 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    int16_t vertices[10] = {x0, y0, x1, y1, x2, y2, x3, y3, x4, y4};
    hagl_fill_polygon(5, vertices, colour);
}

void circle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t r = (rand() % 40);
    color_t colour = rand() % 0xffff;
    hagl_draw_circle(x0, y0, r, colour);
}

void fill_circle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t r = (rand() % 40);
    color_t colour = rand() % 0xffff;
    hagl_fill_circle(x0, y0, r, colour);
}

void ellipse_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t a = (rand() % 40) + 20;
    uint16_t b = (rand() % 40) + 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_ellipse(x0, y0, a, b, colour);
}

void fill_ellipse_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    uint16_t a = (rand() % 40) + 20;
    uint16_t b = (rand() % 40) + 20;
    color_t colour = rand() % 0xffff;
    hagl_fill_ellipse(x0, y0, a, b, colour);
}

void line_demo()
{
    // strcpy(primitive, "LINES");

    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_line(x0, y0, x1, y1, colour);
}

void rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_rectangle(x0, y0, x1, y1, colour);
}

void fill_rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_fill_rectangle(x0, y0, x1, y1, colour);
}

void put_character_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;

    color_t colour = rand() % 0xffff;
    char ascii = rand() % 127;
    hagl_put_char(ascii, x0, y0, colour, font6x9);
}

void put_text_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 80;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;

    color_t colour = rand() % 0xffff;

    hagl_put_text(u"YO¡ MTV raps2♥", x0, y0, colour, font6x9);
}

void put_pixel_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_put_pixel(x0, y0, colour);
}

void triangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x2 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y2 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_draw_triangle(x0, y0, x1, y1, x2, y2, colour);
}

void fill_triangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x2 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y2 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    color_t colour = rand() % 0xffff;
    hagl_fill_triangle(x0, y0, x1, y1, x2, y2, colour);
}

void rgb_demo()
{
    uint16_t red = hagl_color(255, 0, 0);
    uint16_t green = hagl_color(0, 255, 0);
    uint16_t blue = hagl_color(0, 0, 255);

    int16_t x0 = 0;
    int16_t x1 = DISPLAY_WIDTH / 3;
    int16_t x2 = 2 * x1;

    hagl_fill_rectangle(x0, 0, x1 - 1, DISPLAY_HEIGHT, red);
    hagl_fill_rectangle(x1, 0, x2 - 1, DISPLAY_HEIGHT, green);
    hagl_fill_rectangle(x2, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, blue);
}

void round_rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t r = rand() % 10;
    color_t colour = rand() % 0xffff;
    hagl_draw_rounded_rectangle(x0, y0, x1, y1, r, colour);
}

void fill_round_rectangle_demo()
{
    int16_t x0 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y0 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t x1 = (rand() % DISPLAY_WIDTH + 20) - 20;
    int16_t y1 = (rand() % DISPLAY_HEIGHT + 20) - 20;
    int16_t r = rand() % 10;
    color_t colour = rand() % 0xffff;
    hagl_fill_rounded_rectangle(x0, y0, x1, y1, r, colour);
}

void demo_task(void *params)
{
    void (*demo[17]) ();

    demo[0] = rgb_demo;
    demo[1] = put_pixel_demo;
    demo[2] = line_demo;
    demo[3] = circle_demo;
    demo[4] = fill_circle_demo;
    demo[5] = ellipse_demo;
    demo[6] = fill_ellipse_demo;
    demo[7] = triangle_demo;
    demo[8] = fill_triangle_demo;
    demo[9] = rectangle_demo;
    demo[10] = fill_rectangle_demo;
    demo[11] = round_rectangle_demo;
    demo[12] = fill_round_rectangle_demo;
    demo[13] = polygon_demo;
    demo[14] = fill_polygon_demo;
    demo[15] = put_character_demo;
    demo[16] = put_text_demo;

    while (1) {
        //current_demo = 2;
        (*demo[current_demo])();
        drawn++;
    }


    vTaskDelete(NULL);
}

void app_main()
{
    ESP_LOGI(TAG, "SDK version: %s", esp_get_idf_version());
    ESP_LOGI(TAG, "Heap when starting: %d", esp_get_free_heap_size());

    ESP_LOGI(TAG, "Initializing I2C");
    i2c_init();

    ESP_LOGI(TAG, "Initializing AXP192");
    axp.read = &i2c_read;
    axp.write = &i2c_write;
    axp192_init(&axp);

    bb = hagl_init();
    if (bb) {
        ESP_LOGI(TAG, "Back buffer: %dx%dx%d", bb->width, bb->height, bb->depth);
    }

    ESP_LOGI(TAG, "Heap after HAGL init: %d", esp_get_free_heap_size());

    hagl_set_clip_window(0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);
    mutex = xSemaphoreCreateMutex();

    if (NULL != mutex) {
#ifdef HAGL_HAL_USE_BUFFERING
        xTaskCreatePinnedToCore(framebuffer_task, "Framebuffer", 8192, NULL, 1, NULL, 0);
#endif
        xTaskCreatePinnedToCore(fps_task, "FPS", 8192, NULL, 2, NULL, 1);
        xTaskCreatePinnedToCore(demo_task, "Demo", 8192, NULL, 1, NULL, 1);
        xTaskCreatePinnedToCore(switch_task, "Switch", 2048, NULL, 2, NULL, 1);
    } else {
        ESP_LOGE(TAG, "No mutex?");
    }
}
