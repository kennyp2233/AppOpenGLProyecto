#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float spotlightOn = 0.0f;
float lightChanger = 0.0f;
float farolLight = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Aiden Castro", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram C:\Users\Dan\OneDrive\Desktop\proyecto
// ------------------------------------
	Shader lightingShader("shaders/vertexShader.vs", "shaders/fragmentShader.fs");
	//Shader skyboxShader("C:/Users/Alpha/Documents/Visual Studio 2022/OpenGL/OpenGl/shaders/skyVertexShader.vs", "C:/Users/Alpha/Documents/Visual Studio 2022/OpenGL/OpenGL/shaders/skyFragmentShader.fs");

	Model land("model/land/land.obj");
	Model floor("model/sky/sky.obj");
	Model monster("model/monster/monster.obj");
	Model center("model/motel/SH_BUILDING_13.obj");
	Model lamp("model/lamp/lamp.obj");
	Model building("model/building/SH_BUILDING_15.obj");
	Model piso("model/building/piso.obj");
	Model postes("model/building/postes.obj");

	
	// shader configuration
	// --------------------
	lightingShader.use();

	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 30);

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(17.0f, 5.0f, 10.0f), // motel
		glm::vec3(-2.8f, -0.1f, 0.4f),//monster
		glm::vec3(0.0f, 20.0f, 0.0f), // moon

		glm::vec3(32.8395f, 4.89446f, -36.886f), //poste luz 1
		glm::vec3(37.776f, 4.89446f, -26.2997f), //poste luz 2 
		glm::vec3(44.3784f, 4.89446f, -12.1407f), //poste luz 3

		glm::vec3(-3.88431f, 2.26788f,-1.72053f), //granja luz 1
		glm::vec3(-5.96746f, 2.26788f, -0.749144f), //granja luz 2
		glm::vec3(-8.08631f, 2.26788f, 0.238892f), //granja luz 3
		glm::vec3(-10.0249f, 2.26788f, 1.14286f), //granja luz 4

		glm::vec3(-3.88431f, 1.15782f, -1.72053f), //granja luz 5
		glm::vec3(-5.96746f, 1.15782f, -0.749144f), //granja luz 6
		glm::vec3(-8.08631f, 1.15782f, 0.238892f), //granja luz 7
		glm::vec3(-10.0249f, 1.15782f, 1.14286f), //granja luz 8

		glm::vec3(-5.24815f, 2.26788f, -4.64531f), //granja luz 9
		glm::vec3(-7.33131f, 2.26788f, -3.67392f), //granja luz 10
		glm::vec3(-9.45016f, 2.26788f, -2.68588f), //granja luz 11
		glm::vec3(-11.3887f, 2.26788f, -1.78191), //granja luz 12

		glm::vec3(-5.24815f, 1.15782f, -4.64531f), //granja luz 13
		glm::vec3(-7.33131f, 1.15782f, -3.67392f), //granja luz 14
		glm::vec3(-9.45016f, 1.15782f, -2.68588f), //granja luz 15
		glm::vec3(-11.3887f, 1.15782f, -1.78191), //granja luz 16


	};


	glm::vec3 pointLightColors[] = {
	  glm::vec3(0.6f, 0.1f, 0.9f),
	  glm::vec3(0.6f, 0.1f, 0.9f), //morado
	  glm::vec3(1.0f, 1.0f, 0.0f), //amarillo 

	};


	// render loop
	// -----------
	glm::vec3 pos, col;
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();


		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 70.0f);


		//directional light: ambient light
		lightingShader.setVec3("dirLight.direction", 0.1f, 1.0f, 0.2f);
		lightingShader.setVec3("pointLights[0].ambient", 20.0, 20.0, 20.0);
		lightingShader.setVec3("dirLight.diffuse", 0.28f, 0.28f, 0.28f);
		lightingShader.setVec3("dirLight.specular", 0.28f, 0.28f, 0.28f);

		/*----------------------------PointLights----------------------------*/
		pos = pointLightPositions[0];
		col = pointLightColors[0];
		// point light 0: Motel
		lightingShader.setVec3("pointLights[0].position", pos);
		lightingShader.setVec3("pointLights[0].ambient", col.x * 0.16, col.y * 0.15, col.z * 0.16);
		lightingShader.setVec3("pointLights[0].diffuse", glm::vec3(farolLight));
		lightingShader.setVec3("pointLights[2].specular", glm::vec3(farolLight));
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.18);
		lightingShader.setFloat("pointLights[0].quadratic", 0.07);

		// point light 1: water canal
		pos = pointLightPositions[1];
		col = pointLightColors[1];
		lightingShader.setVec3("pointLights[1].position",pos);
		lightingShader.setVec3("pointLights[1].ambient", col.x * 0.15, col.y * 0.15, col.z * 0.15);
		lightingShader.setVec3("pointLights[1].diffuse", lightChanger + 0.6f, lightChanger + 0.4f, 0.0f);
		lightingShader.setVec3("pointLights[1].specular", lightChanger + 0.6f, lightChanger + 0.4f, 0.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.35);
		lightingShader.setFloat("pointLights[1].quadratic", 0.44);

		//point light 2 - moon
		pos = pointLightPositions[2];
		col = pointLightColors[0];
		lightingShader.setVec3("pointLights[2].position", pos);
		lightingShader.setVec3("pointLights[2].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[2].diffuse", col.x + 0.7f, col.y + 0.7f, col.z + 0.7f);
		lightingShader.setVec3("pointLights[2].specular", col.x + 0.7f, col.y + 0.7f, col.z + 0.7f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032);

		
		// poste1
		pos = pointLightPositions[3];
		col = pointLightColors[2];
		lightingShader.setVec3("pointLights[3].position", pos);
		lightingShader.setVec3("pointLights[3].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[3].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[3].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.07);
		lightingShader.setFloat("pointLights[3].quadratic", 0.017);
;
		// poste2
		pos = pointLightPositions[4];
		lightingShader.setVec3("pointLights[4].position", pos);
		lightingShader.setVec3("pointLights[4].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[4].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[4].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[4].constant", 1.0f);
		lightingShader.setFloat("pointLights[4].linear", 0.07);
		lightingShader.setFloat("pointLights[4].quadratic", 0.017);


		// poste3
		pos = pointLightPositions[5];
		lightingShader.setVec3("pointLights[5].position", pos);
		lightingShader.setVec3("pointLights[5].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[5].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[5].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[5].constant", 1.0f);
		lightingShader.setFloat("pointLights[5].linear", 0.07);
		lightingShader.setFloat("pointLights[5].quadratic", 0.017);

		// GRANJA 1
		col = pointLightColors[0];
		pos = pointLightPositions[6];
		lightingShader.setVec3("pointLights[6].position", pos);
		lightingShader.setVec3("pointLights[6].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[6].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[6].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[6].constant", 1.0f);
		lightingShader.setFloat("pointLights[6].linear", 0.07);
		lightingShader.setFloat("pointLights[6].quadratic", 0.017);

		// GRANJA 2
		pos = pointLightPositions[7];
		lightingShader.setVec3("pointLights[7].position", pos);
		lightingShader.setVec3("pointLights[7].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[7].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[7].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[7].constant", 1.0f);
		lightingShader.setFloat("pointLights[7].linear", 0.07);
		lightingShader.setFloat("pointLights[7].quadratic", 0.017);

		// GRANJA 3
		pos = pointLightPositions[8];
		lightingShader.setVec3("pointLights[8].position", pos);
		lightingShader.setVec3("pointLights[8].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[8].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[8].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[8].constant", 1.0f);
		lightingShader.setFloat("pointLights[8].linear", 0.07);
		lightingShader.setFloat("pointLights[8].quadratic", 0.017);

		// GRANJA 4
		pos = pointLightPositions[9];
		lightingShader.setVec3("pointLights[9].position", pos);
		lightingShader.setVec3("pointLights[9].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[9].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[9].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[9].constant", 1.0f);
		lightingShader.setFloat("pointLights[9].linear", 0.07);
		lightingShader.setFloat("pointLights[9].quadratic", 0.017);

		// GRANJA 5
		pos = pointLightPositions[10];
		lightingShader.setVec3("pointLights[10].position", pos);
		lightingShader.setVec3("pointLights[10].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[10].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[10].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[10].constant", 1.0f);
		lightingShader.setFloat("pointLights[10].linear", 0.07);
		lightingShader.setFloat("pointLights[10].quadratic", 0.017);

		// GRANJA 6
		pos = pointLightPositions[11];
		lightingShader.setVec3("pointLights[11].position", pos);
		lightingShader.setVec3("pointLights[11].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[11].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[11].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[11].constant", 1.0f);
		lightingShader.setFloat("pointLights[11].linear", 0.07);
		lightingShader.setFloat("pointLights[11].quadratic", 0.017);

		// GRANJA 7
		pos = pointLightPositions[12];
		lightingShader.setVec3("pointLights[12].position", pos);
		lightingShader.setVec3("pointLights[12].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[12].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[12].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[12].constant", 1.0f);
		lightingShader.setFloat("pointLights[12].linear", 0.07);
		lightingShader.setFloat("pointLights[12].quadratic", 0.017);

		// GRANJA 8
		pos = pointLightPositions[13];
		lightingShader.setVec3("pointLights[13].position", pos);
		lightingShader.setVec3("pointLights[13].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[13].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[13].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[13].constant", 1.0f);
		lightingShader.setFloat("pointLights[13].linear", 0.07);
		lightingShader.setFloat("pointLights[13].quadratic", 0.017);

		// GRANJA 9
		pos = pointLightPositions[14];
		lightingShader.setVec3("pointLights[14].position", pos);
		lightingShader.setVec3("pointLights[14].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[14].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[14].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[14].constant", 1.0f);
		lightingShader.setFloat("pointLights[14].linear", 0.07);
		lightingShader.setFloat("pointLights[14].quadratic", 0.017);

		// GRANJA 10
		pos = pointLightPositions[15];
		lightingShader.setVec3("pointLights[15].position", pos);
		lightingShader.setVec3("pointLights[15].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[15].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[15].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[15].constant", 1.0f);
		lightingShader.setFloat("pointLights[15].linear", 0.07);
		lightingShader.setFloat("pointLights[15].quadratic", 0.017);

		// GRANJA 11
		pos = pointLightPositions[16];
		lightingShader.setVec3("pointLights[16].position", pos);
		lightingShader.setVec3("pointLights[16].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[16].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[16].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[16].constant", 1.0f);
		lightingShader.setFloat("pointLights[16].linear", 0.07);
		lightingShader.setFloat("pointLights[16].quadratic", 0.017);

		// GRANJA 12
		pos = pointLightPositions[17];
		lightingShader.setVec3("pointLights[17].position", pos);
		lightingShader.setVec3("pointLights[17].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[17].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[17].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[17].constant", 1.0f);
		lightingShader.setFloat("pointLights[17].linear", 0.07);
		lightingShader.setFloat("pointLights[17].quadratic", 0.017);

		// GRANJA 13
		pos = pointLightPositions[18];
		lightingShader.setVec3("pointLights[18].position", pos);
		lightingShader.setVec3("pointLights[18].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[18].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[18].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[18].constant", 1.0f);
		lightingShader.setFloat("pointLights[18].linear", 0.07);
		lightingShader.setFloat("pointLights[18].quadratic", 0.017);

		// GRANJA 14
		pos = pointLightPositions[19];
		lightingShader.setVec3("pointLights[19].position", pos);
		lightingShader.setVec3("pointLights[19].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[19].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[19].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[19].constant", 1.0f);
		lightingShader.setFloat("pointLights[19].linear", 0.07);
		lightingShader.setFloat("pointLights[19].quadratic", 0.017);

		// GRANJA 15
		pos = pointLightPositions[20];
		lightingShader.setVec3("pointLights[20].position", pos);
		lightingShader.setVec3("pointLights[20].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[20].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[20].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[20].constant", 1.0f);
		lightingShader.setFloat("pointLights[20].linear", 0.07);
		lightingShader.setFloat("pointLights[20].quadratic", 0.017);

		// GRANJA 16
		pos = pointLightPositions[21];
		lightingShader.setVec3("pointLights[21].position", pos);
		lightingShader.setVec3("pointLights[21].ambient", col.x * 0.08, col.y * 0.08, col.z * 0.08);
		lightingShader.setVec3("pointLights[21].diffuse", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setVec3("pointLights[21].specular", col.x + 0.2f, col.y + 0.2f, col.z + 0.2f);
		lightingShader.setFloat("pointLights[21].constant", 1.0f);
		lightingShader.setFloat("pointLights[21].linear", 0.07);
		lightingShader.setFloat("pointLights[21].quadratic", 0.017);

		

		/*----------------------------SpotLights----------------------------*/
		// spotLight: lantern
		lightingShader.setVec3("spotLights[0].position", camera.Position.x, camera.Position.y, camera.Position.z);
		lightingShader.setVec3("spotLights[0].direction", camera.Front.x, camera.Front.y, camera.Front.z);
		lightingShader.setVec3("spotLights[0].ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLights[0].diffuse", glm::vec3(spotlightOn));
		lightingShader.setVec3("spotLights[0].specular", glm::vec3(spotlightOn));
		lightingShader.setFloat("spotLights[0].constant", 0.8f);
		lightingShader.setFloat("spotLights[0].linear", 0.09);
		lightingShader.setFloat("spotLights[0].quadratic", 0.032);
		lightingShader.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(10.0f)));
		lightingShader.setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(15.0f)));
		// desde 3
		// spotLight: Postes1 
		pos = pointLightPositions[3];
		lightingShader.setVec3("spotLights[1].position", pos.x, pos.y, pos.z);
		lightingShader.setVec3("spotLights[1].direction", 0.0f, -1.0f, 0.0f);
		lightingShader.setVec3("spotLights[1].ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLights[1].diffuse", glm::vec3(1.0f));
		lightingShader.setVec3("spotLights[1].specular", glm::vec3(1.0f));
		lightingShader.setFloat("spotLights[1].constant", 0.8f);
		lightingShader.setFloat("spotLights[1].linear", 0.09);
		lightingShader.setFloat("spotLights[1].quadratic", 0.032);
		lightingShader.setFloat("spotLights[1].cutOff", glm::cos(glm::radians(10.0f)));
		lightingShader.setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(15.0f)));
		// spotLight: Postes2
		pos = pointLightPositions[4];
		lightingShader.setVec3("spotLights[2].position", pos.x, pos.y, pos.z);
		lightingShader.setVec3("spotLights[2].direction", 0.0f, -1.0f, 0.0f);
		lightingShader.setVec3("spotLights[2].ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLights[2].diffuse", glm::vec3(1.0f));
		lightingShader.setVec3("spotLights[2].specular", glm::vec3(1.0f));
		lightingShader.setFloat("spotLights[2].constant", 0.8f);
		lightingShader.setFloat("spotLights[2].linear", 0.09);
		lightingShader.setFloat("spotLights[2].quadratic", 0.032);
		lightingShader.setFloat("spotLights[2].cutOff", glm::cos(glm::radians(10.0f)));
		lightingShader.setFloat("spotLights[2].outerCutOff", glm::cos(glm::radians(15.0f)));
		// spotLight: Postes2
		pos = pointLightPositions[5];
		lightingShader.setVec3("spotLights[3].position", pos.x, pos.y, pos.z);
		lightingShader.setVec3("spotLights[3].direction", 0.0f, -1.0f, 0.0f);
		lightingShader.setVec3("spotLights[3].ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLights[3].diffuse", glm::vec3(1.0f));
		lightingShader.setVec3("spotLights[3].specular", glm::vec3(1.0f));
		lightingShader.setFloat("spotLights[3].constant", 0.8f);
		lightingShader.setFloat("spotLights[3].linear", 0.09);
		lightingShader.setFloat("spotLights[3].quadratic", 0.032);
		lightingShader.setFloat("spotLights[3].cutOff", glm::cos(glm::radians(10.0f)));
		lightingShader.setFloat("spotLights[3].outerCutOff", glm::cos(glm::radians(15.0f)));


		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);


		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);



		model = glm::mat4(1.0f);

		float changeCoord = sin(glfwGetTime());
		float changeCoord2 = cos(glfwGetTime());
		float angle = glfwGetTime() * 25.0f;


		// RENDER FABRIC
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.3f, -0.1f, 1.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		land.Draw(lightingShader);


		// RENDER MONSTER

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.8f, 1.1f, 2.4f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		monster.Draw(lightingShader);

		// RENDER MOTEL
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(17.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(115.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		center.Draw(lightingShader);

		// RENDER floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);;
		floor.Draw(lightingShader);

		// RENDER building

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(47.934f, 0.0f, -26.135f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		building.Draw(lightingShader);

		// PISO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(26.6119f, -0.1f, -52.1805f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		piso.Draw(lightingShader);


		// RENDER POSTES

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(46.0f, 0.0f, -13.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		postes.Draw(lightingShader);


		// RENDER TREVENANT
		float timeValue = glfwGetTime() * 3;
		float dinamicValue1 = cos(timeValue) * 4.0f / 1000;
		if (dinamicValue1 < 0.003) {
			dinamicValue1 = 0.003;
		}

		// RENDER GASTLY

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-40.0f, changeCoord * 1.3f + 3.0f, changeCoord2 * -20.0f));
		//model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		//lightingShader.setMat4("model", model);
		//gastly.Draw(lightingShader);

		// RENDER SKY
		////model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 00.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));;
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//sky.Draw(lightingShader);

		float dinamic = cos(timeValue) * 0.5f;
		if (dinamic < 0) {
			dinamic = dinamic * -1.0f;
		}
		lightChanger = dinamic;

		int randNum = rand() % (500 - 0 + 1) + 0;

		if (randNum >= 0 && randNum <= 10) {
			farolLight = 0.0f;
		}
		if (randNum >= 30 && randNum <= 50) {
			farolLight = pointLightColors[0].x + 0.2f;

		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Ajusta la velocidad de la cámara dependiendo de si "Shift" está presionada o no
	float cameraSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 3.0f * deltaTime : deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, cameraSpeed);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (spotlightOn == 0.0f) {
			spotlightOn = 1.0f;
		}
		else {
			spotlightOn = 0.0f;
		}

	}
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//Exercise 14 Task 2
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}