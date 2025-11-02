/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
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

#include <stdlib.h> 
#include <time.h>   

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

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza=true;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float dragonavance = 0.0f;
float avanceoffset;
//Para el control de los numeros
float tiempoAcumuladoNumeros = 0.0f;
float tiempoPorNumero = 20.0f; //Controla cada cuanto cambia el numero
int contadorCambio = 0; //Contador para saber que numero mostrar
bool mostrarTextura1 = true; //Para mostrar la textura 1 o 2 de los numeros
//Para el dragon
bool dragonX = true; // true = yendo, false = regresando
float RotacionZ = 180.0f * toRadians; // Rotación Z inicial
//Para el letrero------------------------------------------------------
float toffsetLetrero = 0.0f; 
float velocidadLetrero = 0.01f; 
//Para la puerta
float RpuertaDer = 0.0f; // Angulo de rotacion ACTUAL
float TpuertaIzq = 0.0f; // Traslacion X ACTUAL
float TpuertaIzq_Z = 0.0f; // Traslacion Z ACTUAL
float AjusteP = -0.5f; // Ajuste para que la puerta no atraviese el pilar

float TpuertaIzq_Target_X = -2.0f; //Desplazamiento objetivo en X
float RpuertaDer_Target = 90.0f; // Rotacion objetivo de 90 grados
float velocidadPuerta = 0.05f; // Multiplicador para la velocidad de la animacion

// --- Variables para Animación del Dado --------------------------------------------------
bool estaAnimando = false;
float tiempoInicioAnim = 0.0f;
float duracionAnim = 1.5f; // Duración de 1.5 segundos

// Estado actual del dado
glm::vec3 posicionActualDado = glm::vec3(-20.0f, 6.0f, 10.0f); // Posición inicial
glm::vec3 angulosActualesDado = glm::vec3(0.0f, 0.0f, 0.0f); // Rotación inicial

// Para la ROTACIÓN (en grados)
glm::vec3 angulosIniciales;      // Rotación al inicio del lanzamiento
glm::vec3 angulosObjetivo;       // La rotación final
glm::vec3 angulosFinalesAnim;    // La rotación para giros extra

// Para la TRASLACIÓN
glm::vec3 posicionInicial;       // Posición al inicio
glm::vec3 posicionFinal = glm::vec3(-20.0f, -1.5f, 10.0f); // Dónde aterriza
glm::vec3 posicionCenit = glm::vec3(-20.0f, 10.0f, 10.0f); // El punto más alto del arco

// Vector para almacenar las 8 rotaciones objetivo de las 8 caras posibles
std::vector<glm::vec3> rotacionesEulerObjetivo(8);

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture Octo_T;
Texture Puerta_T;
Texture Pilar_T;
Texture Letrero_T;


Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;
Model AlaDer_M;
Model AlaIzq_M;
Model PuertaDer_M;
Model PuertaIzq_M;
Model Pilar_M;
Model Letrero_M;

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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

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

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

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
	meshList.push_back(dadoOcto); 
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}
//Sirve para poder hacer interpolacion lineal y calcular los angulos y posicion en cada frame
float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void calcularRotacionesObjetivo() {
	// Aquí definimos "a mano" los ángulos de Euler (en grados)
	// necesarios para que cada cara apunte hacia arriba (0, 1, 0).

	// Caras Superiores (normales con Y negativa)
	rotacionesEulerObjetivo[0] = glm::vec3(135.0f, 0.0f, 0.0f);  // Cara 1 (Frente Sup)
	rotacionesEulerObjetivo[1] = glm::vec3(0.0f, 0.0f, 135.0f);  // Cara 2 (Derecha Sup)
	rotacionesEulerObjetivo[2] = glm::vec3(-135.0f, 0.0f, 0.0f); // Cara 3 (Atrás Sup)
	rotacionesEulerObjetivo[3] = glm::vec3(0.0f, 0.0f, -135.0f); // Cara 4 (Izquierda Sup)

	// Caras Inferiores (normales con Y positiva)
	rotacionesEulerObjetivo[4] = glm::vec3(-45.0f, 0.0f, 0.0f);  // Cara 5 (Frente Inf)
	rotacionesEulerObjetivo[5] = glm::vec3(0.0f, 0.0f, 45.0f);   // Cara 6 (Derecha Inf)
	rotacionesEulerObjetivo[6] = glm::vec3(45.0f, 0.0f, 0.0f);   // Cara 7 (Atrás Inf)
	rotacionesEulerObjetivo[7] = glm::vec3(0.0f, 0.0f, -45.0f);  // Cara 8 (Izquierda Inf)
}

