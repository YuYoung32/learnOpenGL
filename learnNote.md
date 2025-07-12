from https://learnopengl-cn.github.io/

GL库就像一个大的状态机，面向过程编程。可以分为两类函数：

* 状态设置函数(State-changing Function)，这类函数将会改变上下文
* 状态使用函数(State-using Function)，这类函数会根据当前OpenGL的状态执行一些操作

GL库与外界的数据交互使用各种int值，可以理解为常规编程中的地址。套路是

1. 先生成一个XX Object，生成一个int值代表这个Object
2. bind到某个TARGET上，类似于放到某一个全局位置上
3. 针对某个TARGET做操作，比如存放数据、改变属性。这里为什么不用传统编程方式传入这个int？理解为每一个状态改变都依赖上下文，如果传入ID进行操作则忽略了上下文，该函数就类似一种工具函数了，但设计理念显然不是这样。

渲染过程

![RenderProcess](https://learnopengl-cn.github.io/img/01/04/pipeline.png)

## VBO，EBO，VAO

**VBO**

Vertex Buffer Object，用于存储顶点数组到GPU中，不是必须的，也可以每帧再传入。提前缓存进GPU有利于性能优化。

套路，各种Object都是这个套路：

```C++
unsigned int VBO;
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO); // 放置到某种类型的槽位上
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 针对某种类型的槽位里面的东西做操作
```

**EBO、IBO**

Element Buffer Object 或称为 Index Buffer Object 是为了复用VBO的顶点，解决重复顶点带来的浪费。

套路：

```C++
unsigned int EBO; // 元素缓冲对象：Element Buffer Object，EBO 存储 OpenGL 用来决定要绘制哪些顶点的索引 或 索引缓冲对象 Index Buffer Object，IBO
glGenBuffers(1, &EBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 放置到某种类型的槽位上
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 针对某种类型的槽位里面的东西做操作
```

**VAO**

Vertex Array Buffer, 说是顶点数组Buffer，其实更像一个管理器。VAO用来配置顶点的属性, 本身并不存储顶点。

在上下文bind一个VAO的情况下，后续bind的VBO和EBO会追加到这个VAO上（不能颠倒顺序，必须先bind
VAO）。因此在draw前bind一个VAO就可以了！无需再bind一堆buffer。使用OpenGL的核心模式时，在draw之前**必须**要bind一个VAO。

VBO不是为VAO而生, 可以在bind VBO后调用`glVertexAttribPointer`把VBO内的数据放到VAO里.

一个VAO可以使用多个VBO的数据, 一个VBO也可以被多个VAO使用.

VAO的管理结构如下：

![VAO](https://learnopengl-cn.github.io/img/01/04/vertex_array_objects_ebo.png)

套路：

```C++
unsigned int VAO; // 顶点数组对象：Vertex Array Object，VAO
glGenVertexArrays(1, &VAO);
glBindVertexArray(VAO);  // 放置到某种类型的槽位上，直接通过函数名指定

// 预先告诉GL Vertex Shader里的in参数如何读取数据，理解为一种特化的glUniform
// position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
glEnableVertexAttribArray(0);
// color attribute
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
glEnableVertexAttribArray(1);
// texture0 coord attribute
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
glEnableVertexAttribArray(2);
```

### 向Shader传递数据的方式

1. 通过绑定VAO然后自动读取`GL_ARRAY_BUFFER`槽位，通过layout自动绑定位置。

   ```c++
   // 创建和绑定一个 VBO（存储顶点位置）
   GLuint positionVBO;
   glGenBuffers(1, &positionVBO);
   glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
   
   // 创建 VAO
   GLuint vao;
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);
   
   // 设置顶点属性指针（位置属性）
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   ```

   ```glsl
   #version 330 core
   layout(location = 0) in vec3 aPos; // 顶点属性
   void main() {
       gl_Position = vec4(aPos, 1.0);
   }
   ```

2. 通过uniform

   ```c++
   GLuint shaderProgram = ...; // 已经编译和链接的着色器程序
   glUseProgram(shaderProgram);
   
   GLuint uniformPosLoc = glGetUniformLocation(shaderProgram, "uniformPosition");
   glUniform3f(uniformPosLoc, 0.0f, 0.5f, 0.0f); // 设置 uniform 的值
   ```

   ```glsl
   #version 330 core
   uniform vec3 uniformPosition; // 使用 uniform 变量
   void main() {
       gl_Position = vec4(uniformPosition, 1.0);
   }
   ```

方式总结：

| **传递方式**                      | **描述**                                                     | **特点**                                                     | **适用场景**                                                 |
| --------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **顶点属性 (Vertex Attribute)**   | 每个顶点的独立属性，例如位置、法线、颜色、纹理坐标等。       | - 数据与每个顶点一一对应<br>- 通过 VBO（顶点缓冲对象）提供<br>- 使用 `layout(location)` 标记 | 用于逐个顶点数据，例如顶点坐标、法线、纹理坐标等。           |
| **统一变量 (Uniform)**            | 所有顶点共享的全局变量，例如变换矩阵、光照参数等。           | - 每帧或每次绘制调用可以更改<br>- 全局共享值<br>- 使用 `glUniform` 设置 | 用于传递每帧不变的全局数据，例如模型矩阵、视图矩阵、光照方向等。 |
| **常量 (Constants)**              | 着色器中的硬编码变量。                                       | - 值固定，不能动态修改<br>- 直接在着色器中定义               | 用于固定的参数或标志，例如比例因子、常量参数等。             |
| **纹理 (Texture)**                | 使用纹理存储相关数据，通过采样器读取（如高度图、法线贴图、顶点位置信息）。 | - 可以存储大量数据<br>- 数据通常是图像或二维数组<br>- 顶点着色器中通过 `sampler` 和 `texture()` 访问 | 用于地形高度图、法线贴图或其他需要通过二维纹理查找的数据。   |
| **着色器存储缓冲对象 (SSBO)**     | 存储大量动态数据的缓冲对象，可在着色器中直接读取或写入。     | - 适用于大量数据<br>- 支持动态读取和写入<br>- 需要 OpenGL 4.3 及以上版本 | 用于存储和访问大量动态数据，例如实例化渲染、复杂的变换矩阵或大规模结构化数据。 |
| **纹理缓冲对象 (TBO)**            | 一种特殊类型的缓冲对象，以纹理形式访问大规模数据（类似 SSBO，但使用纹理 API）。 | - 数据通过纹理单元访问<br>- 和 SSBO 类似，但兼容性更强       | 用于大规模数据，例如离散采样点、顶点属性或动态索引的缓冲数据。 |
| **实例属性 (Instance Attribute)** | 在实例化渲染中，为每个实例传递独立的属性，例如每个实例的变换矩阵、颜色等。 | - 适用于实例化渲染<br>- 使用 `glVertexAttribDivisor` 设置更新频率 | 用于实例化渲染，例如场景中每个对象的独立变换矩阵或颜色。     |

## Texture

from [learnOpenGL-CN](https://learnopengl-cn.github.io/01%20Getting%20started/06%20Textures/)

**坐标**

纹理坐标和Normalized Device Coordinates是不一样的。每次顶点着色器运行后，可见的所有顶点都为标准化设备坐标(Normalized Device Coordinate, NDC)。

NDC：

![NDC](https://learnopengl-cn.github.io/img/01/04/ndc.png)

纹理坐标：

![texture.png](https://learnopengl-cn.github.io/img/01/06/tex_coords.png)

```text
^ Y (1,1)
|
|
|
- - - - -> X (1,0)
```

**环绕方式**

尝试读取纹理坐标，纹理坐标超出后[0,1]后如何处理。

![TextureWrap](https://learnopengl-cn.github.io/img/01/06/texture_wrapping.png)

**纹理过滤**

缩放时要如何取颜色。

![TextureFilter](https://learnopengl-cn.github.io/img/01/06/filter_nearest.png)
![TextureFilter](https://learnopengl-cn.github.io/img/01/06/filter_linear.png)

![TextureFilter](https://learnopengl-cn.github.io/img/01/06/texture_filtering.png)

套路
```C++
// Texture
int width, height, nrChannels;

unsigned char *data = stbi_load("../src/assets/container.jpg", &width, &height, &nrChannels, 0);
unsigned int texture0;
glGenTextures(1, &texture0);
glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
glBindTexture(GL_TEXTURE_2D, texture0);

// 纹理超出后的环绕方式 S T R 类似 X Y Z
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
// 纹理缩放后的采样方式
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
glGenerateMipmap(GL_TEXTURE_2D);
stbi_image_free(data);
data = nullptr;

shader.setInt("texture1", 0); // 0代表GL_TEXTURE0

```

## 3D坐标系

OpenGL坐标系是右手坐标系

![](https://learnopengl-cn.github.io/img/01/08/coordinate_systems_right_handed.png)



## 视角空间转换过程

![](https://learnopengl-cn.github.io/img/01/08/coordinate_systems.png)



## LookAt矩阵原理

![](https://learnopengl-cn.github.io/img/01/09/camera_axes.png)

## 欧拉角
pitch 俯仰角

yaw 偏航角

roll 翻滚角

![欧拉角](https://learnopengl-cn.github.io/img/01/09/camera_pitch_yaw_roll.png)

## 光照

光照无法完全模拟，只有近似模型。其中一个模型被称为风氏光照模型(Phong Lighting Model)。风氏光照模型的主要结构由3个分量组成：环境(Ambient)、漫反射(Diffuse)和镜面(Specular)光照。下面这张图展示了这些光照分量看起来的样子：

![Phine Mix](https://learnopengl-cn.github.io/img/02/02/basic_lighting_phong.png)

- 环境光照(Ambient Lighting)：即使在黑暗的情况下，世界上通常也仍然有一些光亮（月亮、远处的光），所以物体几乎永远不会是完全黑暗的。为了模拟这个，我们会使用一个环境光照常量，它永远会给物体一些颜色。环境光在物体四面都有，是真正的环境。
- 漫反射光照(Diffuse Lighting)：模拟光源对物体的方向性影响(Directional Impact)。它是风氏光照模型中视觉上最显著的分量。**物体的某一部分越是正对着光源（接近法线），它就会越亮。**
- 镜面光照(Specular Lighting)：模拟有光泽物体上面出现的亮点。镜面光照的颜色相比于物体的颜色会更倾向于光的颜色。**视线的某一部分越是正对着点的法线，镜面效果越强**

关注对法向量的处理：

1. 法向量应使用World Space坐标系，因为需要和世界中其他元素做运算

2. 法向量应当对物体的变换做变化，但是变化和物体矩阵不同，因为物体模型进行位移和拉伸后会改变，法向量按照这样改变后变不会垂直与面。法线的变换矩阵是模型矩阵的inverse的transpose。

   ![](https://learnopengl-cn.github.io/img/02/02/basic_lighting_normal_transformation.png)



