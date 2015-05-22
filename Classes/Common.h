#ifndef __COMMON_SCENE_H__
#define __COMMON_SCENE_H__

namespace config
{
	const float window_height = 600.0f;
	const float window_width = 1200.0f;

	const float cube_radius = 20.0f;//六边形半径
	

	// 大地图UI参数
	const float map_space_height = 600.0f;
	const float map_space_width = 900.0f;
	const float map_space_x = 0.f;
	const float map_space_y = 0.f;

    // 设置区整体参数
    const float setting_area_x = map_space_width;
    const float setting_area_y = 0.f;
    const float setting_area_w = window_width - map_space_width;
    const float setting_area_h = window_height;

	// 小地图UI参数
    const float minimap_width = setting_area_w;
	const float minimap_height = minimap_width * 0.618;
	const float minimap_x = map_space_width;
	const float minimap_y = 0;
}

#endif