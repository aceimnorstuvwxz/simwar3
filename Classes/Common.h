#ifndef __COMMON_SCENE_H__
#define __COMMON_SCENE_H__

namespace config
{
    const int tank_max = 2;

	const float window_height = 600.0f;
	const float window_width = 1200.0f;

    // 地图
    const float cube_y = 1813.f /(80 * 2 +1 );
	const float cube_x = 1645.0f / (2 + 1.5f*79);// 六边形半径

	// ¥ÛµÿÕºUI≤Œ ˝
	const float map_space_height = 600.0f;
	const float map_space_width = 900.0f;
	const float map_space_x = 0.f;
	const float map_space_y = 0.f;

    // …Ë÷√«¯’˚ÃÂ≤Œ ˝
    const float setting_area_x = map_space_width;
    const float setting_area_y = 0.f;
    const float setting_area_w = window_width - map_space_width;
    const float setting_area_h = window_height;

	// –°µÿÕºUI≤Œ ˝
    const float minimap_width = setting_area_w;
	const float minimap_height = minimap_width * 0.8f;
	const float minimap_x = map_space_width;
	const float minimap_y = 0;

    // time
    const float bigmap_move_time = 0.3f;
    const float tank_move_time = 0.1f;

    // 状态烂
    const float status_scale = 0.3;
    const float status_x = setting_area_x + setting_area_w / 2;
    const float status_y = window_height - 20;
    const float status_y_line_height = 25;
    const float status_y_0 = window_height - 60 - status_y_line_height * 0;
    const float status_y_1 = status_y_0 - status_y_line_height;
    const float status_y_2 = status_y_1 - status_y_line_height;
    const float status_y_3 = status_y_2 - status_y_line_height;
    const float status_y_4 = status_y_3 - status_y_line_height;
    const float status_y_5 = status_y_4 - status_y_line_height;
    const float status_l_x = setting_area_x + 20;

    // 按键
    const float btn_x = setting_area_x + 20;
    const float btn_y_start  = status_y_5 - 20;
    const float btn_y_strip = 38;
    const float btn_scale = 0.29f;
    const float btn_y_m1 = btn_y_start - btn_y_strip * 0;
    const float btn_y_0 = btn_y_m1  - btn_y_strip;
    const float btn_y_1 = btn_y_0 - btn_y_strip;
    const float btn_y_2 = btn_y_1 - btn_y_strip;
    const float btn_y_3 = btn_y_2 - btn_y_strip;

    // 规则属性
    const int init_move_point = 5;

    // tank
    const float tanke_scale = 0.2f;

    // 杂
    const float target_scale = 0.5f;
}
typedef
struct _cord
{
    int x,y;
    _cord(int xx, int yy):x(xx), y(yy){};
    bool operator==(const struct _cord& right){
        return x == right.x && y == right.y;
    }
} Cord;
#endif