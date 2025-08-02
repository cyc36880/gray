#include "../inc/color.h"

#include "../inc/sys_config_and_flash.h"

#define SORT_SIZE 10

typedef struct
{
    uint8_t num; // 前几个有效
    struct
    {
        uint8_t color_pos;
        int diff;
    } color_diff[COLOR_STUDY_NUM];

} color_diff_t;

static void rgb2hsv(lv_color_hsv_t *hsv);
static void update_flash_data(void);


static color_table_t color_tab[SENSORE_NUM][COLOR_STUDY_NUM] = {0};
static lv_color_hsv_t sort_buf[SENSORE_NUM][SORT_SIZE] = {0};
static lv_color_hsv_t median[SENSORE_NUM][SORT_SIZE] = {0};
static color_diff_t color_diff[SENSORE_NUM] = {0};
uint8_t colorVal[REGISTER_NUM] = {0};

static void color_hsv_sort_h(lv_color_hsv_t *hsv_buf, uint8_t len)
{
    lv_color_hsv_t hsv;
    for (uint8_t i = 0; i < len - 1; i++)
    {
        for (uint8_t j = i + 1; j < len; j++)
        {
            if (hsv_buf[j].h < hsv_buf[i].h)
            {
                hsv = hsv_buf[i];
                hsv_buf[i] = hsv_buf[j];
                hsv_buf[j] = hsv;
            }
        }
    }
}

static uint16_t getColorOffset(uint16_t max, uint16_t thr, uint16_t cVal)
{
    int16_t val = (thr + cVal) / 2;
    uint16_t val1 = abs(val - thr);
    uint16_t val2 = abs(val - cVal);

    if ((val1 + val2) < max / 2)
        return val1 + val2;
    else
        return max - (val1 + val2);
}

void color_init(void)
{
    // get_sys_config_data(SYSCONF_MEM_LIST_COLOR_TAB, color2_tab);
}

void color_study(uint8_t color)
{
    if (color >= COLOR_STUDY_NUM)
        return;
    for (uint8_t i = 0; i < SORT_SIZE; i++)
    {
        for (uint8_t j = 0; j < SORT_SIZE; j++)
        {
            lv_color_hsv_t hsv[SENSORE_NUM];
            rgb2hsv(&hsv);
            for (uint8_t k = 0; k < SENSORE_NUM; k++)
            {
                sort_buf[k][j].h = hsv[k].h;
                sort_buf[k][j].s = hsv[k].s;
                sort_buf[k][j].v = hsv[k].v;
            }
        }
        set_rgb_brightness(700, 700, 700); //
        HAL_Delay(15);                  // 增加训练的视觉感受
        set_rgb_brightness(0, 0, 0);       //
        HAL_Delay(15);                  // 增加训练的视觉感受
        for (uint8_t j = 0; j < SENSORE_NUM; j++)
        {
            color_hsv_sort_h(sort_buf[j], SORT_SIZE);
            median[j][i].h = sort_buf[j][SORT_SIZE / 2].h;
            median[j][i].s = sort_buf[j][SORT_SIZE / 2].s;
            median[j][i].v = sort_buf[j][SORT_SIZE / 2].v;
        }
    }
    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        color_hsv_sort_h(median[i], SORT_SIZE);
        color_tab[i][color].hsv.h = median[i][SORT_SIZE / 2].h;
        color_tab[i][color].hsv.s = median[i][SORT_SIZE / 2].s;
        color_tab[i][color].hsv.v = median[i][SORT_SIZE / 2].v;
        color_tab[i][color].has_color = 1;
    }

    update_flash_data();
}

void color_identify(void)
{
    lv_color_hsv_t hsv[SENSORE_NUM] = {0};
    rgb2hsv(hsv);

    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        uint8_t color_diff_count = 0;
        for (uint8_t j = 0; j < COLOR_STUDY_NUM; j++)
        {
            if (1 == color_tab[i][j].has_color)
            {
                color_diff[i].color_diff[color_diff_count].diff =
                    getColorOffset(360, color_tab[i][j].hsv.h, hsv[i].h);
                color_diff[i].color_diff[color_diff_count].color_pos = j;
                color_diff_count++;
            }
        }
        color_diff[i].num = color_diff_count;

        // 没有颜色
        if (color_diff[i].num == 0)
        {
            colorVal[i] = 0;
            return;
        }

        // 找到最接近的颜色
        uint8_t min_pos = 0;
        for (uint8_t j = 1; j < color_diff[i].num; j++)
        {
            if (color_diff[i].color_diff[j].diff < color_diff[i].color_diff[min_pos].diff)
            {
                min_pos = j;
            }
        }
        if (color_diff[i].color_diff[min_pos].diff > 20) // 颜色差异过大
        {
            colorVal[i] = 0;
        }
        else
        {
            colorVal[i] = color_diff[i].color_diff[min_pos].color_pos + 1;
        }
    }
}

void clear_color_study(void)
{
    memset(color_tab, 0, sizeof(color_tab));
    update_flash_data();
}

static void update_flash_data(void)
{
    set_sys_config_info(SYSCONF_MEM_LIST_COLOR_TAB, color_tab, sizeof(color_tab));
    sync_sys_config_info();
}

// =================================
/**
 * @description: r g b 转 hsv
 * @param {uint8_t} r 红灯亮度（0-255）
 * @param {uint8_t} g 绿灯亮度（0-255）
 * @param {uint8_t} b 蓝灯亮度（0-255）
 * @return {*}
 */
static void rgb2hsv(lv_color_hsv_t *hsv)
{
    colot_t color[SENSORE_NUM] = {0};

    set_rgb_brightness(600, 0, 0);
    HAL_Delay(10);
    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        color[i].r = VAL4096_2_255(get_adc_val(i));
    }

    set_rgb_brightness(0, 600, 0);
    HAL_Delay(10);
    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        color[i].g = VAL4096_2_255(get_adc_val(i));
    }

    set_rgb_brightness(0, 0, 600);
    HAL_Delay(10);
    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        color[i].b = VAL4096_2_255(get_adc_val(i));
    }

    set_rgb_brightness(0, 0, 0);

    for (uint8_t i = 0; i < SENSORE_NUM; i++)
    {
        hsv[i] = lv_color_rgb_to_hsv(color[i].r, color[i].g, color[i].b);
    }
}
