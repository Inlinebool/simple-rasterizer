///
/// \file main.cpp
/// \brief SDL Demo Code
/// \author Kairong Jiang <jiangkairong@email.arizona.edu>
/// \date 03/11/18
///
///


/*
 ***********************************************************************

 Copyright (C) 2018, Kairong Jiang
 University of Arizona

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 ***********************************************************************
 */


//include SDL2 libraries
#include <SDL.h>

//C++ includes
#include <iostream>
#include <fstream>
#include <cfloat>
#include <cmath>
#include <vector>
#include <algorithm>
#include "Renderer.h"

using namespace std;

///
/// Log an SDL error with some error message to the output stream of our
/// choice
///
/// \param os The output stream to write the message to
/// \param msg The error message to write, SDL_GetError() appended to it
///
void logSDLError(std::ostream &os, const std::string &msg) {
  os << msg << " error: " << SDL_GetError() << std::endl;
}

///
/// Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
/// the texture's width and height
///
/// \param tex The source texture we want to draw
/// \param ren The renderer we want to draw to
/// \param x The x coordinate to draw to
/// \param y The y coordinate to draw to
///
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
  //Setup the destination rectangle to be at the position we want
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;

  //Query the texture to get its width and height to use
  SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
  SDL_RenderCopy(ren, tex, nullptr, &dst);
}


///
/// Main function.  Initializes an SDL window, renderer, and texture,
/// and then goes into a loop to listen to events and draw the texture.
///
/// \param argc Number of command line arguments
/// \param argv Array of command line arguments
/// \return integer indicating success (0) or failure (nonzero)
///

int main(int argc, char **argv) {

  if (argc == 1) {
    cout << "please set the relative path of the scene file as the argument of the program." << endl;
    return 0;
  }

  string inputFileName = argv[1];

  Renderer rasterizeRenderer(inputFileName);

  auto result = rasterizeRenderer.renderForDisplay();

  ofstream out;

  //Integers specifying the width (number of columns) and height (number
  //of rows) of the image
  int num_cols = static_cast<int>(result->cols());
  int num_rows = static_cast<int>(result->rows());

  //Start up SDL and make sure it went ok
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    logSDLError(std::cout, "SDL_Init");
    return 1;
  }

  //Setup our window and renderer
  SDL_Window *window = SDL_CreateWindow("Ray Tracer", 100, 100, num_cols, num_rows, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    logSDLError(std::cout, "CreateWindow");
    SDL_Quit();
    return 1;
  }
  SDL_Renderer
      *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    logSDLError(std::cout, "CreateRenderer");
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  //The textures we'll be using
  SDL_Texture *background;

  //A raw data array of characters.  Each column is drawn using the same
  //color that is a grayscale ramp from the leftmost to rightmost pixel.
//  unsigned char *data = new unsigned char[num_cols * num_rows * 3];
//  //r is row, c is column, and ch is channel
//  for (int r = 0; r < num_rows; r++) {
//    for (int c = 0; c < num_cols; c++) {
//      for (int ch = 0; ch < 3; ch++) {
//        data[3 * (r * num_cols + c) + ch] = 255 * float(c) / num_cols;
//      }
//    }
//  }
  auto *data = ImageUtils::getRawData(*result);

  //Initialize the texture.  SDL_PIXELFORMAT_RGB24 specifies 3 bytes per
  //pixel, one per color channel
  background =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, num_cols, num_rows);
  //Copy the raw data array into the texture.
  SDL_UpdateTexture(background, nullptr, data, 3 * num_cols);
  if (background == nullptr) {
    logSDLError(std::cout, "CreateTextureFromSurface");
  }


  //Make sure they both loaded ok
  if (background == nullptr) {
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  //Variables used in the rendering loop
  SDL_Event event;
  bool quit = false;
  bool leftMouseButtonDown = false;
  int start_mouseX;
  int start_mouseY;
  float orig_x_angle;
  float orig_y_angle;

  while (!quit) {
    //Grab the time for frame rate computation
    const Uint64 start = SDL_GetPerformanceCounter();

    //Clear the screen
    SDL_RenderClear(renderer);


    //Event Polling
    //This while loop responds to mouse and keyboard commands.
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
      //Use number input to select which clip should be drawn
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:break;
          case SDLK_f:rasterizeRenderer.setShadingPolicy(Renderer::FLAT_SHADING);
            delete[] data;
            result = rasterizeRenderer.renderForDisplay();
            data = ImageUtils::getRawData(*result);
            break;
          case SDLK_g:rasterizeRenderer.setShadingPolicy(Renderer::GOURAUD_SHADING);
            delete[] data;
            result = rasterizeRenderer.renderForDisplay();
            data = ImageUtils::getRawData(*result);
            break;
          case SDLK_p:rasterizeRenderer.setShadingPolicy(Renderer::PHONG_SHADING);
            delete[] data;
            result = rasterizeRenderer.renderForDisplay();
            data = ImageUtils::getRawData(*result);
            break;
          case SDLK_i:break;
          case SDLK_n:break;
          case SDLK_m:break;
          case SDLK_b:break;
          case SDLK_s:cout << "saving image to ppm" << endl;
            out.open(inputFileName + ".ppm", std::ios::out | std::ios::trunc | std::ios::binary);
            out << "P6" << std::endl;
            out << std::to_string(num_cols).data() << " " << std::to_string(num_rows).data() << std::endl;
            out << std::to_string(255).data() << std::endl;
            for (int i = 0; i < num_rows; i++) {
              for (int j = 0; j < num_cols; j++) {
                out << (*result)(i, j)(0);
                out << (*result)(i, j)(1);
                out << (*result)(i, j)(2);
              }
            }
            out.close();
            break;
          default:break;
        }
      } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT)
          leftMouseButtonDown = false;

      } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          leftMouseButtonDown = true;
        }
      } else if (event.type == SDL_MOUSEMOTION) {
        if (leftMouseButtonDown) {
          int mouseX = event.motion.x;
          int mouseY = event.motion.y;

          data[3 * (mouseY * num_cols + mouseX) + 0] = 255;
          data[3 * (mouseY * num_cols + mouseX) + 1] = 0;
          data[3 * (mouseY * num_cols + mouseX) + 2] = 0;
        }
      }
    }

    //Update the texture, assuming data has changed.
    SDL_UpdateTexture(background, nullptr, data, 3 * num_cols);
    //display the texture on the screen
    renderTexture(background, renderer, 0, 0);
    //Update the screen
    SDL_RenderPresent(renderer);

    //Display the frame rate to stdout
    const Uint64 end = SDL_GetPerformanceCounter();
    const static Uint64 freq = SDL_GetPerformanceFrequency();
    const double seconds = (end - start) / static_cast< double >( freq );
    //You may want to comment this line out for debugging purposes
//    std::cout << "Frame time: " << seconds * 1000.0 << "ms" << std::endl;
  }


  //After the loop finishes (when the window is closed, or escape is
  //pressed, clean up the data that we alloc/**/ated.
  delete[] data;
  SDL_DestroyTexture(background);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
