/*
 * Textures from http://photosculpt.net/gallery/textures-seamless-tileable/
 * */

#include "adCore/adShader.hpp"
#include "adCore/adWindow.hpp"
#include "adCore/adImage.hpp"
#include "adPlatform/adOpenGL.hpp"
#include "adText/adPrintLib.hpp"
#include "adCore/adShader.hpp"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <SDL2/SDL.h>

#include <cstdio>

glm::mat4 modelMatrix;
glm::mat4 screenMatrix;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
glm::mat4 cameraMatrix;
glm::mat4 mvp;

glm::vec3 lightSource;

adShader squareShader;
GLuint   squareVerticesBuffer;
GLuint   squareCoordsBuffer;
GLuint   squareIndicesBuffer;

GLuint   textureColor;


void Render(int a_width, int a_height)
{
  glViewport(0, 0, a_width, a_height);

  projectionMatrix = glm::perspective(glm::pi<float>()*0.25f,
                                      static_cast<float>(a_width) /
                                      static_cast<float>(a_height), 0.1f, 10.f);



  viewMatrix = glm::inverse(cameraMatrix);

  mvp = projectionMatrix * viewMatrix * modelMatrix;
  //mvp = modelMatrix;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glDepthMask(GL_TRUE);

  glClearColor(0.0f, 0.0f, 0.0f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glUseProgram(squareShader.m_program);

  glBindBuffer(GL_ARRAY_BUFFER, squareVerticesBuffer);
  glVertexAttribPointer(squareShader.m_locations[0], 3, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(squareShader.m_locations[0]);

  glBindBuffer(GL_ARRAY_BUFFER, squareCoordsBuffer);
  glVertexAttribPointer(squareShader.m_locations[1], 2, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(squareShader.m_locations[1]);

  //a_gl.activeTexture(a_gl.TEXTURE0);
  //a_gl.bindTexture(a_gl.TEXTURE_2D, a_buffers.ditherTexture);
  //a_gl.uniform1i(a_shaders.starShader.u_dither, 0);

  //glUniform1f(m_shaderStar.m_locations[7], m_gamma);//set gamma
  glUniformMatrix4fv(squareShader.m_locations[2], 1, GL_FALSE, (float*)&mvp);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureColor);
  glUniform1i(squareShader.m_locations[3],0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIndicesBuffer);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void LoadData()
{
  glGenBuffers(1, &squareVerticesBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, squareVerticesBuffer);

  float squareVertices[] = {
      1.0f,  0.0f, 1.0f,
      -1.0f, 0.0f, 1.0f,
      1.0f,  0.0f, -1.0f,
      -1.0f, 0.0f, -1.0f
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, squareVertices,
               GL_STATIC_DRAW);

  glGenBuffers(1, &squareCoordsBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, squareCoordsBuffer);

  float squareCoords[] = {
      1.0f, 0.0f,
      0.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, squareCoords,
               GL_STATIC_DRAW);

  glGenBuffers(1, &squareIndicesBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIndicesBuffer);

  uint16_t squareIndices[] = {
      1,3,0,2,0,3
  };

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t)*6, squareIndices,
               GL_STATIC_DRAW);

  squareShader.SetNumAttributes(4);
  squareShader.AddLocation(0, adShader::ATTRIBUTE, "a_pos");
  squareShader.AddLocation(1, adShader::ATTRIBUTE, "a_texCoord");
  squareShader.AddLocation(2, adShader::UNIFORM, "u_mvp");
  squareShader.AddLocation(3, adShader::UNIFORM, "u_textureColor");



  squareShader.m_vertFilename = "shaders/relief_gl110.vert";
  squareShader.m_fragFilename = "shaders/relief_gl110.frag";

  FILE* vertFile = fopen(squareShader.m_vertFilename.Data(), "r");
  FILE* fragFile = fopen(squareShader.m_fragFilename.Data(), "r");

  fseek (vertFile , 0 , SEEK_END);
  squareShader.m_vertFileSize = ftell (vertFile);
  rewind (vertFile);

  fseek (fragFile , 0 , SEEK_END);
  squareShader.m_fragFileSize = ftell (fragFile);
  rewind (fragFile);


  squareShader.m_vertFile = new char[squareShader.m_vertFileSize+1];
  squareShader.m_fragFile = new char[squareShader.m_fragFileSize+1];

  memset(squareShader.m_vertFile, 0, squareShader.m_vertFileSize+1);
  memset(squareShader.m_fragFile, 0, squareShader.m_fragFileSize+1);

  fread(squareShader.m_vertFile, squareShader.m_vertFileSize, 1, vertFile);
  fread(squareShader.m_fragFile, squareShader.m_fragFileSize, 1, fragFile);


  fclose(vertFile);
  fclose(fragFile);

  squareShader.CreateProgram();

  SDL_Surface* imageColor = adImage::LoadImage("textures/photosculpt-peebles-diffuse.png");

  textureColor = adImage::MakeTexture(imageColor);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);

  SDL_FreeSurface(imageColor);
}

void FreeData()
{

  glDeleteBuffers(1, &squareVerticesBuffer);
  glDeleteBuffers(1, &squareCoordsBuffer);
  glDeleteBuffers(1, &squareIndicesBuffer);
  glDeleteTextures(1, &textureColor);
}

int main()
{
  int width = 800;
  int height = 600;
  uint32_t flags = /*SDL_WINDOW_MINIMIZED | */SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL |
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI /* | SDL_WINDOW_HIDDEN*/;

  bool running = true;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS))
  {
    fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
    return 0;
  }

  TTF_Init();

  adWindow* window = new adWindow(glm::ivec2(width, height), flags, 2, 0);

  LoadData();

  glm::vec3 up(0.f,0.f,1.f);
  glm::vec3 pos(0.f, -2.f, 0.f);
  //glm::vec3 center(0.f,0.f,0.f);

  glm::vec3 z = glm::normalize(pos);
  glm::vec3 x = glm::cross(up,z);
  glm::vec3 y = glm::cross(z,x);

  cameraMatrix[0] = glm::vec4(x,0.0);
  cameraMatrix[1] = glm::vec4(y,0.0);
  cameraMatrix[2] = glm::vec4(z,0.0);
  cameraMatrix[3] = glm::vec4(pos,1.0);

  lightSource = glm::vec3(-1.f,-1.f,-1.f);

  while(running)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
        running = false;
      else if(event.type == SDL_KEYUP)
      {
        switch( event.key.keysym.sym )
        {
          case SDLK_ESCAPE:
            running = false;
            break;
        }
      }
      else if(event.type == SDL_WINDOWEVENT)
      {
        switch (event.window.event)
        {
          case SDL_WINDOWEVENT_RESIZED:
            window->Resize(glm::ivec2(event.window.data1, event.window.data2));
            //width = event.window.data1;
            //height = event.window.data2;
            //m_render.m_print.SetScreen(event.window.data1, event.window.data2);
            break;
          case SDL_WINDOWEVENT_SIZE_CHANGED:
            //m_window->Resize(glm::ivec2(event.window.data1, event.window.data2));
            break;

        }
      }
    }

    Render(window->m_sizePixel.x, window->m_sizePixel.y);
    SDL_GL_SwapWindow(window->m_window);

  }

  delete window;

  window = nullptr;

  TTF_Quit();
  SDL_Quit();

  return 0;
}
