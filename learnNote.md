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

## LookAt矩阵原理

![](https://learnopengl-cn.github.io/img/01/09/camera_axes.png)

## 欧拉角
pitch 俯仰角

yaw 偏航角

roll 翻滚角

![欧拉角](https://learnopengl-cn.github.io/img/01/09/camera_pitch_yaw_roll.png)