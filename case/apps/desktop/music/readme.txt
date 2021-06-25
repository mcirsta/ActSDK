场景和控件示例


一、文件说明

1. 场景
（1）列表场景: scene_listmenu.c, scene_listmenu.h
（2）播放场景: scene_playing.c, scene_playing.h
（3）设置场景: scene_option.c, scene_option.h

2. 控件
（1）标题栏: headerbar.c, headerbar.h
（2）菜单列表: listmenu.c, listmenu.h
（3）音量条: volumebar.c, volumebar.h
（4）设置: option.c, option.h

3. 绘图函数库
（1）GUI风格绘图函数库: style_gui.c, style_gui.h
（2）DC风格绘图函数库: style_dc.c, style_dc.h

4. 主应用
（1）应用入口: music_demo.c

5. 其它
（1）UI-Editor生成资源头文件: logo_sty.h
（2）调试宏定义: global.h


二、操作说明

1. 场景切换
（1）列表场景 <--> 播放场景 <--> 设置场景

2. 按键操作

（1）列表场景: 
	Left/Right－弹出音量条；
	Up/Down－上／下一条；
	Enter－进入播放场景；
	Return－隐藏音量条／退出应用；

（2）播放场景: 
	Enter－进入设置场景；
	Return－退回列表场景；

（3）设置场景
	Enter－退回播放场景；
	Return－退回播放场景；
