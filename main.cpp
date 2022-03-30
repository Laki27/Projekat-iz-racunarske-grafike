#include <iostream>
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Model.h>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouseCallBack(GLFWwindow *window, double xpos, double ypos);
unsigned int ucitajTexturu(const char *path);
unsigned int ucitajCubeMap(std::vector<std::string> faces);
void renderPodloga();

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp=glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(14.2f, 1.8f, 2.2);

glm::vec3 na_sceni_kutije_ambient=glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 na_sceni_kutije_specular = glm::vec3(1.0f);

glm::vec3 na_sceni_bandere_ambient=glm::vec3(0.2);
glm::vec3 na_sceni_bandere_diffuse=glm::vec3(1.0f, 0.75f, 0.55f);
glm::vec3 na_sceni_bandere_specular=glm::vec3(0.4);

glm::vec3 direkciono_difuzno = glm::vec3(0.75f, 0.6f, 0.65f);
glm::vec3 direkciono_specular = glm::vec3(1.0f);
glm::vec3 boja_ulicnog = glm::vec3(1.0f, 0.95f, 0.75f);

int noc=2;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX=400;
float lastY=300;
float yaw = -130;
float pitch=0.0f;

int main() {
    int glfwInitStatus=glfwInit();
    assert(glfwInitStatus==GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window=glfwCreateWindow(800, 600, "Hello", nullptr, nullptr);
    if(window== nullptr){
        std::cout<<"Failed to create a window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouseCallBack);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to init GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Shader kutije("../resources/shader/vertex1.vs", "../resources/shader/kutije.fs");
    Shader bandere("../resources/shader/vertex1.vs", "../resources/shader/bandere.fs");

    Shader drvo("../resources/shader/vertex1.vs", "../resources/shader/model.fs");
    Model drvece("../resources/object/Drvo/drvo.obj");

    Shader konj("../resources/shader/vertex1.vs", "../resources/shader/model.fs");
    Model konji("../resources/object/Konj/konj.obj");

    Shader sunce("../resources/shader/svetlost.vs", "../resources/shader/svetslost.fs");
    Shader ulicno("../resources/shader/svetlost.vs", "../resources/shader/svetslost.fs");
    Shader podloga("../resources/shader/podloga.vs", "../resources/shader/podloga.fs");
    Shader trava("../resources/shader/trava.vs", "../resources/shader/trava.fs");
    Shader skyboxShader("../resources/shader/skyBox.vs", "../resources/shader/skyBox.fs");

    float vertices[] {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f,1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,1.0f,0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f
    };
    float skyboxVertices[] ={
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
    glm::vec3 pozicijaKutija[]={
            glm::vec3( -4.0f,  -0.18f,  3.0f),
            glm::vec3( -2.0f,  -0.1f, -2.0f),
            glm::vec3(-5.5f, -0.15f, 1.5f),
            glm::vec3(-7.2f, -0.1f, -2.3f),
            glm::vec3( 0.4f, -0.27f, 1.5f),
            glm::vec3(-4.8f,0.13f, -25.2f),
            glm::vec3(-4.8f,0.13f, 9.2f),
            glm::vec3(-12.15f,0.13f, -20.0f),
            glm::vec3(-12.15f,0.13f, 4.1f),
            glm::vec3(-12.15f,0.13f, -7.95f),
            glm::vec3(2.54f,0.13f, -20.0f),
            glm::vec3(2.54f,0.13f, 4.1f)
    };
    glm::vec3 velicinaKutija[] = {
            glm::vec3 (1.0f, 1.0f, 1.0f),
            glm::vec3 (1.2f, 1.2f, 1.7f),
            glm::vec3 (1.3f, 1.1f, 1.0f),
            glm::vec3 (1.4f, 1.2f, 1.3f),
            glm::vec3 (1.1f, 0.8f, 0.9f),
            glm::vec3(15.0f,1.6f, 0.3f),
            glm::vec3(15.0f,1.6f, 0.3f),
            glm::vec3(0.3f,1.6f, 10.0f),
            glm::vec3(0.3f,1.6f, 10.0f),
            glm::vec3(0.3f,1.6f, 14.1f),
            glm::vec3(0.3f,1.6f, 10.0f),
            glm::vec3(0.3f,1.6f, 10.0f)
    };
    glm::vec3 pozicija_ulicno[] = {
            glm::vec3(4.0f, 3.0f, -25.0f),
            glm::vec3(7.0f, 3.0f, -13.67f),
            glm::vec3(7.0f, 3.0f, -1.34f),
            glm::vec3(4.0f, 3.0f, 9.0f)
    };
    glm::vec3 pozicija_drveca[]={
            glm::vec3(20.0f,-0.7f, 7.0f),
            glm::vec3(20.0f, -0.7f, -22.0f),
            glm::vec3(13.0f,-0.7f, -7.0f),
            glm::vec3(10.0f, -0.7f, -22.0f),
            glm::vec3(10.0f, -0.7f, 7.0f),
            glm::vec3(-9.0f, -0.7f, 4.0f),
            glm::vec3(-4.0f, -0.7f, -22.0f)
    };
    float rotacija_drveca[]={
            glm::radians(0.0f),
            glm::radians(0.0f),
            glm::radians(0.0f),
            glm::radians(90.0f),
            -glm::radians(90.0f),
            -glm::radians(180.0f),
            glm::radians(60.0f)
    };
    glm::vec3 pozicija_trave[]={
            glm::vec3(-9.6f, 0.8f, -22.2f),
            glm::vec3(2.0f, 0.8f, -24.2f),
            glm::vec3(4.7f,0.8f, -24.9f),
            glm::vec3(21.0f, 0.8f, -22.0f),
            glm::vec3(23.5f, 0.8f, -18.0),
            glm::vec3(23.5f, 0.8f, -10.0f),
            glm::vec3(23.5, 0.8, -2.0f),
            glm::vec3(18.8, 0.8, 6.0f),
            glm::vec3(5.0, 0.8, 7.0)
    };
    float rotacija_trave[]={
            glm::radians(45.0f),
            glm::radians(135.0f),
            glm::radians(180.0f),
            glm::radians(90.0f),
            glm::radians(90.0f),
            glm::radians(90.0f),
            glm::radians(90.0f),
            glm::radians(90.0f),
            glm::radians(180.0f)
    };

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0*sizeof(float)));

    unsigned VBO, kutijeVAO;
    glGenVertexArrays(1, &kutijeVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(kutijeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned bandereVAO;
    glGenVertexArrays(1, &bandereVAO);
    glBindVertexArray(bandereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned sunceVAO;
    glGenVertexArrays(1, &sunceVAO);
    glBindVertexArray(sunceVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned ulicnoVAO;
    glGenVertexArrays(1, &ulicnoVAO);
    glBindVertexArray(ulicnoVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned podlogaVAO;
    glGenVertexArrays(1, &podlogaVAO);
    glBindVertexArray(podlogaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)3);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned travaVAO;
    glGenVertexArrays(1, &travaVAO);
    glBindVertexArray(travaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int diffuseMapKutije = ucitajTexturu("../resources/texture/container2.png");
    unsigned int specularMapKutije = ucitajTexturu("../resources/texture/container2_specular.png");

    unsigned int diffuseMapPodloga = ucitajTexturu("../resources/texture/diffuseZemlja.jpg");
    unsigned int specularMapPodloga= ucitajTexturu("../resources/texture/specularZemlja.jpg");
    unsigned int normalMapPodloga= ucitajTexturu("../resources/texture/normalZemlja.jpg");

    unsigned int banderaMap = ucitajTexturu("../resources/texture/metal.png");

    unsigned int travaMap= ucitajTexturu("../resources/texture/grass.png");

    std::vector<std::string> faces {
        "../resources/skybox/stars-2129014.jpg",
        "../resources/skybox/stars-2129014.jpg",
        "../resources/skybox/stars-2129014.jpg",
        "../resources/skybox/stars-2129014.jpg",
        "../resources/skybox/stars-2129014.jpg",
        "../resources/skybox/stars-2129014.jpg"
    };

    unsigned int cubeMap=ucitajCubeMap(faces);

    skyboxShader.use();
    skyboxShader.setUniform1I("skybox", 0);

    kutije.use();
    kutije.setUniform1I("material.diffuse", 0);
    kutije.setUniform1I("material.specular", 1);

    podloga.use();
    podloga.setUniform1I("diffuseMap", 0);
    podloga.setUniform1I("normalMap", 1);
    podloga.setUniform1I("specularMap", 2);

    trava.use();
    trava.setUniform1I("trava", 0);

    bandere.use();
    bandere.setUniform1I("material.metal", 0);

    glm::mat4 model = glm::mat4(1.0f);

    while(!glfwWindowShouldClose(window)){

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800/600, 0.1f, 150.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);

        float currentFrame = glfwGetTime()-25.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        float visina_sunca=cos(currentFrame/8.0f);
        float korekcija=sin(currentFrame/8.0f);

        cameraPos.y=2.0f;
        if(cameraPos.x<-11.0f){
            cameraPos.x=-11.0f;
        }
        if(cameraPos.x>21.0f){
            cameraPos.x=21.0f;
        }
        if(cameraPos.z<-24.0f){
            cameraPos.z=-24.0f;
        }
        if(cameraPos.z>8.0f){
            cameraPos.z=8.0f;
        }

        glm::vec3 nebo = glm::vec3(visina_sunca*0.6+0.1, visina_sunca*0.75f+0.1, visina_sunca+0.1);
        if(nebo.y<0.0f){
            nebo.x=0.0f;
            nebo.y=0.0f;
            nebo.z=0.0f;
        }

        glClearColor(nebo.x, nebo.y, nebo.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        podloga.use();
        podloga.setMat4("projection", projection);
        podloga.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, -0.69f, -8.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(17.5f, 17.5f, 1.0f));
        podloga.setMat4("model", model);
        podloga.setVec3("viewPos", cameraPos);
        podloga.setVec3("direkciono.direction", glm::vec3(-16*korekcija, -80*visina_sunca, -80*korekcija));
        podloga.setVec3("direkciono.ambient", nebo);
        podloga.setVec3("direkciono.diffuse", direkciono_difuzno);
        podloga.setVec3("direkciono.specular", direkciono_specular);

        podloga.setVec3("na_sceni[0].ambient", na_sceni_kutije_ambient);
        podloga.setVec3("na_sceni[0].diffuse", na_sceni_kutije_ambient);
        podloga.setVec3("na_sceni[0].specular", na_sceni_kutije_specular);

        podloga.setVec3("na_sceni[1].ambient", na_sceni_kutije_ambient);
        podloga.setVec3("na_sceni[1].diffuse", na_sceni_kutije_ambient);
        podloga.setVec3("na_sceni[1].specular", na_sceni_kutije_specular);

        podloga.setVec3("na_sceni[2].ambient", na_sceni_kutije_ambient);
        podloga.setVec3("na_sceni[2].diffuse", na_sceni_kutije_ambient);
        podloga.setVec3("na_sceni[2].specular", na_sceni_kutije_specular);

        podloga.setVec3("na_sceni[3].ambient", na_sceni_kutije_ambient);
        podloga.setVec3("na_sceni[3].diffuse", na_sceni_kutije_ambient);
        podloga.setVec3("na_sceni[3].specular", na_sceni_kutije_specular);

        podloga.setVec3("lightPos0", pozicija_ulicno[0]);
        podloga.setVec3("lightPos1", pozicija_ulicno[1]);
        podloga.setVec3("lightPos2", pozicija_ulicno[2]);
        podloga.setVec3("lightPos3", pozicija_ulicno[3]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMapPodloga);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMapPodloga);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specularMapPodloga);
        renderPodloga();

        trava.use();
        trava.setMat4("projection", projection);
        trava.setMat4("view", view);
        trava.setVec3("viewPos", cameraPos);
        trava.setVec3("na_sceni[0]", pozicija_ulicno[0]);
        trava.setVec3("na_sceni[1]", pozicija_ulicno[1]);
        trava.setVec3("na_sceni[2]", pozicija_ulicno[2]);
        trava.setVec3("na_sceni[3]", pozicija_ulicno[3]);
        trava.setUniform1I("noc", noc);
        trava.setFloat("p", visina_sunca);

        glBindVertexArray(travaVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, travaMap);
        glDisable(GL_CULL_FACE);
        for(unsigned int i=0; i<9; i++){
            model=glm::mat4(1.0f);
            model = glm::translate(model, pozicija_trave[i]);
            model=glm::rotate(model, rotacija_trave[i], glm::vec3(0.0f, 1.0f, 0.0f));
            model=glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model=glm::scale(model, glm::vec3(3.0f, 4.0f, 3.0f));
            trava.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glEnable(GL_CULL_FACE);

        kutije.use();
        kutije.setMat4("projection", projection);
        kutije.setMat4("view", view);
        kutije.setVec3("direkciono.direction", glm::vec3(-16*korekcija, -80*visina_sunca, -80*korekcija));
        kutije.setVec3("direkciono.ambient", nebo);
        kutije.setVec3("direkciono.diffuse", direkciono_difuzno);
        kutije.setVec3("direkciono.specular", direkciono_specular);
        kutije.setVec3("viewPos", cameraPos);

        kutije.setVec3("na_sceni[0].position", pozicija_ulicno[0]);
        kutije.setVec3("na_sceni[0].ambient", na_sceni_kutije_ambient);
        kutije.setVec3("na_sceni[0].diffuse", na_sceni_kutije_ambient);
        kutije.setVec3("na_sceni[0].specular", na_sceni_kutije_specular);

        kutije.setVec3("na_sceni[1].position", pozicija_ulicno[1]);
        kutije.setVec3("na_sceni[1].ambient", na_sceni_kutije_ambient);
        kutije.setVec3("na_sceni[1].diffuse", na_sceni_kutije_ambient);
        kutije.setVec3("na_sceni[1].specular", na_sceni_kutije_specular);

        kutije.setVec3("na_sceni[2].position", pozicija_ulicno[2]);
        kutije.setVec3("na_sceni[2].ambient", na_sceni_kutije_ambient);
        kutije.setVec3("na_sceni[2].diffuse", na_sceni_kutije_ambient);
        kutije.setVec3("na_sceni[2].specular", na_sceni_kutije_specular);

        kutije.setVec3("na_sceni[3].position", pozicija_ulicno[3]);
        kutije.setVec3("na_sceni[3].ambient", na_sceni_kutije_ambient);
        kutije.setVec3("na_sceni[3].diffuse", na_sceni_kutije_ambient);
        kutije.setVec3("na_sceni[3].specular", na_sceni_kutije_specular);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMapKutije);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMapKutije);
        glBindVertexArray(kutijeVAO);

         for(unsigned int i=0; i<12; i++) {
             glm::mat4 model = glm::mat4(1.0f);
             model = glm::translate(model, pozicijaKutija[i]);
             model = glm::scale(model, velicinaKutija[i]);
             kutije.setMat4("model", model);
             glDrawArrays(GL_TRIANGLES, 0, 36);
         }

        bandere.use();
        bandere.setVec3("direkciono.direction", glm::vec3(-16*korekcija, -80*visina_sunca, -80*korekcija));
        bandere.setVec3("direkciono.ambient", nebo);
        bandere.setVec3("direkciono.diffuse", direkciono_difuzno);
        bandere.setVec3("direkciono.specular",  direkciono_specular);
        bandere.setVec3("viewPos", cameraPos);
        bandere.setVec3("na_sceni[0].position", pozicija_ulicno[0]);
        bandere.setVec3("na_sceni[0].ambient", na_sceni_bandere_ambient);
        bandere.setVec3("na_sceni[0].diffuse", na_sceni_bandere_diffuse);
        bandere.setVec3("na_sceni[0].specular", na_sceni_bandere_specular);

        bandere.setVec3("na_sceni[1].position", pozicija_ulicno[1]);
        bandere.setVec3("na_sceni[1].ambient", na_sceni_bandere_ambient);
        bandere.setVec3("na_sceni[1].diffuse", na_sceni_bandere_diffuse);
        bandere.setVec3("na_sceni[1].specular", na_sceni_bandere_specular);

        bandere.setVec3("na_sceni[2].position", pozicija_ulicno[2]);
        bandere.setVec3("na_sceni[2].ambient", na_sceni_bandere_ambient);
        bandere.setVec3("na_sceni[2].diffuse", na_sceni_bandere_diffuse);
        bandere.setVec3("na_sceni[2].specular", na_sceni_bandere_specular);

        bandere.setVec3("na_sceni[3].position", pozicija_ulicno[3]);
        bandere.setVec3("na_sceni[3].ambient", na_sceni_bandere_ambient);
        bandere.setVec3("na_sceni[3].diffuse", na_sceni_bandere_diffuse);
        bandere.setVec3("na_sceni[3].specular", na_sceni_bandere_specular);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, banderaMap);
        bandere.setMat4("projection", projection);
        bandere.setMat4("view", view);
        for(unsigned int i=0; i<4; i++){
            model = glm::mat4(1.0f);
            model=glm::translate(model, pozicija_ulicno[i]+glm::vec3(0.0f, -2.055f, 0.0f));
            model=glm::scale(model, glm::vec3(0.5f, 3.3f, 0.5f));
            bandere.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        ulicno.use();
        ulicno.setMat4("projection", projection);
        ulicno.setMat4("view", view);
        ulicno.setVec3("boja", boja_ulicnog);
        for(unsigned int i=0; i<4; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, pozicija_ulicno[i]);
            ulicno.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        sunce.use();
        sunce.setMat4("projection", projection);
        sunce.setMat4("view", view);
        sunce.setVec3("boja", glm::vec3(1.0f, 0.6f, 0.2f));

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(16*korekcija, 80*visina_sunca, 80*korekcija-20.0f));
        sunce.setMat4("model", model);
        glBindVertexArray(sunceVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        drvo.use();
        drvo.setMat4("projection", projection);
        drvo.setMat4("view", view);
        drvo.setVec3("viewPos", cameraPos);
        drvo.setVec3("direkciono.direction", glm::vec3(-16*korekcija, -80*visina_sunca, -80*korekcija));
        drvo.setVec3("direkciono.ambient", nebo);
        drvo.setVec3("direkciono.diffuse", direkciono_difuzno);
        drvo.setVec3("direkciono.specular",  direkciono_specular);

        drvo.setVec3("na_sceni[0].position", pozicija_ulicno[0]);
        drvo.setVec3("na_sceni[0].ambient", na_sceni_kutije_ambient);
        drvo.setVec3("na_sceni[0].diffuse", na_sceni_kutije_ambient);
        drvo.setVec3("na_sceni[0].specular", na_sceni_kutije_specular);
        drvo.setVec3("na_sceni[1].position", pozicija_ulicno[1]);
        drvo.setVec3("na_sceni[1].ambient", na_sceni_kutije_ambient);
        drvo.setVec3("na_sceni[1].diffuse", na_sceni_kutije_ambient);
        drvo.setVec3("na_sceni[1].specular", glm::vec3(0.4f));
        drvo.setVec3("na_sceni[2].position", pozicija_ulicno[2]);
        drvo.setVec3("na_sceni[2].ambient", na_sceni_kutije_ambient);
        drvo.setVec3("na_sceni[2].diffuse", na_sceni_kutije_ambient);
        drvo.setVec3("na_sceni[2].specular", na_sceni_kutije_specular);
        drvo.setVec3("na_sceni[3].position", pozicija_ulicno[3]);
        drvo.setVec3("na_sceni[3].ambient", na_sceni_kutije_ambient);
        drvo.setVec3("na_sceni[3].diffuse", na_sceni_kutije_ambient);
        drvo.setVec3("na_sceni[3].specular", na_sceni_kutije_specular);

        for(unsigned int i=0; i<7; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, pozicija_drveca[i]);
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, rotacija_drveca[i], glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
            drvo.setMat4("model", model);
            drvece.Draw(drvo);
        }

        konj.use();
        konj.setMat4("projection", projection);
        konj.setMat4("view", view);
        konj.setVec3("viewPos", cameraPos);
        konj.setVec3("direkciono.direction", glm::vec3(-16*korekcija, -80*visina_sunca, -80*korekcija));
        konj.setVec3("direkciono.ambient", nebo);
        konj.setVec3("direkciono.diffuse", direkciono_difuzno);
        konj.setVec3("direkciono.specular",  direkciono_specular);

        konj.setVec3("na_sceni[0].position", pozicija_ulicno[0]);
        konj.setVec3("na_sceni[0].ambient", na_sceni_kutije_ambient);
        konj.setVec3("na_sceni[0].diffuse", na_sceni_kutije_ambient);
        konj.setVec3("na_sceni[0].specular", na_sceni_kutije_specular);
        konj.setVec3("na_sceni[1].position", pozicija_ulicno[1]);
        konj.setVec3("na_sceni[1].ambient", na_sceni_kutije_ambient);
        konj.setVec3("na_sceni[1].diffuse", na_sceni_kutije_ambient);
        konj.setVec3("na_sceni[1].specular", glm::vec3(0.4f));
        konj.setVec3("na_sceni[2].position", pozicija_ulicno[2]);
        konj.setVec3("na_sceni[2].ambient", na_sceni_kutije_ambient);
        konj.setVec3("na_sceni[2].diffuse", na_sceni_kutije_ambient);
        konj.setVec3("na_sceni[2].specular", na_sceni_kutije_specular);
        konj.setVec3("na_sceni[3].position", pozicija_ulicno[3]);
        konj.setVec3("na_sceni[3].ambient", na_sceni_kutije_ambient);
        konj.setVec3("na_sceni[3].diffuse", na_sceni_kutije_ambient);
        konj.setVec3("na_sceni[3].specular", na_sceni_kutije_specular);

        model=glm::mat4(1.0f);
        model=glm::translate(model, glm::vec3(-3.0f, -0.7f, -13.0f));
        model=glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model=glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model=glm::scale(model, glm::vec3(0.0023f, 0.0023f, 0.0023f));
        konj.setMat4("model", model);
        konji.Draw(konj);

        if(80*visina_sunca<1.0f){
            na_sceni_kutije_ambient=glm::vec3(1.0f, 0.65f, 0.25f);
            na_sceni_kutije_specular=glm::vec3(0.8f);
            boja_ulicnog=glm::vec3(1.0f, 0.7f, 0.3f);

            na_sceni_bandere_ambient=glm::vec3(0.4);
            na_sceni_bandere_diffuse=glm::vec3(1.0f, 0.95f, 0.75f);
            na_sceni_bandere_specular=glm::vec3(0.7);

            direkciono_difuzno=glm::vec3(0.0f, 0.0f, 0.0f);
            direkciono_specular=glm::vec3(0.0f);
            noc=1;

            glDepthFunc(GL_LEQUAL);
            skyboxShader.use();
            skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
            skyboxShader.setMat4("projection", projection);
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        }
        else{
            na_sceni_kutije_ambient=glm::vec3 (0.0f, 0.0f, 0.0f);
            na_sceni_kutije_specular=glm::vec3(0.0f);
            boja_ulicnog=glm::vec3(0.85f, 0.8f, 0.55f);

            na_sceni_bandere_ambient=glm::vec3(0.0f);
            na_sceni_bandere_diffuse=glm::vec3(0.0f);
            na_sceni_bandere_specular=glm::vec3(0.0f);

            direkciono_difuzno=glm::vec3(0.75f, 0.6f, 0.65f);
            direkciono_specular=glm::vec3(0.5f);
            noc=2;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &kutijeVAO);
    glDeleteVertexArrays(1, &sunceVAO);
    glDeleteVertexArrays(1, &bandereVAO);
    glDeleteVertexArrays(1, &ulicnoVAO);
    glDeleteVertexArrays(1, &podlogaVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &VBO);

    kutije.deleteProgram();
    podloga.deleteProgram();
    bandere.deleteProgram();
    ulicno.deleteProgram();
    sunce.deleteProgram();
    konj.deleteProgram();
    trava.deleteProgram();
    drvo.deleteProgram();
    glfwTerminate();
    return 0;
}

void mouseCallBack(GLFWwindow *window, double xpos, double ypos){
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sens=0.2f;
    xoffset*=sens;
    yoffset*=sens;
    yaw += xoffset;
    pitch +=yoffset;

    pitch = std::min(pitch, 79.0f);
    pitch = std::max(pitch, -79.0f);

    glm::vec3 front;
    front.x=cos(glm::radians(yaw))*cos(glm::radians(pitch));
    front.y=sin(glm::radians(pitch));
    front.z=sin(glm::radians(yaw))*cos(glm::radians(pitch));
    cameraFront=glm::normalize(front);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = 4 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

unsigned int ucitajTexturu(const char *path){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
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

unsigned int ucitajCubeMap(std::vector<std::string> faces){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderPodloga(){
    if (quadVAO == 0)
    {

        glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
        glm::vec3 pos4( 1.0f,  1.0f, 0.0f);

        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);

        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;

        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


        float quadVertices[] = {
                pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
                pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
                pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

                pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
                pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
                pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
