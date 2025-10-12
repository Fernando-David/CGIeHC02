/*
Práctica 6: Texturizado
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
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture logofiTexture;

Texture dadoTextureAnim;
Texture OctoTexture;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Dadodae_M;
Model Dadoobj_M;
Model Dadofbx_M;

Model DadoAnim_M;
Model Carro_M;
Model Cofre_M;
Model LlantaDDer_M;
Model LlantaDIzq_M;
Model LlantaTDer_M;
Model LlantaTIzq_M;


Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//cálculo del promedio de las normales para sombreado de Phong
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
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
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

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.51f,  0.33f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.74f,	0.33f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.74f,	0.66f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.51f,	0.66f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

	GLfloat cubo_verticesAnim[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.01f,  0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.25f,	0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.25f,	0.66f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.01f,	0.66f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.99f,  0.34f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.76f,	0.34f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.76f,	0.66f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.99f,	0.66f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.51f,  0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.74f,	0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.74f,	0.66f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.51f,	0.66f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.74f,  0.99f,		0.0f, 1.0f, 0.0f,
		0.5f,  -0.5f,  0.5f,	0.74f,	0.67f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.51f,	0.67f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.51f,	0.99f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.74f,  0.01f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.74f,	0.333f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.51f,	0.333f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.51f,	0.01f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dadoAnim = new Mesh();
	dadoAnim->CreateMesh(cubo_verticesAnim, cubo_indices, 192, 36);
	meshList.push_back(dadoAnim);
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
         0.0f,  0.5f,  0.0f,   0.487f, 0.739f,   0.0f,  0.707f, -0.707f,  // 0: Vértice Superior
        -0.5f,  0.0f,  0.5f,   0.258f, 0.5f,   0.0f,  0.707f, -0.707f,  // 1: Frente Izquierda
         0.5f,  0.0f,  0.5f,   0.722f, 0.5f,   0.0f,  0.707f, -0.707f,  // 2: Frente Derecha

        // Cara Derecha Superior 
         0.0f,  0.5f,  0.0f,   0.48f, 0.739f,   0.707f, 0.707f, 0.0f,    // 3: Vértice Superior
         0.5f,  0.0f,  0.5f,   0.728f, 0.5f,   0.707f, 0.707f, 0.0f,    // 4: Frente Derecha
         0.5f,  0.0f, -0.5f,   0.96f, 0.739f,   0.707f, 0.707f, 0.0f,    // 5: Atrás Derecha

        // Cara Atrás Superior 
         0.0f,  0.5f,  0.0f,   0.48f, 0.739f,   0.0f,  0.707f, 0.707f,   // 6: Vértice Superior
         0.5f,  0.0f, -0.5f,   0.96f, 0.739f,   0.0f,  0.707f, 0.707f,   // 7: Atrás Derecha
        -0.5f,  0.0f, -0.5f,   0.73f, 0.98f,   0.0f,  0.707f, 0.707f,   // 8: Atrás Izquierda

        // Cara Izquierda Superior 
         0.0f,  0.5f,  0.0f,   0.48f, 0.739f,  -0.707f, 0.707f, 0.0f,    // 9: Vértice Superior
        -0.5f,  0.0f, -0.5f,   0.014f, 0.739f,  -0.707f, 0.707f, 0.0f,    // 10: Atrás Izquierda
        -0.5f,  0.0f,  0.5f,   0.247f, 0.5f,  -0.707f, 0.707f, 0.0f,    // 11: Frente Izquierda

        // ------------------ PIRÁMIDE INFERIOR ------------------
        // Cara Frente Inferior 
         0.0f, -0.5f,  0.0f,   0.48f, 0.25f,   0.0f, -0.707f, -0.707f,  // 12: Vértice Inferior
         0.5f,  0.0f,  0.5f,   0.724f, 0.5f,   0.0f, -0.707f, -0.707f,  // 13: Frente Derecha
        -0.5f,  0.0f,  0.5f,   0.251f, 0.5f,   0.0f, -0.707f, -0.707f,  // 14: Frente Izquierda

        // Cara Derecha Inferior 
         0.0f, -0.5f,  0.0f,   0.5f, 0.256f,   0.707f, -0.707f, 0.0f,   // 15: Vértice Inferior
         0.5f,  0.0f, -0.5f,   0.957f, 0.252f,   0.707f, -0.707f, 0.0f,   // 16: Atrás Derecha
         0.5f,  0.0f,  0.5f,   0.724f, 0.49f,   0.707f, -0.707f, 0.0f,   // 17: Frente Derecha

        // Cara Atrás Inferior 
         0.0f, -0.5f,  0.0f,   0.496f, 0.248f,   0.0f, -0.707f, 0.707f,   // 18: Vértice Inferior
        -0.5f,  0.0f, -0.5f,   0.728f, 0.009f,   0.0f, -0.707f, 0.707f,   // 19: Atrás Izquierda
         0.5f,  0.0f, -0.5f,   0.955f, 0.248f,   0.0f, -0.707f, 0.707f,   // 20: Atrás Derecha

        // Cara Izquierda Inferior 
         0.0f, -0.5f,  0.0f,   0.478f, 0.25f,  -0.707f, -0.707f, 0.0f,   // 21: Vértice Inferior
        -0.5f,  0.0f,  0.5f,   0.248f, 0.487f,  -0.707f, -0.707f, 0.0f,   // 22: Frente Izquierda
        -0.5f,  0.0f, -0.5f,   0.022f, 0.25f,  -0.707f, -0.707f, 0.0f,   // 23: Atrás Izquierda

	};

	Mesh* dadoOcto = new Mesh();
	dadoOcto->CreateMesh(Octo_vertices, Octo_indices, 192, 24);
	meshList.push_back(dadoOcto);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
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
	dadoTexture = Texture("Textures/dadonum.jpg");
	dadoTexture.LoadTexture();
	//Animales
	dadoTextureAnim = Texture("Textures/dado_animales.jpg");
	dadoTextureAnim.LoadTexture();
	//----
	//Dado de 8 caras	
	OctoTexture = Texture("Textures/Dado8.jpg");
	OctoTexture.LoadTexture();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	
	
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");

	Dadodae_M = Model();
	Dadodae_M.LoadModel("Models/dado.dae");

	Dadoobj_M = Model();
	Dadoobj_M.LoadModel("Models/dado.obj");

	Dadofbx_M = Model();
	Dadofbx_M.LoadModel("Models/dado.fbx");

	DadoAnim_M = Model();
	DadoAnim_M.LoadModel("Models/dadoAnim.obj");
	
	//Carro
	Carro_M = Model();	
	Carro_M.LoadModel("Models/Carro.fbx");
	Cofre_M = Model();
	Cofre_M.LoadModel("Models/Cofre.fbx");
	LlantaTDer_M = Model();
	LlantaTDer_M.LoadModel("Models/LlantaTDer.obj");
	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 Octo(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelCarro(1.0);
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
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();


		//Dado del ejercicio de clase texturizado por OpenGL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.5f, 8.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTextureAnim.UseTexture();
		meshList[5]->RenderMesh();
		
		//Dado de Opengl 8 caras
		Octo = glm::mat4(1.0);
		Octo = glm::translate(Octo, glm::vec3(-6.5f, 10.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Octo));
		OctoTexture.UseTexture();
		meshList[6]->RenderMesh();
		
		//Dado importado con Blender
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-9.5f, 8.5f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DadoAnim_M.RenderModel();

		//Carro
		modelCarro = glm::mat4(1.0);
		modelCarro = glm::translate(modelCarro, glm::vec3(-20.0f, 3.0f, 0.0f));
		modelCarro = glm::rotate(modelCarro, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelCarro = glm::rotate(modelCarro, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelCarro = glm::scale(modelCarro, glm::vec3(8.0f, 8.0f, 8.0f));
		modelaux = modelCarro;
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		Carro_M.RenderModel();
		//Cofre
		//color = glm::vec3(0.25f, 0.0f, 0.0f);
		//modelCarro = modelaux;
		modelCarro = glm::translate(modelCarro, glm::vec3(0.0f, -1.3f, 0.21f));
		//modelaux = model;
	
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		Cofre_M.RenderModel();
		//Llanta Delantera Derecha
		modelCarro = modelaux;
		modelCarro = glm::translate(modelCarro, glm::vec3(-1.0f, -1.5f, -0.25f));
		//modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		LlantaTDer_M.RenderModel();

		//Llanta Delantera izquierda
		modelCarro = modelaux;
		modelCarro = glm::translate(modelCarro, glm::vec3(1.0f, -1.5f, -0.25f));
		//modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		LlantaTDer_M.RenderModel();

		//Llanta Trasera Derecha
		modelCarro = modelaux;
		modelCarro = glm::translate(modelCarro, glm::vec3(-1.0f, 1.3f, -0.25f));
		//modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		LlantaTDer_M.RenderModel();

		//Llanta Trasera Izquierda
		modelCarro = modelaux;
		modelCarro = glm::translate(modelCarro, glm::vec3(1.0f, 1.3f, -0.25f));
		//modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro));
		LlantaTDer_M.RenderModel();




		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/