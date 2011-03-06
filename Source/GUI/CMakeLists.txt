cmake_minimum_required(VERSION 2.8)

project(RenderWindowUI)

find_package(VTK)
include(${VTK_USE_FILE})

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SimpleViewSrcs
  RenderWindowUI.cxx
  RegistrationMonitor.cxx
  SimpleViewUI.cxx
  vtkKWImage.cxx
  vtkKWImageIO.cxx
)

set(SimpleViewUI SimpleViewUI.ui)
set(SimpleViewHeaders SimpleViewUI.h)

QT4_WRAP_UI(UISrcs ${SimpleViewUI})
QT4_WRAP_CPP(MOCSrcs ${SimpleViewHeaders} )

source_group("Resources" FILES
  ${SimpleViewUI}
)

source_group("Generated" FILES
  ${UISrcs}
  ${MOCSrcs}
)

add_executable( RenderWindowUI ${SimpleViewSrcs} ${UISrcs} ${MOCSrcs})
target_link_libraries( RenderWindowUI QVTK ${ITK_LIBRARIES})
