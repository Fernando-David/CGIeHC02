//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
//---------------
static const char* verdeShader = "shaders/ShaderVerde.vert";
static const char* verdeOscuroShader = "shaders/ShaderVerdeOscuro.vert";
static const char* cafeShader = "shaders/ShaderCafe.vert";
static const char* azulShader = "shaders/ShaderAzul.vert";
static const char* rojoShader = "shaders/ShaderRojo.vert";
//---------------
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
			//X			Y			Z			R		G		B
			//Letra F
			-0.8f, 0.8f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.2f,0.7f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.2f,0.8f,0.0f,	0.0f,	0.0f,	1.0f,

			-0.8f,0.8f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.8f,0.7f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.2f,0.7f,0.0f,	0.0f,	0.0f,	1.0f,

			-0.8f,0.8f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.8f,0.2f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.6f,0.2f,0.0f,	0.0f,	0.0f,	1.0f,

			-0.8f,0.8f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.6f,0.2f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.6f,0.8f,0.0f,	0.0f,	0.0f,	1.0f,

			-0.6f,0.5f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.6f,0.4f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.4f,0.4f,0.0f,	0.0f,	0.0f,	1.0f,

			-0.6f,0.5f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.4f,0.4f,0.0f,	0.0f,	0.0f,	1.0f,
			-0.4f,0.5f,0.0f,	0.0f,	0.0f,	1.0f,


			//Letra D

			0.2f,0.8f,0.0f,		1.0f,	0.0f,	0.0f,
			0.2f,0.2f,0.0f,		1.0f,	0.0f,	0.0f,
			0.4f,0.8f,0.0f,		1.0f,	0.0f,	0.0f,

			0.4f,0.8f,0.0f,		1.0f,	0.0f,	0.0f,
			0.2f,0.2f,0.0f,		1.0f,	0.0f,	0.0f,
			0.4f,0.2f,0.0f,		1.0f,	0.0f,	0.0f,

			0.4f,0.8f,0.0f,		1.0f,	0.0f,	0.0f,
			0.4f,0.6f,0.0f,		1.0f,	0.0f,	0.0f,
			0.6f,0.5f,0.0f,		1.0f,	0.0f,	0.0f,

			0.4f,0.8f,0.0f,		1.0f,	0.0f,	0.0f,
			0.6f,0.5f,0.0f,		1.0f,	0.0f,	0.0f,
			0.8f,0.6f,0.0f,		1.0f,	0.0f,	0.0f,

			0.6f,0.5f,0.0f,		1.0f,	0.0f,	0.0f,
			0.8f,0.4f,0.0f,		1.0f,	0.0f,	0.0f,
			0.8f,0.6f,0.0f,		1.0f,	0.0f,	0.0f,

			0.4f,0.4f,0.0f,		1.0f,	0.0f,	0.0f,
			0.4f,0.2f,0.0f,		1.0f,	0.0f,	0.0f,
			0.6f,0.5f,0.0f,		1.0f,	0.0f,	0.0f,

			0.4f,0.2f,0.0f,		1.0f,	0.0f,	0.0f,
			0.8f,0.4f,0.0f,		1.0f,	0.0f,	0.0f,
			0.6f,0.5f,0.0f,		1.0f,	0.0f,	0.0f,

			//Letra M
			-0.8f,-0.2f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.8f,-0.8f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.6f,-0.8f,0.0f,	0.0f,	1.0f,	0.0f,

			-0.8f,-0.2f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.6f,-0.8f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.7f,-0.2f,0.0f,	0.0f,	1.0f,	0.0f,

			-0.7f,-0.2f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.6f,-0.8f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.6f,-0.5f,0.0f,	0.0f,	1.0f,	0.0f,

			-0.7f,-0.2f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.6f,-0.5f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.5f,-0.4f,0.0f,	0.0f,	1.0f,	0.0f,

			-0.5f,-0.4f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.6f,-0.5f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.4f,-0.5f,0.0f,	0.0f,	1.0f,	0.0f,

			-0.6f,-0.5f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.5f,-0.6f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.4f,-0.5f,0.0f,	0.0f,	1.0f,	0.0f,

			-0.5f,-0.4f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.4f,-0.5f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.3f,-0.2f,0.0f,	0.0f,	1.0f,	0.0f,

			-0.3f,-0.2f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.4f,-0.5f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.4f,-0.8f,0.0f,	0.0f,	1.0f,	0.0f,

			-0.3f,-0.2f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.4f,-0.8f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.2f,-0.2f,0.0f,	0.0f,	1.0f,	0.0f,

			-0.2f,-0.2f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.4f,-0.8f,0.0f,	0.0f,	1.0f,	0.0f,
			-0.2f,-0.8f,0.0f,	0.0f,	1.0f,	0.0f,

			//letra N

			0.2f,-0.2f,0.0f,	1.0f,	1.0f,	0.0f,
			0.2f,-0.8f,0.0f,	1.0f,	1.0f,	0.0f,
			0.4f,-0.8f,0.0f,	1.0f,	1.0f,	0.0f,

			0.2f,-0.2f,0.0f,	1.0f,	1.0f,	0.0f,
			0.4f,-0.8f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.4f,-0.5f, 0.0f,	1.0f,	1.0f,	0.0f,

			0.2f, -0.2f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.4f, -0.5f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.3f, -0.2f, 0.0f,	1.0f,	1.0f,	0.0f,

			0.3f, -0.2f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.4f, -0.5f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.7f, -0.8f, 0.0f,	1.0f,	1.0f,	0.0f,

			0.3f, -0.2f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.7f, -0.8f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.6f, -0.5f, 0.0f,	1.0f,	1.0f,	0.0f,

			0.6f, -0.2f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.6f, -0.5f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.8f, -0.8f, 0.0f,	1.0f,	1.0f,	0.0f,

			0.6f, -0.5f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.7f, -0.8f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.8f, -0.8f, 0.0f,	1.0f,	1.0f,	0.0f,

			0.6f, -0.2f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.8f, -0.8f, 0.0f,	1.0f,	1.0f,	0.0f,
			0.8f, -0.2f, 0.0f,	1.0f,	1.0f,	0.0f,
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,558);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = { //indice 1
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_trianguloverde[] = { //indice 2
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,

	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	GLfloat vertices_cuadradoverde[] = { //indice 3
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);



	GLfloat vertices_cuadradocafe[] = { //indice 4
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f, 0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,

	};

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);


	GLfloat vertices_cuadradorojo[] = { //indice 5
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);
}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//shaders para cubos y puramides
	Shader* shader3 = new Shader(); //indice 2 color verde
	shader3->CreateFromFiles(verdeShader, fShader);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader(); //indice 3 color verde oscuro
	shader4->CreateFromFiles(verdeOscuroShader, fShader);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader(); //indice 4 color café
	shader5->CreateFromFiles(cafeShader, fShader);
	shaderList.push_back(*shader5);

	Shader* shader6 = new Shader(); //indice 5 color azul
	shader6->CreateFromFiles(azulShader, fShader);
	shaderList.push_back(*shader6);

	Shader* shader7 = new Shader(); //indice 6 color rojo
	shader7->CreateFromFiles(rojoShader, fShader);
	shaderList.push_back(*shader7);


}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//cambio de color de fondo a blanco
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		//------------------------------------------------------------------------------------------
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		/*shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//Letras de las iniciales
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();*/
		//------------------------------------------------------------------------------------------

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		// lista de shaders con indice de los shaders para cubo y piramide
		// color verde 2
		// color verde oscuro 3
		// color café 4
		// color azul 5
		// color rojo 6
		//cubo rojo
		shaderList[6].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -4.0f));
		model = glm::scale(model, glm::vec3(0.8f, 1.2f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		// piramide azul
		shaderList[5].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, -4.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// cubos café
		shaderList[4].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -0.8f, -4.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.8f, -0.8f, -4.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		// piramides verde oscuro
		shaderList[3].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -0.425f, -4.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.8f, -0.425f, -4.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// cubos verdes
		shaderList[2].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, -0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.25f, -0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//------------------------------------------------------------------------------------------
		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/