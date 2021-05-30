#include "CaseTessellationOnShader.h"
#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

#include "stb_image.h"
#include "TRSUtils.h"
#include "TRSResource.h"
#include "TRSWindowConfig.h"
#include "TRSCamera.h"
#include "TRSCallBackFunc.h"
#include "TRSConst.h"
#include "BSpline.h"
#include "BSplineSurface.h"

int CaseTessellationOnShader()
{
    float vertices[] = {
        -0.8f, 0.0f, 0.0f,
        -0.4f, 0.0f, 0.0f,
        -0.4f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f
    };
    float tangent[] = {
        0.5f, 0.866f, 0.0f,
        0.5f, 0.866f, 0.0f
    };
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, g_OpenGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, g_OpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(DefaultWindowWidth, DefaultWindowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    char* vertexShaderSource = readTextFile("shaders/hermite_curve_vert.glsl");
    char* fragmentShaderSource = readTextFile("shaders/hermite_curve_frag.glsl");
    char* tessControlShaderSource = readTextFile("shaders/hermite_curve_tess.glsl");
    char* tessEvaluateShaderSource = readTextFile("shaders/hermite_curve_eval.glsl");

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int tessControlShader;
    tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tessControlShader, 1, &tessControlShaderSource, NULL);
    glCompileShader(tessControlShader);
    glGetShaderiv(tessControlShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(tessControlShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::TESS_CONTROL::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int tessEvaluateShader;
    tessEvaluateShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tessEvaluateShader, 1, &tessEvaluateShaderSource, NULL);
    glCompileShader(tessEvaluateShader);
    glGetShaderiv(tessEvaluateShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(tessEvaluateShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::TESS_EVALUATE::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, tessControlShader);
    glAttachShader(shaderProgram, tessEvaluateShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);//bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);//unbind VAO

    GLuint locTan0 = glGetUniformLocation(shaderProgram, "vTan0");
    GLuint locTan1 = glGetUniformLocation(shaderProgram, "vTan1");
    glUniform3fv(locTan0, 1, &tangent[0]);
    glUniform3fv(locTan1, 1, &tangent[3]);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);

        glPatchParameteri(GL_PATCH_VERTICES, 2);
        glDrawArrays(GL_PATCHES, 0, 4);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

#include <iostream>
#include "TRSViewer.h"
#include "TRSGeode.h"
#include "TRSGroup.h"
#include "TRSTexture.h"
#include "TRSVAO.h"
#include "TRSResource.h"
#include "TRSConst.h"
#include "TRSCamera.h"
#include "TRSShader.h"
#include "TRSStateset.h"
#include "TRSUtils.h"
#include "TRSAssimpLoader.h"

int CaseTessHermiteCurve(int argn, char** argc)
{
    float vertices[] = {
        -0.8f, 0.0f, 0.0f,
        -0.4f, 0.0f, 0.0f,
        -0.4f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };
    float tangent[] = {
        0.5f, 0.866f, 0.0f,
        0.5f, 0.866f, 0.0f
    };

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    pNode->readFromVertex(vertices, sizeof(vertices) / sizeof(float), EnVertex);
    std::shared_ptr<TRSStateSet> pSS = pNode->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/DefaultVertexWithoutMVP.glsl");
    shader->createFragmentShader("shaders/DefaultFragment.glsl");
    shader->createTessControlShader("shaders/HermiteTesc.glsl");
    shader->createTessEvaluateShader("shaders/HermiteTese.glsl");
    shader->createProgram();

    shader->addUniform3v("vTan0", glm::vec3(0.5f, 0.866f, 0.0f));
    shader->addUniform3v("vTan1", glm::vec3(0.5f, 0.866f, 0.0f));
    pNode->getVAO()->setDrawType(GL_PATCHES);
    pNode->getVAO()->setDrawParam(2);

    viewer->setSecenNode(pNode);
    viewer->run();
    return 0;
}

int CaseTessBezierCurve(int argn, char** argc)
{
    float vertices[] = {
        -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f
    };
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();

    std::shared_ptr<TRSGeode> bezierCurve = std::make_shared<TRSGeode>();
    bezierCurve->readFromVertex(vertices, sizeof(vertices) / sizeof(float), EnVertex);
    std::shared_ptr<TRSStateSet> pSS = bezierCurve->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/DefaultVertexWithoutMVP.glsl");
    shader->createFragmentShader("shaders/DefaultFragment.glsl");
    shader->createTessControlShader("shaders/BezierTesc.glsl");
    shader->createTessEvaluateShader("shaders/BezierTese.glsl");
    shader->createProgram();
    bezierCurve->getVAO()->setDrawType(GL_PATCHES);
    bezierCurve->getVAO()->setDrawParam(4);
    bezierCurve->setColor(glm::vec4(1, 0.5, 0.5, 1));

    int arraySize;
    float* grid = createXYGridVertexArray(0.5, 7, arraySize);
    std::shared_ptr<TRSGeode> gridLine = std::make_shared<TRSGeode>();
    gridLine->readFromVertex(grid, arraySize, EnVertex);
    gridLine->getVAO()->setDrawType(GL_LINES);
    gridLine->setColor(glm::vec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> polyLine = std::make_shared<TRSGeode>();
    polyLine->readFromVertex(vertices, sizeof(vertices) / sizeof(float), EnVertex);
    polyLine->getVAO()->setDrawType(GL_LINE_STRIP);
    polyLine->setColor(glm::vec4(1, 1, 1, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(gridLine);
    rootNodes->addChild(polyLine);
    rootNodes->addChild(bezierCurve);

    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
}

int CaseTessBezierSurface(int argn, char** argc)
{
    // 4*4 Bezier surface control pts
    float vertices[] = {
        0.0,  0.0,  0.0,
        1.0,  0.0,  -1.0,
        2.0,  0.0,  -1.0,
        3.0,  0.0,  0.0,
        0.0,  1.0,  2.0,
        1.0,  1.0,  0.0,
        2.0,  1.0,  0.0,
        3.0,  1.0,  2.0,
        0.0,  2.0,  2.0,
        1.0,  2.0,  0.0,
        2.0,  2.0,  0.0,
        3.0,  2.0,  2.0,
        0.0,  3.0,  0.0,
        1.0,  3.0,  -1.0,
        2.0,  3.0,  -1.0,
        3.0,  3.0,  0.0,
    };
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();

    std::shared_ptr<TRSGeode> bezierSurface = std::make_shared<TRSGeode>();
    bezierSurface->readFromVertex(vertices, sizeof(vertices) / sizeof(float), EnVertex);
    std::shared_ptr<TRSStateSet> pSS = bezierSurface->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/DefaultVertexWithoutMVP.glsl");
    shader->createFragmentShader("shaders/PhongFragment.glsl");
    shader->createTessControlShader("shaders/BezierSurFaceTesc.glsl");
    shader->createTessEvaluateShader("shaders/BezierSurFaceTese.glsl");
    shader->createProgram();
    bezierSurface->getVAO()->setDrawType(GL_PATCHES);
    bezierSurface->getVAO()->setDrawParam(16);
    bezierSurface->setColor(glm::vec4(1, 0.5, 0.5, 1));

    int arraySize;
    float* grid = createXYGridVertexArray(0.5, 7, arraySize);
    std::shared_ptr<TRSGeode> gridLine = std::make_shared<TRSGeode>();
    gridLine->readFromVertex(grid, arraySize, EnVertex);
    gridLine->getVAO()->setDrawType(GL_LINES);
    gridLine->setColor(glm::vec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> bezierSurfaceWireframe = std::make_shared<TRSGeode>(*bezierSurface, true);
    bezierSurfaceWireframe->setPolygonMode(GL_LINE);
    bezierSurfaceWireframe->setColor(glm::vec4(0.5, 1.0, 0.5, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(gridLine);
    rootNodes->addChild(bezierSurface);
    rootNodes->addChild(bezierSurfaceWireframe);

    viewer->setSecenNode(rootNodes);
    viewer->run();

    return 0;
}


int CaseTessBSplineCurve(int argn, char** argc)
{
    float vertexBigDipper[] = {
        -2.4,   1.0,    0.0,
        -1.3,   1.0,    0.0,
        -0.75,  0.7,   0.0,
         0.0,   0.0,    0.0,
         0.0,  -0.75,   0.0,
         1.28, -1.08,   0.0,
         1.68, -0.27,   0.0
    };
    //unsigned int elementBigDipper[] = {
    //    0,1,2,3,
    //    1,2,3,4,
    //    2,3,4,5,
    //    3,4,5,6
    //};
    BSpline* bs = new BSpline;
    bs->setCtrlPts(vertexBigDipper, 6);
    int num = 100;
    float* curve = new float[3 * num + 3];
    int sampleNum = (num / 10 + 1);
    float* samplePtPair = new float[6 * sampleNum];
    float* curPair = samplePtPair;
    float* pt = curve;
    float norm[3]; //normal of any point.
    for (int i=0; i<=num; i++)
    {
        float u = float(i) / num;
        bs->interpolatePoint(u, pt);
        if (i % 10 == 0)
        {
            memcpy(curPair, pt, sizeof(float) * 3);
            curPair += 3;

            bs->interpolateTangent(u, norm);
            normlize(norm);
            float newPt[3];
            add(pt, norm, newPt);
            memcpy(curPair, newPt, sizeof(float) * 3);
            curPair += 3;
        }
        pt += 3;
    }

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    //std::shared_ptr<TRSGeode> BSplineCurve = std::make_shared<TRSGeode>();
    //BSplineCurve->readFromVertex(vertexBigDipper, sizeof(vertexBigDipper) / sizeof(float), EnVertex, 
    //    elementBigDipper, sizeof(elementBigDipper) / sizeof(unsigned int));
    //std::shared_ptr<TRSStateSet> pSS = BSplineCurve->getOrCreateStateSet();
    //TRSShader* shader = pSS->getShader();
    //shader->createVertexShader("shaders/DefaultVertexWithoutMVP.glsl");
    //shader->createFragmentShader("shaders/DefaultFragment.glsl");
    //shader->createTessControlShader("shaders/BezierTesc.glsl");
    //shader->createTessEvaluateShader("shaders/BezierTese.glsl");
    //shader->createProgram();
    //BSplineCurve->getVAO()->setDrawType(GL_PATCHES);
    //BSplineCurve->getVAO()->setDrawParam(4);
    //BSplineCurve->setColor(glm::vec4(1, 1, 1, 1));

    std::shared_ptr<TRSGeode> Bspline = std::make_shared<TRSGeode>();
    Bspline->readFromVertex(curve, 303, EnVertex);
    Bspline->getVAO()->setDrawType(GL_LINE_STRIP);
    Bspline->setColor(glm::vec4(0.9, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> BSplineNormPair = std::make_shared<TRSGeode>();
    BSplineNormPair->readFromVertex(samplePtPair, sampleNum*6, EnVertex);
    BSplineNormPair->getVAO()->setDrawType(GL_LINES);
    BSplineNormPair->setColor(glm::vec4(0.5, 1, 1, 1));

    std::shared_ptr<TRSGeode> CtrlPolygon = std::make_shared<TRSGeode>();
    CtrlPolygon->readFromVertex(vertexBigDipper, sizeof(vertexBigDipper) / sizeof(float), EnVertex);
    CtrlPolygon->getVAO()->setDrawType(GL_LINE_STRIP);
    CtrlPolygon->setColor(glm::vec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(Bspline);
    rootNodes->addChild(CtrlPolygon);
    rootNodes->addChild(BSplineNormPair);

    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
}


int CaseTessBSplineSurface(int argn, char** argc)
{
    float SurfacePts[]=
    {
        0.000000, 0.000000, 0.003754,
        1.000000, 0.000000, 1.690756,
        2.000000, 0.000000, 0.579913,
        3.000000, 0.000000, 2.426222,
        4.000000, 0.000000, 1.755028,
        5.000000, 0.000000, 1.439619,
        0.000000, 1.000000, 1.050874,
        1.000000, 1.000000, 2.687887,
        2.000000, 1.000000, 2.468520,
        3.000000, 1.000000, 2.239814,
        4.000000, 1.000000, 0.522324,
        5.000000, 1.000000, 2.576830,
        0.000000, 2.000000, 2.131504,
        1.000000, 2.000000, 1.540605,
        2.000000, 2.000000, 0.911985,
        3.000000, 2.000000, 0.044954,
        4.000000, 2.000000, 0.274209,
        5.000000, 2.000000, 1.093356,
        0.000000, 3.000000, 0.441939,
        1.000000, 3.000000, 0.497696,
        2.000000, 3.000000, 2.965575,
        3.000000, 3.000000, 1.337077,
        4.000000, 3.000000, 0.357250,
        5.000000, 3.000000, 0.014008,
        0.000000, 4.000000, 0.026734,
        1.000000, 4.000000, 1.133641,
        2.000000, 4.000000, 1.594989,
        3.000000, 4.000000, 1.713553,
        4.000000, 4.000000, 1.805292,
        5.000000, 4.000000, 1.821497,
    };
    int uResolution = 10;
    int vResolution = 10;
    float* surface = new float[uResolution*vResolution*3];
    memset(surface, 0, uResolution*vResolution * 3 * sizeof(float));
    float* normPairArr = new float[uResolution*vResolution * 6];
    memset(surface, 0, uResolution*vResolution * 6 * sizeof(float));
    BSplineSurface* bsSurface = new BSplineSurface;
    bsSurface->setCtrlPts(SurfacePts, 5, 4);
    for (int vIndex =0; vIndex<vResolution; vIndex++)
    {
        float v = float(vIndex) / (vResolution - 1);
        for (int uIndex = 0; uIndex<uResolution; uIndex++)
        {
            float u = float(uIndex) / (uResolution - 1);
            float *curPt = surface + (vIndex*uResolution+uIndex) * 3;
            bsSurface->interpolatePoint(u, v, curPt);

            float norm[3];
            bsSurface->interpolateNormal(u, v, norm);
            float* curNormPair = normPairArr + (vIndex*uResolution + uIndex) * 6;
            memcpy(curNormPair, curPt, sizeof(float) * 3);
            curNormPair += 3;
            float nextPt[3];
            add(curPt, norm, nextPt);
            memcpy(curNormPair, nextPt, sizeof(float) * 3);
            curNormPair += 3;
        }
    }

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> BsplineSurFace = std::make_shared<TRSGeode>();
    unsigned int* surFaceEleArr = genWireFrameElementsArray(uResolution, vResolution);
    BsplineSurFace->readFromVertex(surface, uResolution*vResolution * 3, EnVertex, surFaceEleArr, uResolution*vResolution*6);
    BsplineSurFace->setPolygonMode(GL_LINE);
    BsplineSurFace->getVAO()->setDrawType(GL_TRIANGLES);
    BsplineSurFace->setColor(glm::vec4(0.9, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> NormLines = std::make_shared<TRSGeode>();
    NormLines->readFromVertex(normPairArr, uResolution*vResolution * 6, EnVertex);
    NormLines->getVAO()->setDrawType(GL_LINES);
    NormLines->setColor(glm::vec4(1, 1, 0.5, 1));

    std::shared_ptr<TRSGeode> CtrlPolygon = std::make_shared<TRSGeode>();
    unsigned int* ctrlPolygonEleArr = genWireFrameElementsArray(6, 5);
    CtrlPolygon->readFromVertex(SurfacePts, sizeof(SurfacePts) / sizeof(float), EnVertex, ctrlPolygonEleArr, 180);
    CtrlPolygon->setPolygonMode(GL_LINE);
    CtrlPolygon->getVAO()->setDrawType(GL_TRIANGLES);
    CtrlPolygon->setColor(glm::vec4(0.5, 0.5, 1, 1));



    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(BsplineSurFace);
    //rootNodes->addChild(CtrlPolygon);
    rootNodes->addChild(NormLines);

    glPointSize(3);
    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
    return 0;
}
