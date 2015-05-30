/* 游戏配置参数 */
#ifndef __COMMON_SCENE_H__
#define __COMMON_SCENE_H__

namespace config
{
    const int tank_max = 2;//坦克数量

	const float window_height = 600.0f;//窗口大小
	const float window_width = 1200.0f;

    // 地图方格大小
    const float cube_y = 1813.f /(80 * 2 +1 );
	const float cube_x = 1645.0f / (2 + 1.5f*79);// 六边形半径

	// 大地图位置
	const float map_space_height = 600.0f;
	const float map_space_width = 900.0f;
	const float map_space_x = 0.f;
	const float map_space_y = 0.f;

    // 功能区域位置和大小
    const float setting_area_x = map_space_width;
    const float setting_area_y = 0.f;
    const float setting_area_w = window_width - map_space_width;
    const float setting_area_h = window_height;

	// 小地图位置和大小
    const float minimap_width = setting_area_w;
	const float minimap_height = minimap_width * 0.8f;
	const float minimap_x = map_space_width;
	const float minimap_y = 0;

    // 时间
    const float bigmap_move_time = 0.3f;
    const float tank_move_time = 0.1f;

    // 状态栏各部件位置
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
    const float status_l_x2 = setting_area_x + 130;
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
    const int init_move_point = 5;//初始化机动值

    // tank
    const float tanke_scale = 0.2f;//坦克缩放

    // 杂
    const float target_scale = 0.5f;//目标缩放
    const float small_scale = 0.07f;//小地图坦克缩放
}
typedef
//坐标类
struct _cord
{
    int x,y;
    _cord(int xx, int yy):x(xx), y(yy){};
    bool operator==(const struct _cord& right){
        return x == right.x && y == right.y;
    }
} Cord;
#endif