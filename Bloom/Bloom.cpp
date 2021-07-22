
#include "GLShader.hpp"
#include "string_cast.hpp"




// settings
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 800;



void SquareSpawner(std::vector<Species>& speciesList, std::vector<Species>& generatedSpecies, std::vector<TrailPixel>& trailBuffer)
{
    for (int x = 0; x < SCR_WIDTH; x++)
    {
        for (int y = 0; y < SCR_HEIGHT; y++)
        {

            unsigned int index = unsigned int(x * SCR_HEIGHT + y);
            trailBuffer[index].SpeciesID_evapSpeed_blendSpeed.z = -1;
            Species selectedSpecies;
            for (int sIndex = 0; sIndex < generatedSpecies.size(); sIndex++)
            {
                if (x >= sIndex * (SCR_WIDTH / generatedSpecies.size()) && x <= (sIndex + 1) * (SCR_WIDTH / generatedSpecies.size()))
                {
                    selectedSpecies = generatedSpecies[sIndex];
                    selectedSpecies.active_blendSize_sensorSize_speciesIndex.x = true;
                    trailBuffer[index].pixelColour = selectedSpecies.trailColour;
                    trailBuffer[index].SpeciesID_evapSpeed_blendSpeed.x = selectedSpecies.active_blendSize_sensorSize_speciesIndex.w;
                    trailBuffer[index].SpeciesID_evapSpeed_blendSpeed.z = selectedSpecies.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.y;
                    selectedSpecies.position = glm::vec4(x, y, 0, 0);
                    speciesList[index] = selectedSpecies;
                }
            }



        }
    }

}


void RingSpawner(std::vector<Species> &speciesList, std::vector<Species> &generatedSpecies, std::vector<TrailPixel> &trailBuffer )
{
    float raduis = 150;
    int h = SCR_HEIGHT / 2;
    int k = SCR_WIDTH / 2;
    int angleOffset = 360.0 / generatedSpecies.size();

    for (int x = 0; x < SCR_WIDTH; x++)
    {
        for (int y = 0; y < SCR_HEIGHT; y++)
        {
            //GET PIXEL INDEX
            unsigned int index = unsigned int(x * SCR_HEIGHT + y);
            
            //CREATE DEFAULT SPECIES 
            Species selectedSpecies;
            trailBuffer[index] = TrailPixel();
            //IF WITH IN THE CIRCLE
            double distance = pow(raduis, 2) - pow(x - h, 2) - pow(y - k, 2);
            if (distance >= 0.0)
            {
         
                for (int sIndex = 0; sIndex < generatedSpecies.size(); sIndex++)
                {
                    int angleToPoint = abs(atan2(y - h, x - k) * 180.0 / 3.141592654);
                    //if y < h angle becomes negative
                    if (y < h)
                        angleToPoint = angleToPoint+180;

                    if (angleToPoint >= (sIndex * angleOffset) && angleToPoint <= ((sIndex + 1) * angleOffset))
                    {
                        selectedSpecies = generatedSpecies[sIndex];
                        //activate species
                        selectedSpecies.active_blendSize_sensorSize_speciesIndex.x = 1;
                        //set trail from species params
                        trailBuffer[index].pixelColour = selectedSpecies.trailColour;
                        trailBuffer[index].SpeciesID_evapSpeed_blendSpeed.x = selectedSpecies.active_blendSize_sensorSize_speciesIndex.w;
                        trailBuffer[index].SpeciesID_evapSpeed_blendSpeed.z = selectedSpecies.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.y;
                        trailBuffer[index].SpeciesID_evapSpeed_blendSpeed.y = selectedSpecies.evapSpeed_blendSpeed_sensOffsetDist_senseAngleSpacing.x;
                        //ensure species has a position
                        selectedSpecies.position = glm::vec4(x, y, 0, 0);
                        speciesList[index] = selectedSpecies;
                    }
     
                }
                if (selectedSpecies.active_blendSize_sensorSize_speciesIndex.w == -1) {
                    std::cout << "UH OH" << std::endl;
                }



            }
  


        
        }
    }

 

}