void tirarDado() {
	if (estaAnimando) return; // No hacer nada si ya está animándose

	estaAnimando = true;
	tiempoInicioAnim = (float)glfwGetTime();

	// Guardar estado inicial
	posicionInicial = posicionActualDado;
	angulosIniciales = angulosActualesDado;

	// Se elige un resultado aleatorio entre 0 y 7
	int resultado = rand() % 8;
	angulosObjetivo = rotacionesEulerObjetivo[resultado]; // Obtiene el vec3 objetivo

	// Se agregan giros extra aleatorios
	angulosFinalesAnim.x = angulosObjetivo.x + (rand() % 4 + 3) * 360.0f;
	angulosFinalesAnim.y = angulosObjetivo.y + (rand() % 4 + 3) * 360.0f;
	angulosFinalesAnim.z = angulosObjetivo.z + (rand() % 4 + 3) * 360.0f;
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDadoOcto();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

	Octo_T = Texture("Textures/Dado8.jpg");
	Octo_T.LoadTextureA();
	Puerta_T = Texture("Texture/PuertaAmarilla.jpg");
	Puerta_T.LoadTextureA();
	Pilar_T = Texture("Textures/PilaresT.jpg");
	Pilar_T.LoadTextureA();
	Letrero_T = Texture("Textures/Letrero.png");
	Letrero_T.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dragon_M = Model();
	Dragon_M.LoadModel("Models/17174_Tiamat_new.obj");
	Tiamat_M = Model();
	Tiamat_M.LoadModel("Models/Dragon.obj");
	AlaDer_M = Model();
	AlaDer_M.LoadModel("Models/AlaDer.obj");
	AlaIzq_M = Model();
	AlaIzq_M.LoadModel("Models/AlaIzq.obj");

	PuertaDer_M = Model();
	PuertaDer_M.LoadModel("Models/PuertaDer.obj");
	PuertaIzq_M = Model();
	PuertaIzq_M.LoadModel("Models/PuertaIzq.obj");
	Pilar_M = Model();
	Pilar_M.LoadModel("Models/PilarDer.obj");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/Letrero.obj");

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
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
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

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	srand((unsigned int)time(NULL)); // Inicializar el generador aleatorio
	calcularRotacionesObjetivo();    // Pre-calcular las 8 rotaciones del dado

	movCoche = 0.0f;
	movOffset = 0.3f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	avanceoffset = 0.1f;

	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//< >
		angulovaria += 5.0f * deltaTime;
		//dragonavance
		//><
		/*Animación en LOOP: NUNCA SE DETIENE
		Si tu animación en algún momento se va a iniciar y detener, debes de hacer que el ciclo de animación
		inicie y termine en el mismo estado
		*/
		if(avanza)
		{
			if (movCoche > -250.0f)
			{
				movCoche -= movOffset * deltaTime;
				rotllanta -= rotllantaOffset * deltaTime;
			}
			else
			{
				avanza = !avanza;
			}
		}
		else
		{
			if (movCoche < 350.0f)
			{
				movCoche += movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
			}
			else
				{
				avanza = !avanza;
			}
		}
		
		//animación del dragón
		// 
		//dragonavance-=avanceoffset* deltaTime;

		if (dragonX)
		{
			// Estado 1: Hacia adelante
			dragonavance -= avanceoffset * deltaTime;
			if (dragonavance <= -20.0f)
			{
				dragonavance = -20.0f;
				dragonX = false;
				RotacionZ = 0.0f;
			}
		}
		else
		{
			// Estado 2: Regresando
			dragonavance += avanceoffset * deltaTime;
			if (dragonavance >= 0.0f)
			{
				dragonavance = 0.0f;
				dragonX = true;
				RotacionZ = 180.0f * toRadians;
			}
		}
//Animacion del dado-----------------------------------------------------------------
		if (estaAnimando) {
			float tiempoTranscurrido = lastTime - tiempoInicioAnim;
			float t = tiempoTranscurrido / duracionAnim; // Tiempo normalizado [0.0, 1.0]

			if (t >= 1.0f) {
				// --- Animación terminada ---
				t = 1.0f;
				estaAnimando = false;
				posicionActualDado = posicionFinal;
				angulosActualesDado = angulosObjetivo; // Fija la rotación final exacta

			}
			else {
				// --- Animación en progreso ---

				// Interpolar Rotación (con Lerp)
				angulosActualesDado.x = lerp(angulosIniciales.x, angulosFinalesAnim.x, t);
				angulosActualesDado.y = lerp(angulosIniciales.y, angulosFinalesAnim.y, t);
				angulosActualesDado.z = lerp(angulosIniciales.z, angulosFinalesAnim.z, t);

				// Interpolar Traslación (Arco con Bezier cuadrática)
				float uno_menos_t = 1.0f - t;
				posicionActualDado = (uno_menos_t * uno_menos_t * posicionInicial) +
					(2.0f * uno_menos_t * t * posicionCenit) +
					(t * t * posicionFinal);
			}
		}

		//Animacion de la puerta
		if (mainWindow.getPuertaAbierta())
		{
			// --- ABRIR PUERTAS ---
			// Interpolar suavemente hacia los valores objetivo
			RpuertaDer = lerp(RpuertaDer, RpuertaDer_Target, velocidadPuerta * deltaTime);
			TpuertaIzq = lerp(TpuertaIzq, TpuertaIzq_Target_X, velocidadPuerta * deltaTime);
			TpuertaIzq_Z = lerp(TpuertaIzq_Z, AjusteP, velocidadPuerta * deltaTime);
		}
		else
		{
			// --- CERRAR PUERTAS ---
			// Interpolar suavemente de vuelta a posicion original
			RpuertaDer = lerp(RpuertaDer, 0.0f, velocidadPuerta * deltaTime);
			TpuertaIzq = lerp(TpuertaIzq, 0.0f, velocidadPuerta * deltaTime);
			TpuertaIzq_Z = lerp(TpuertaIzq_Z, 0.0f, velocidadPuerta * deltaTime);
		}


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
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();



		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche - 50.0f, -0.2f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//dragonavance para moverlo hacia adelante y atrás en X
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f+dragonavance, 5.0f + 3*sin(glm::radians(angulovaria)), 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, RotacionZ, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		/*color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tiamat_M.RenderModel();
		
		//alas del dragón
		//ala derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.25f, -3.0f));
		model = glm::rotate(model, (10.0f * sin(glm::radians(angulovaria))) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaDer_M.RenderModel();

		//ala izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, (-10.0f * sin(glm::radians(angulovaria))) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaIzq_M.RenderModel();

		//Para tirar el dado presionar L
		if (mainWindow.getsKeys()[GLFW_KEY_L] && !estaAnimando)
		{
			tirarDado();
		}

		//Dado 8 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, posicionActualDado); // Usar posición animada

		// Aplicar rotaciones Euler
		model = glm::rotate(model, glm::radians(angulosActualesDado.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angulosActualesDado.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angulosActualesDado.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Octo_T.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//Puerta
		//Pilar izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0f, 1.5f, 20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pilar_T.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pilar_M.RenderModel();
		//Puerta izquierda
		model = glm::mat4(1.0);
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.35f, -0.7f, 0.0f));
		model = glm::translate(model, glm::vec3(TpuertaIzq, 0.0f, TpuertaIzq_Z));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_T.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaIzq_M.RenderModel();
		//Pilar derecho
		model = glm::mat4(1.0);
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.75f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pilar_T.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pilar_M.RenderModel();
		//Puerta derecha
		model = glm::mat4(1.0);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.35f, -0.7f, 0.0f));
		model = glm::rotate(model, glm::radians(RpuertaDer), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_T.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaDer_M.RenderModel();
		//letrero
		model = glm::mat4(1.0);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.85f, 2.25f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Vector para el offset del letrero
		// Para el letrero con desplazamiento
		toffsetLetrero += velocidadLetrero * deltaTime;
		if (toffsetLetrero > 1.0f)
		{
			toffsetLetrero = 0.0f;
		}
		glm::vec2 letreroOffset = glm::vec2(toffsetLetrero, 0.0f);
		// Envio del offset al shader
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(letreroOffset));
		Letrero_T.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Letrero_M.RenderModel();
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		/*color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		
		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.00;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		// 
		
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		for (int i = 1; i<4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		 }

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}
 

		//número cambiante 
		//¿Cómo hacer para que sea a una velocidad visible?
		
		tiempoAcumuladoNumeros += deltaTime;
		if (tiempoAcumuladoNumeros >= tiempoPorNumero)
		{
			toffsetnumerocambiau += 0.25;
			tiempoAcumuladoNumeros = 0.0f;
			if (toffsetnumerocambiau > 1.0){
				toffsetnumerocambiau = 0.0;
			}
			contadorCambio++;
			if (contadorCambio >= 5)
			{
				mostrarTextura1 = !mostrarTextura1;
				contadorCambio = 0;
			}
		}
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		if (mostrarTextura1)
		{
			Numero1Texture.UseTexture();
			
		}
		else
		{
			Numero2Texture.UseTexture();
			
		}

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();




		glDisable(GL_BLEND);
		
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
