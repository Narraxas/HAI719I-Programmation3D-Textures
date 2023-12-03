// Local includes
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Context.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>

Material::~Material() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
}

void Material::init() {
	// TODO : Change shader by your
	m_program_mesh = load_shaders("shaders/unlit/vertex.glsl", "shaders/unlit/fragment.glsl");
	m_program_skyBox = load_shaders("shaders/unlit/vertexSky.glsl", "shaders/unlit/fragmentSky.glsl");
	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
    light_position = {0.5, 1., 0.5};

    // m_texture = loadTexture2DFromFilePath("data/TwoSidedPlane/TwoSidedPlane_BaseColor.png");
    // m_normalMap = loadTexture2DFromFilePath("data/TwoSidedPlane/TwoSidedPlane_Normal.png");

    // m_texture = loadTexture2DFromFilePath("data/Mandarin/MandarinOrange_Basecolor.jpg");
    // m_normalMap = loadTexture2DFromFilePath("data/Mandarin/MandarinOrange_Normal.png");

    m_texture = loadTexture2DFromFilePath("data/WaterBottle/WaterBottle_baseColor.png");
    m_normalMap = loadTexture2DFromFilePath("data/WaterBottle/WaterBottle_normal.png");

    // --------------- Init Skybox -----------------

    float skyboxVertices[] = {
            -20.0f,  20.0f, -20.0f,
            -20.0f, -20.0f, -20.0f,
            20.0f, -20.0f, -20.0f,
            20.0f, -20.0f, -20.0f,
            20.0f,  20.0f, -20.0f,
            -20.0f,  20.0f, -20.0f,

            -20.0f, -20.0f,  20.0f,
            -20.0f, -20.0f, -20.0f,
            -20.0f,  20.0f, -20.0f,
            -20.0f,  20.0f, -20.0f,
            -20.0f,  20.0f,  20.0f,
            -20.0f, -20.0f,  20.0f,

            20.0f, -20.0f, -20.0f,
            20.0f, -20.0f,  20.0f,
            20.0f,  20.0f,  20.0f,
            20.0f,  20.0f,  20.0f,
            20.0f,  20.0f, -20.0f,
            20.0f, -20.0f, -20.0f,

            -20.0f, -20.0f,  20.0f,
            -20.0f,  20.0f,  20.0f,
            20.0f,  20.0f,  20.0f,
            20.0f,  20.0f,  20.0f,
            20.0f, -20.0f,  20.0f,
            -20.0f, -20.0f,  20.0f,

            -20.0f,  20.0f, -20.0f,
            20.0f,  20.0f, -20.0f,
            20.0f,  20.0f,  20.0f,
            20.0f,  20.0f,  20.0f,
            -20.0f,  20.0f,  20.0f,
            -20.0f,  20.0f, -20.0f,

            -20.0f, -20.0f, -20.0f,
            -20.0f, -20.0f,  20.0f,
            20.0f, -20.0f, -20.0f,
            20.0f, -20.0f, -20.0f,
            -20.0f, -20.0f,  20.0f,
            20.0f, -20.0f,  20.0f
    };

	// VBO_Skybox
    glGenBuffers(1, &VBO_Skybox);
    glGenVertexArrays(1, &VAO_Skybox);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Skybox);
    glBindVertexArray(VAO_Skybox);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    // Load Texture Skybox
    std::vector<std::string> skyboxTex= {
            "data/Skybox/right.jpg",
            "data/Skybox/left.jpg",
            "data/Skybox/top.jpg",
            "data/Skybox/bottom.jpg",
            "data/Skybox/front.jpg",
            "data/Skybox/back.jpg"
    };
    m_texture_sky = loadCubeMap(skyboxTex);
}

void Material::clear() {
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
}

void Material::bind(int idx) {
	check();
	if(idx == 0){
        m_program = m_program_skyBox;
    } else {
        m_program = m_program_mesh;
    }
	glUseProgram(m_program);
	internalBind(idx);
}

void Material::internalBind(int idx) {
	if(idx == 0){
		glDepthFunc(GL_LEQUAL);
        glBindVertexArray(VAO_Skybox);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_sky);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    } else {
		// bind parameters
		GLint color = getUniform("color");
		glUniform4fv(color, 1, glm::value_ptr(m_color));
		if (m_texture != -1) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glUniform1i(getUniform("colorTexture"), 0);
		}
		if (m_normalMap != -1) {
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, m_normalMap);
			glUniform1i(getUniform("normalTexture"), 1);
		}
		if (m_texture_sky != -1) {
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_sky);
			glUniform1i(getUniform("skybox"), 2);
		}
    }

	// TODO : Add your custom parameters here
    glUniform3fv(getUniform("viewPos"), 1, glm::value_ptr(Context::camera.position));
    glUniform3fv(getUniform("lightPos"), 1.0f , glm::value_ptr(light_position));
}

void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
}

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string& in_uniformName) {
	check();
	return glGetUniformLocation(m_program, in_uniformName.c_str());
}
