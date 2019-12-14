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






## To-Do list
* 基础要求
- [x] 建模:基于OpenGL/WebGL，具有基本体素(立方体、球、圆柱、圆锥、多 面棱柱、多面棱台)的建模表达能力
- [ ] 存储:具有基本三维网格模型导入导出功能(建议OBJ或DAE格式);
- [ ] 编辑:具有基本材质、纹理的显示和编辑能力;
- [x] 变换:具有基本几何变换功能(旋转、平移、缩放等);
- [ ] 光照:基本光照明模型要求，并实现基本的光源编辑(如调整光源的位置，光强等参数);
- [x] 漫游:能对建模后场景进行漫游如Zoom In/Out， Pan, Orbit, Zoom To Fit等 观察功能。
- [ ] 记录:能够提供动画播放功能(多帧数据连续绘制)，能够提供屏幕截取/保 存功能。
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