void CreateBuffers( GLuint AgentBuffer, int speciesCount)
{
    //the number by which to skip when initializing agents



    SpeciesFactory factory = SpeciesFactory();
    std::vector<TrailPixel> trailPixels = std::vector <TrailPixel>(SCR_WIDTH * SCR_HEIGHT);
    std::vector<Species> speciesList = std::vector<Species>(SCR_WIDTH * SCR_HEIGHT);

    srand(time(NULL));
    std::cout << "####################################" << std::endl;



    std::vector<Species> generatedSpecies;
    for (int i = 0; i < speciesCount; i++)
    {
        generatedSpecies.push_back(*factory.GenerateSpeciesType());
        //generatedSpecies[i].angle_speed_turnSpeed.y = rand() % 3 == 0 ? 0 : generatedSpecies[i].angle_speed_turnSpeed.y;

        std::cout << "Species[" << int(i)+1;
        std::cout << "] colour: " + glm::to_string(generatedSpecies[i].speciesColour) << std::endl;
    }
    std::cout << speciesCount + " Species types"<< std::endl;
    std::cout << "####################################" << std::endl;

    RingSpawner(speciesList, generatedSpecies, trailPixels);

    //agents[400 * SCR_HEIGHT + 400].agentColour = glm::vec4(1, 0, 0, 1);
    //create agent buffer
    glGenBuffers(1, &AgentBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, AgentBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (SCR_WIDTH*SCR_HEIGHT) * sizeof(Species), speciesList.data(), GL_DYNAMIC_DRAW);

    
    void* agentWritePtr =  glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
    // now copy data into memory
    memcpy(agentWritePtr, &speciesList, sizeof(speciesList));
    // make sure to tell OpenGL we're done with the pointer
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, AgentBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 3);
    //create trailMap buffer

    GLuint trailBuffer;
    glGenBuffers(1, &trailBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, trailBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (SCR_WIDTH * SCR_HEIGHT) * sizeof(TrailPixel), trailPixels.data(), GL_DYNAMIC_DRAW);

    void* trailMapPtr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
    // now copy data into memory
    memcpy(agentWritePtr, &trailPixels, sizeof(trailPixels));
    // make sure to tell OpenGL we're done with the pointer
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, trailBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 4);
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    

    GLuint shaderProgram = LoadShader("C:\\Users\\Vaughan\\Documents\\NFTART\\Bloom\\CPP\\Bloom\\fragment.frag", "C:\\Users\\Vaughan\\Documents\\NFTART\\Bloom\\CPP\\Bloom\\vert.vert");
    GLuint computeProgram = LoadComputeShader("C:\\Users\\Vaughan\\Documents\\NFTART\\Bloom\\CPP\\Bloom\\PixelShader.glsl");
    GLuint evaporateProgram = LoadEvaporateComputeShader("C:\\Users\\Vaughan\\Documents\\NFTART\\Bloom\\CPP\\Bloom\\EvaporateCompute.glsl");


    unsigned int EBO = 0, VAO = 0, VBO = 0;
    GenerateVertexBuffer(&EBO, &VAO, &VBO);
    
    int speciesCount = (rand() % 2) + 1;
    speciesCount = 1;
    GLuint AgentBuffer = 0;
    CreateBuffers(AgentBuffer, speciesCount);


    //Gen a texture for compute shader to write into 
    GLuint outputTexture = GenerateTexture((int)SCR_HEIGHT, (int)SCR_WIDTH);
  
   
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
    // render loop
    // -----------

    float deltaTime = glfwGetTime();
    float total = 0.0;



    while (!glfwWindowShouldClose(window))
    {

        // input
        // -----
        processInput(window);

        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT);



   
        deltaTime = (glfwGetTime() - total);
        total += deltaTime;


        //evaporate trailMap
        GLint dtEv = glGetUniformLocation(evaporateProgram, "deltaTime");
        GLint speciesCountEV = glGetUniformLocation(evaporateProgram, "speciesCount");
        glUseProgram(evaporateProgram);
        glUniform1f(dtEv, deltaTime);
        glUniform1i(speciesCountEV, speciesCount);
        glDispatchCompute(SCR_WIDTH, SCR_HEIGHT, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        //evaporate trailMap

        GLint dt = glGetUniformLocation(computeProgram, "deltaTime");
        //compute particle sim
        glUseProgram(computeProgram);
        //std::cout << deltaTime << std::endl;
        glUniform1f(dt, deltaTime);

        glDispatchCompute(SCR_WIDTH, SCR_HEIGHT, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        //compute particle sim



        //use the shader program before draw
        glUseProgram(shaderProgram);
        //bind texture

        //draw vertex array
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        auto err = glGetError();
        if(err != 0)
        std::cout << err << std::endl; // returns 0 (no error)

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &outputTexture);
    glDeleteProgram(shaderProgram);

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void GenerateVertexBuffer(unsigned int * EBO, unsigned int * VAO, unsigned int * VBO) {
    // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
    const float vertices[] = {
        //vertex positions      texture coords
        1.0f,  1.0f, 0.0f,      1.0f, 1.0f, // top right 
        1.0f, -1.0f, 0.0f,      1.0f, 0.0f, // bottom right
       -1.0f, -1.0f, 0.0f,      0.0f, 0.0f, // bottom left
       -1.0f,  1.0f, 0.0f,      0.0f, 1.0f  // top left 
    };
    const unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*VAO);

    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex shader attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint GenerateTexture(int text_h, int text_w)
{

    //// The framebuffer for texture
    //GLuint textureFrameBuffer = 0;
    //glGenFramebuffers(1, &textureFrameBuffer);
    //glBindFramebuffer(GL_FRAMEBUFFER, textureFrameBuffer);

    // dimensions of the image
    GLuint tex_output;
    glGenTextures(1, &tex_output);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_output);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
    glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    if (tex_output < 0)
    {
        throw new std::exception("screenTexture not bound");
    }
    else {

  /*      const GLubyte* sExtensions = glGetString(GL_EXTENSIONS);
        for (size_t i = 0; sExtensions[i] ; i++)
        {
            std::cout << sExtensions[i];
            if(sExtensions[i] == ' ')
                std::cout << std::endl;
        }
        std::cout << std::endl;*/

        int work_grp_cnt[3];

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

        printf("max global (total) work group counts x:%i y:%i z:%i\n",
            work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

        int work_grp_size[3];

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

        printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
            work_grp_size[0], work_grp_size[1], work_grp_size[2]);


        GLint work_grp_inv;
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
        printf("max local work group invocations %i\n", work_grp_inv);
    }

    return tex_output;
}