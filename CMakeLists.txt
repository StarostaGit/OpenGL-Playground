cmake_minimum_required(VERSION 3.17)
project(playground)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_FILES
        lib/imgui/imgui.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_impl_glfw.cpp
        lib/imgui/imgui_impl_opengl3.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/imconfig.h
        lib/imgui/imgui.h
        lib/imgui/imgui_impl_glfw.h
        lib/imgui/imgui_impl_opengl3.h
        lib/imgui/imgui_internal.h
        lib/imgui/imstb_rectpack.h
        lib/imgui/imstb_textedit.h
        lib/imgui/imstb_truetype.h)

add_executable(imgui_test lib/imgui/main.cpp ${IMGUI_FILES} scenes/ManyBoxesScene.h scenes/SimpleBoxScene.h scenes/MaterialLightMapsScene.h)

add_executable(playground main.cpp VertexBuffer.cpp VertexBuffer.h IndexBuffer.cpp IndexBuffer.h VertexBufferLayout.h VertexArray.cpp VertexArray.h VertexBufferLayout.cpp Shader.cpp Shader.h Renderer.cpp Renderer.h utils.h lib/stb/stb_image.h lib/stb/stb_image.cpp Texture.cpp Texture.h ${IMGUI_FILES} Camera.cpp Camera.h Scene.h scenes/SimpleLightningScene.h Material.h Material.cpp Light.h shapes.h shapes.cpp scenes/MultipleLightsScene.h)

find_package(OpenGL REQUIRED)
find_package(FREEGLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(
        ${OPENGL_INCLUDE_DIRS}
        ${FREEGLUT_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW3_INCLUDE_DIRS}
        lib/imgui
        ./
)

target_link_libraries(playground opengl32 glew32 freeglut glu32 glfw3)

target_link_libraries(imgui_test opengl32 glew32 freeglut glu32 glfw3)