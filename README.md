# CG_2019_ZJU
This is our Computer Graphics course project in ZJU

For the sake of the correctness in README, Chinese will be used in the following:

# 做什么
## 我的世界

## Requirements

需要依赖有: glut, glew, glm, SOIL
- glm 用于矩阵等数学运算
- SOIL 用于读取各种格式的图片(显示纹理)

现在项目中应该已经这些库应该已经有了，在项目的 `include` 与 `libs` 目录下。

我的开发环境是VS2019, 项目配置如下:
- 使用 **Debug x86** 进行项目配置和编译
- 项目-属性-平台选 WIN32
- 在 VC++目录-包含目录中添加 `$(ProjectDir)include`
- 在 VC++目录-库目录中添加 `$(ProjectDir)libs`
- 在 链接器-输入-附加依赖项中添加 `opengl32.lib;SOIL.lib;glew32s.lib` (`opengl32.lib`一定要有)

## 项目结构介绍

- `main.cpp`: 创建了一个全局的游戏对象`Game MineCraft(SCREEN_WIDTH, SCREEN_HEIGHT);`, 在相应的 glut 响应函数中调用游戏对象中的逻辑。
- `Game` 类: 主要看 `game.h` 中的接口和相应注释。主要参考了 [LearnOpenGL](https://learnopengl-cn.github.io/06%20In%20Practice/2D-Game/02%20Setting%20up/) 中的游戏结构


**物体渲染**

游戏渲染，在 Game类中的 `Render` 方法中，调用 `world->render();` 渲染整个世界。

- `world` 类, 方块, 天空盒等所有在世界中的对象都在该类中创建。世界的大小为 WIDTH, 方块的坐标 x, y, z 均为整数。(这一部分有点乱。。)
    - `skyBox` 类，`World::Load()` 的时候创建一个 skybox 对象，`World::init()` 的时候初始化渲染, `World::render()` 的时候调用 `skyBox.render();` 直接渲染出天空盒。
    - `block` 与 `cubeRender` 类, `block` 是一个个方块对象, `cubeRender` 是立方体渲染器。`block` 使用 `cuberender` 进行渲染。

**着色器与纹理**
- 纹理图片资源放在项目的`textures`目录中，着色器程序放在项目的 `shaders` 目录中
- `texture` 类: 用于加载图片，生成纹理ID (不需要直接调用，给`resource_manager` 类用的)
- `shader` 类: 用于加载，编译着色程序，创建着色器 (不需要直接调用，给`resource_manager` 类用的)
- `resource_manager` 类: 均使用该类**加载与使用**纹理资源和着色器资源, 详细参照 [LearnOpenGL](https://learnopengl-cn.github.io/06%20In%20Practice/2D-Game/02%20Setting%20up/)
    `Game::Init()` 与 `void Block::Draw(CubeRender* Render)` 中有用到

**其它**
- `mousePicker` 类: 鼠标拾取功能的实现
- `carame` 类: 相机漫游，跳跃，飞行与重力，碰撞检测(依赖于world的接口)等功能


## 2019年12月14日更新
- 对MyCraft工程的.h文件和.cpp文件归档整理，以后大家就在那两个文件夹里添加类
- 导入Opencv库，用以增加截屏功能，[opencv导入教程](https://blog.csdn.net/weixin_43085694/article/details/103537012)
- 添加了screenshot.h和screenshot.cpp两个文件，在main函数的Key函数中添加了'P'键截屏，并且保存在指定文档中

## 2019年12月16日更新
- 修复飞行时空格键问题。修改方块的创建逻辑，能够在远距离创建方块。
- 增加鼠标右键消除方块功能，K 键进行视角的 Zoom 功能 
- 增加创建不同种类的方块功能，**Q 键切换想要放置的方块**。目前有 草，冰块，金块三种。若需要增加更多的类型。添加相应的`xxBlock.h` 与 `xxBlock.cpp`, 并修改 `blockType.h` 与 `blockFactory.h` 即可
- 截屏功能修改为使用 SOIL 库保存, 图片保存在项目的 `screenshot` 文件夹中

## 2019年12月17日更新

* 以16 * 16 * 128的大小划分区块，使用柏林噪声生成地形，修改原本绘制与修改地形的代码。

## 2019年12月18日更新
* 修复了工程运行失败的问题
* 完善截屏功能，图片保存在screenshot文件里，截图命名为年_月_日_小时_分钟_秒

## 2019年12月22日更新
* 增加了树木与机器人的绘制
* 目前帧率较低，同时树木与机器人存在与地面不完全贴合的问题

## 2019年12月23日更新

* 修改地图生成方法，动态加载、卸载相机附近的区块

## 2019年12月26日更新

* 增加存储功能，开始游戏前在控制台输入存档名，在save文件夹中创建txt文件
* 游戏中I键打开旧存档，U键开始新存档，在控制台中输入存档名
* 若在chunkManager类中定义chunks，更新游戏时无法渲染方块，暂时放回world类中定义chunks
* `file` 类:控制文件的创建输入输出

## To-Do list
* 基础要求
- [x] 建模:基于OpenGL/WebGL，具有基本体素(立方体、球、圆柱、圆锥、多 面棱柱、多面棱台)的建模表达能力
- [ ] 存储:具有基本三维网格模型导入导出功能(建议OBJ或DAE格式);
- [ ] 编辑:具有基本材质、纹理的显示和编辑能力;
- [x] 变换:具有基本几何变换功能(旋转、平移、缩放等);
- [ ] 光照:基本光照明模型要求，并实现基本的光源编辑(如调整光源的位置，光强等参数);
- [x] 漫游:能对建模后场景进行漫游如Zoom In/Out， Pan, Orbit, Zoom To Fit等 观察功能。
- [x] 记录:能够提供动画播放功能(多帧数据连续绘制)，能够提供屏幕截取/保 存功能。
* 高级要求
- [ ] 具有NURBS曲面建模能力
- [x] 漫游时可实时碰撞检测
- [ ] 光照明模型细化，可任选实现实时阴影、caustic、位移纹理、全局光照明(光子跟踪)、辐射度、AO叠加等
- [ ] 采用HTML5/IOS/Android移动平台实现
- [ ] 构建了基于此引擎的完整三维游戏，具有可玩性
- [ ] 与虚拟现实/增强现实应用结合
- [ ] 具有一定的对象表达能力，能够表达门、窗、墙等;
- [ ] 复杂材质效果



# 成员
商钰坤（组长）
蒋博文
梁家宁
王道源
徐贤民
杨钰茹


# 时间表
| 时间段 | 成员 | 任务 |
| ------ | ------ | ------ |
| 2019.12.1 - 12.8 | 全体 | 讨论构思 |
| 2019.12.8 - 12.11 | 商钰坤、蒋博文 | 构建基础的OpenGL/C++框架 |
| 2019.12.8 - 12.11 | 梁家宁、王道源、徐贤民、杨钰茹 | 构思和建立功能模块 |
| 2019.12.11课前课间 | 全体 | 讨论细节与可行性 |
| 2019.12.11 - 12.18 | 全体 | 依据最新的分工，实现各自的功能 |
|2019.12.18 - |全体|各自完成分配到的任务|
