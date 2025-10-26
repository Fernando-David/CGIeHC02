/*
Práctica 8: Iluminación 2 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture OctoTexture;
Texture LadrilloTexture;


Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model DaCapo_M;
Model Carro_M;
Model Cofre_M;
Model Bird_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);



}

void CrearDadoOcto()
{
	unsigned int Octo_indices[] = {
		// Pirámide Superior - 4 caras triangulares
			// Cara Frente Superior
			0, 1, 2,
			// Cara Derecha Superior
			3, 4, 5,
			// Cara Atrás Superior
			6, 7, 8,
			// Cara Izquierda Superior
			9, 10, 11,

			// Pirámide Inferior - 4 caras triangulares
			// Cara Frente Inferior
			12, 13, 14,
			// Cara Derecha Inferior
			15, 16, 17,
			// Cara Atrás Inferior
			18, 19, 20,
			// Cara Izquierda Inferior
			21, 22, 23

	};


	GLfloat Octo_vertices[] = {
		// x,     y,     z,     s,    t,      nx,     ny,     nz

		// ------------------ PIRÁMIDE SUPERIOR ------------------
		// Cara Frente Superior 
		 0.0f,  0.5f,  0.0f,   0.487f, 0.739f,   0.0f,  -0.707f, -0.707f,  // 0: Vértice Superior
		-0.5f,  0.0f,  0.5f,   0.258f, 0.5f,   0.0f,  -0.707f, -0.707f,  // 1: Frente Izquierda
		 0.5f,  0.0f,  0.5f,   0.722f, 0.5f,   0.0f,  -0.707f, -0.707f,  // 2: Frente Derecha

		 // Cara Derecha Superior 
		  0.0f,  0.5f,  0.0f,   0.48f, 0.739f,   -0.707f, -0.707f, 0.0f,    // 3: Vértice Superior
		  0.5f,  0.0f,  0.5f,   0.728f, 0.5f,   -0.707f, -0.707f, 0.0f,    // 4: Frente Derecha
		  0.5f,  0.0f, -0.5f,   0.96f, 0.739f,   -0.707f, -0.707f, 0.0f,    // 5: Atrás Derecha

		  // Cara Atrás Superior 
		   0.0f,  0.5f,  0.0f,   0.48f, 0.739f,   0.0f,  -0.707f, 0.707f,   // 6: Vértice Superior
		   0.5f,  0.0f, -0.5f,   0.96f, 0.739f,   0.0f,  -0.707f, 0.707f,   // 7: Atrás Derecha
		  -0.5f,  0.0f, -0.5f,   0.73f, 0.98f,   0.0f,  -0.707f, 0.707f,   // 8: Atrás Izquierda

		  // Cara Izquierda Superior 
		   0.0f,  0.5f,  0.0f,   0.48f, 0.739f,  0.707f, -0.707f, 0.0f,    // 9: Vértice Superior
		  -0.5f,  0.0f, -0.5f,   0.014f, 0.739f,  0.707f, -0.707f, 0.0f,    // 10: Atrás Izquierda
		  -0.5f,  0.0f,  0.5f,   0.247f, 0.5f,  0.707f, -0.707f, 0.0f,    // 11: Frente Izquierda

		  // ------------------ PIRÁMIDE INFERIOR ------------------
		  // Cara Frente Inferior 
		   0.0f, -0.5f,  0.0f,   0.48f, 0.25f,   0.0f, 0.707f, -0.707f,  // 12: Vértice Inferior
		   0.5f,  0.0f,  0.5f,   0.724f, 0.5f,   0.0f, 0.707f, -0.707f,  // 13: Frente Derecha
		  -0.5f,  0.0f,  0.5f,   0.251f, 0.5f,   0.0f, 0.707f, -0.707f,  // 14: Frente Izquierda

		  // Cara Derecha Inferior 
		   0.0f, -0.5f,  0.0f,   0.5f, 0.256f,   -0.707f, 0.707f, 0.0f,   // 15: Vértice Inferior
		   0.5f,  0.0f, -0.5f,   0.957f, 0.252f,   -0.707f, 0.707f, 0.0f,   // 16: Atrás Derecha
		   0.5f,  0.0f,  0.5f,   0.724f, 0.49f,   -0.707f, 0.707f, 0.0f,   // 17: Frente Derecha

		   // Cara Atrás Inferior 
			0.0f, -0.5f,  0.0f,   0.496f, 0.248f,   0.0f, 0.707f, 0.707f,   // 18: Vértice Inferior
		   -0.5f,  0.0f, -0.5f,   0.728f, 0.009f,   0.0f, 0.707f, 0.707f,   // 19: Atrás Izquierda
			0.5f,  0.0f, -0.5f,   0.955f, 0.248f,   0.0f, 0.707f, 0.707f,   // 20: Atrás Derecha

			// Cara Izquierda Inferior 
			 0.0f, -0.5f,  0.0f,   0.478f, 0.25f,  0.707f, 0.707f, 0.0f,   // 21: Vértice Inferior
			-0.5f,  0.0f,  0.5f,   0.248f, 0.487f,  0.707f, 0.707f, 0.0f,   // 22: Frente Izquierda
			-0.5f,  0.0f, -0.5f,   0.022f, 0.25f,  0.707f, 0.707f, 0.0f,   // 23: Atrás Izquierda

	};

	Mesh* dadoOcto = new Mesh();
	dadoOcto->CreateMesh(Octo_vertices, Octo_indices, 192, 24);
	meshList.push_back(dadoOcto); //Indice 4 en meshList
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDadoOcto();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	//Dado de 8 caras	
	OctoTexture = Texture("Textures/Dado8.jpg");
	OctoTexture.LoadTexture();
	LadrilloTexture = Texture("Textures/Ladrillo.png");
	LadrilloTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/Llanta.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	//Lampara de DaCapo
	DaCapo_M = Model();
	DaCapo_M.LoadModel("Models/DaCapo.obj");
	Carro_M = Model();
	Carro_M.LoadModel("Models/Carro.fbx");
	Cofre_M = Model();
	Cofre_M.LoadModel("Models/Cofre.fbx");

	Bird_M = Model();
	Bird_M.LoadModel("Models/Pbird.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	//Verde
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	//Luz faro
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		10.0f, 2.7f, -10.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	//Luz del pajaro
	pointLights[2] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 0.5f,
		0.0f, 5.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//Faro del cofre del coche
	spotLights[1] = SpotLight(1.0f, 0.0f, 0.0f,
		1.0f, 0.8f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//delantera del coche
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//Luz trasera coche
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	//Matrices de modelos
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelCarro(1.0);
	glm::mat4 Luz(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);

		/*if (mainWindow.getPrendida())
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}
		//Para el pajaro
		if (mainWindow.getPrendida2())
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}*/

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, 4.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		LadrilloTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Dado de 8 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 5.0f, -4.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		OctoTexture.UseTexture();
		meshList[4]->RenderMesh();

		//DaCapo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -1.0f, -10.0f));
		//A la luz 1 le pongo la posición del DaCapo
		pointLights[1].SetPos(glm::vec3(model[3]));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DaCapo_M.RenderModel();

		//Carro propio
		modelCarro = glm::mat4(1.0);
		modelCarro = glm::translate(modelCarro, glm::vec3(-40.0f, 4.0f, 0.0f));
		modelCarro = glm::translate(modelCarro, glm::vec3(0.0f + mainWindow.getmuevex(), 0.5f, -3.0f));
		// Luz delantera
		spotLights[2].SetFlash(glm::vec3(modelCarro[3]) + glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// Luz trasera
		spotLights[3].SetFlash(glm::vec3(modelCarro[3]) - glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

		PointLight activePointLights[MAX_POINT_LIGHTS];
		unsigned int activePointLightCount = 0;

		activePointLights[activePointLightCount++] = pointLights[0];

		// Luz de la lampara
		if (mainWindow.getPrendida())
		{
			activePointLights[activePointLightCount++] = pointLights[1];
		}

		// Luz del pajaro
		if (mainWindow.getPrendida2())
		{
			activePointLights[activePointLightCount++] = pointLights[2];
		}
		
		SpotLight activeSpotLights[MAX_SPOT_LIGHTS];
		unsigned int activeSpotLightCount = 0;

		// ActiveSpotLights se llena con las luces que solo se van a usar
		activeSpotLights[activeSpotLightCount++] = spotLights[0]; // Flash cámara
		activeSpotLights[activeSpotLightCount++] = spotLights[1]; // Luz del Cofre

		// Se agregan las luces al Shaderlist que se enviarán al shader
		if (mainWindow.getMovimientoCoche() == 1.0f) {
			activeSpotLights[activeSpotLightCount++] = spotLights[2]; // Encender delantera
		}
		else if (mainWindow.getMovimientoCoche() == -1.0f) {
			activeSpotLights[activeSpotLightCount++] = spotLights[3]; // Encender trasera
		}
		
		//--------
		modelCarro = glm::rotate(modelCarro, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelCarro = glm::rotate(modelCarro, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelCarro = glm::scale(modelCarro, glm::vec3(8.0f, 8.0f, 8.0f));
		modelaux = modelCarro;



		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		Carro_M.RenderModel();
		modelCarro = glm::translate(modelCarro, glm::vec3(0.0f, -1.3f, 0.21f));
		modelCarro = glm::rotate(modelCarro, glm::radians(mainWindow.getArticulacionCofre()), glm::vec3(-1.0f, 0.0f, 0.0f));
		//Luz del cofre
		// Posicion del cofre
		glm::vec3 cofrePos = glm::vec3(modelCarro[3]);
		//Vector de dirección local del cofre (antes de transformaciones)
		glm::vec3 cofreDirLocal = glm::vec3(0.0f, -1.0f, 0.0f);
		//  Se rota el vector de dirección al sistema de coordenadas global
		//   Se usa mat3 para tomar solo la parte de rotación/escala
		glm::vec3 cofreDirGlobal = glm::normalize(glm::mat3(modelCarro) * cofreDirLocal);

		activeSpotLights[1].SetFlash(cofrePos, cofreDirGlobal);

		//Se mandan las luces puntuales activas al shader
		shaderList[0].SetPointLights(activePointLights, activePointLightCount);

		//Se mandan las luces tipo spotlight activas al shader
		shaderList[0].SetSpotLights(activeSpotLights, activeSpotLightCount);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		Cofre_M.RenderModel();
		//Llanta Delantera Derecha
		modelCarro = modelaux;
		modelCarro = glm::translate(modelCarro, glm::vec3(-1.0f, -1.5f, -0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		Llanta_M.RenderModel();

		//Llanta Delantera izquierda
		modelCarro = modelaux;
		modelCarro = glm::translate(modelCarro, glm::vec3(1.0f, -1.5f, -0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		Llanta_M.RenderModel();

		//Llanta Trasera Derecha
		modelCarro = modelaux;
		modelCarro = glm::translate(modelCarro, glm::vec3(-1.0f, 1.3f, -0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		Llanta_M.RenderModel();

		//Llanta Trasera Izquierda
		modelCarro = modelaux;
		modelCarro = glm::translate(modelCarro, glm::vec3(1.0f, 1.3f, -0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		Llanta_M.RenderModel();

		//Punishing Bird
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -1.0f, -10.0f));
		pointLights[2].SetPos(glm::vec3(model[3]));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bird_M.RenderModel();


		//----------------------------------------------------------------------------------------------------
		// 
				//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
