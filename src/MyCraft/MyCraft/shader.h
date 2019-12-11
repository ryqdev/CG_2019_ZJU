#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h" // 包含glew来获取所有的必须OpenGL头文件
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// 程序ID
	GLuint ID;

	// 激活当前着色器
	Shader& Use();

	// Compiles the shader from given source code
	void    Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr); // Note: geometry source code is optional 
	
	void    SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
	void    SetInteger(const GLchar* name, GLint value, GLboolean useShader = false);
	void    SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void    SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = false);
	void    SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void    SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = false);
	void    SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void    SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = false);
	void    SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = false);
private:
	// Checks if compilation or linking failed and if so, print the error logs
	void    checkCompileErrors(GLuint object, std::string type);
};


#endif

/*

void init_shader()
{
	// 顶点输入(标准化设备坐标)，会将其发送给顶点着色器
	GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	const GLchar* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";
	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	// 使用 GLSL 语言编写顶点着色器
	// 接着在动态运行时，编译着色器代码
	// 先创建一个着色器对象
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// 将着色器源码附着到着色器对象上
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// 检测编译时候的错误
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 编写片段着色器
	// 编译片段着色器
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 两个着色器编译好后，将这两个着色器对象链接到用来渲染的着色器程序中
	//	着色器程序
	//	着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。
	//	要使用编译好的着色器，需要链接为着色器程序对象，并在渲染对象的时候激活着色器程序
	//	已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。

	// 创建一个着色器程序对象
	shaderProgram = glCreateProgram();
	// 链接着色器对象：当链接着色器至一个程序的时候，它会把每个着色器的输出链接到下个着色器的输入。
	// 当输出和输入不匹配的时候，你会得到一个连接错误。
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// 把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// 顶点着色器会在GPU中创建内存用于存储顶点数据
	// 通过顶点缓冲对象VBO 来管理这个内存，它会在GPU内存(通常被称为显存)中储存大量顶点。
	// 按照下面的方法生成一个 VBO 对象
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	// OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
	// 可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中
	// 注意绑定缓冲类型后，使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 定义了OpenGL该如何解释顶点数据
	// 链接顶点属性: 指定输入数据的哪一个部分对应顶点着色器的哪一个顶点属性(顶点属性在顶点着色器中声明了)
	// 使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
	glVertexAttribPointer(
		0, // 指定要配置的顶点属性, 我们在顶点着色器中有指定 layout (location = 0)
		3, // 顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3
		GL_FLOAT, // 数据类型，(GLSL中vec*都是由浮点数值组成的)
		GL_FALSE, // 是否希望数据被标准化，如果是所有数据都会被映射到0-1之间
		3 * sizeof(GLfloat), // 连续的顶点属性组之间的间隔，
		(GLvoid*)0	// 它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0
	);
	
	// 每个顶点属性从一个VBO管理的内存中获得它的数据，
	// 而具体是从哪个VBO（程序中可以有多个VBO）获取则是通过在
	// 调用glVetexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的。
	// 由于在调用glVetexAttribPointer之前绑定的是先前定义的VBO对象，
	// 顶点属性0现在会链接到它的顶点数据。
	// 以顶点属性位置值作为参数，启用顶点属性
	glEnableVertexAttribArray(0);

	// Note that this is allowed, the call to glVertexAttribPointer registered VBO 
	// as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	glBindVertexArray(0);
	
}
*/

/*
	在 OpenGL 绘制物体现在是像下面这样:
	// 0. 复制顶点数组到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 1. 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 2. 当我们渲染一个物体时要使用着色器程序
	glUseProgram(shaderProgram);
	// 3. 绘制物体
	someOpenGLFunctionThatDrawsOurTriangle();

	使用顶点数组对象可以把所有的状态配置储存在一个对象中，并且可以通过绑定这个对象来恢复状态

	顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，
	任何随后的顶点属性调用都会储存在这个VAO中。这样的好处就是，
	当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候
	只需要绑定相应的VAO就行了。这使在不同顶点数据和属性配置之间切换变得
	非常简单，只需要绑定不同的VAO就行了。刚刚设置的所有状态都将存储在VAO中


	创建一个VAO和创建一个VBO很类似：
		GLuint VAO;
		glGenVertexArrays(1, &VAO);
	要想使用VAO，要做的只是使用glBindVertexArray绑定VAO。
	从绑定之后起，我们应该绑定和配置对应的VBO和属性指针，
	之后解绑VAO供之后使用。当我们打算绘制一个物体的时候，
	我们只要在绘制物体前简单地把VAO绑定到希望使用的设定上就行了。
	这段代码应该看起来像这样：

		// ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
		// 1. 绑定VAO
		glBindVertexArray(VAO);
		// 2. 把顶点数组复制到缓冲中供OpenGL使用
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. 设置顶点属性指针
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//4. 解绑VAO
		glBindVertexArray(0);

		[...]

		// ..:: 绘制代（游戏循环中） :: ..
		// 5. 绘制物体
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		someOpenGLFunctionThatDrawsOurTriangle();
		glBindVertexArray(0);
	*/