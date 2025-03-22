cmake_minimum_required(VERSION 3.29)

project(
  ChatVision-client
  VERSION 0.1
  LANGUAGES CXX)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Multimedia)

add_subdirectory(json)

qt_standard_project_setup()

qt_add_executable(appChatVision-client main.cpp)

qt_add_qml_module(
  appChatVision-client
  URI
  ChatVision-client
  VERSION
  1.0
  # QML_FILES Main.qml
  SOURCES
  SOURCES
  piplinebuild.h
  piplinebuild.cpp
  mediathread.cpp
  mediathread.h
  ConstValue.h
  # QML_FILES LoginPage.qml
  QML_FILES
  # HomePage.qml
  RESOURCES
  RESOURCES
  assets.qrc
  SOURCES
  personalpagecontroller.h
  personalpagecontroller.cpp
  SOURCES
  communicationpagecontroller.h
  communicationpagecontroller.cpp
  SOURCES
  tool.h
  tool.cpp
  QML_FILES
  # ConversationListPage.qml
  QML_FILES
  # CommunicationPage.qml
  QML_FILES
  # FindFriendPage.qml
  SOURCES
  client.h
  client.cpp
  SOURCES
  sendmsg.h
  sendmsg.cpp
  SOURCES
  recvmsg.h
  recvmsg.cpp
  SOURCES
  msgnode.h
  msgnode.cpp
  SOURCES
  user.h
  user.cpp
  SOURCES
  filetools.h
  filetools.cpp
  SOURCES
  singleton.h
  singleton.cpp
  QML_FILES
  # FollowListPage.qml
  SOURCES
  followlistpagecontroller.h
  followlistpagecontroller.cpp
  QML_FILES
  # FollowUserInfoPage.qml
  QML_FILES
  # BaseInfoProperties.qml
  SOURCES
  myimageprovider.h
  myimageprovider.cpp
  SOURCES
  findfriendpagecontroller.h
  findfriendpagecontroller.cpp
  SOURCES tokenbucket.h tokenbucket.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-webrtc-1.0
                  gstreamer-sdp-1.0 gstreamer-rtp-1.0)
pkg_check_modules(JSON_GLIB REQUIRED json-glib-1.0)

include_directories(${JSON_GLIB_INCLUDE_DIRS} ${GST_INCLUDE_DIRS})
include_directories(/usr/include/nice)

link_directories(${JSON_GLIB_LIBRARY_DIRS} ${GST_LIBRARY_DIRS} libnice.so)

target_compile_features(appChatVision-client PRIVATE cxx_std_20)

set_target_properties(
  appChatVision-client
  PROPERTIES AUTORCC TRUE
             MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE)

target_link_libraries(
  appChatVision-client
  PRIVATE Qt6::Quick Qt6::quickmultimedia ${GST_LIBRARIES} # 链接GStreamer库
          gstvideo-1.0 ${JSON_GLIB_LIBRARIES} nlohmann_json)

include(GNUInstallDirs)
install(
  TARGETS appChatVision-client
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# qt_add_executable(ChatVision-client)  //Creates and finalizes an application
# target of a platform-specific type. This command performs the following tasks:
# 1. Create a CMake target of the appropriate type for the target platform. 2.
# Link the target to the Qt::Core library. 3. Handle finalization of the CMake
# target.

# qt_standard_project_setup()     #set(CMAKE_AUTOUIC ON)  set(CMAKE_AUTOMOC ON)

# @if %{HasTranslation} find_package(Qt6 REQUIRED COMPONENTS Core Quick
# LinguistTools) @else find_package(Qt6 REQUIRED COMPONENTS Core Quick) @endif

# target_sources(appChatVision-client PRIVATE main.cpp qml.qrc @if
# %{HasTranslation} %{TsFileName} @endif
#
# PUBLIC
#
# )

# qt_import_qml_plugins(ChatVision-client)

# cmake_minimum_required(VERSION 3.16)
#
# project(ChatVision-client VERSION 0.1 LANGUAGES CXX)
#
# @if !true set(CMAKE_AUTOMOC ON) @endif set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
# find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)
#
# @if true @if false qt_standard_project_setup(REQUIRES 6.5) @else
# qt_standard_project_setup() @endif @endif
#
# qt_add_executable(appChatVision-client main.cpp )
#
# qt_add_qml_module(appChatVision-client URI ChatVision-client VERSION 1.0
# QML_FILES Main.qml )
#
# # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. #
# If you are developing for iOS or macOS you should consider setting an #
# explicit, fixed bundle identifier manually though.
# set_target_properties(appChatVision-client PROPERTIES #
# MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appChatVision-client
# MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
# MACOSX_BUNDLE_SHORT_VERSION_STRING
# ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE TRUE
# WIN32_EXECUTABLE TRUE )
#
# target_link_libraries(appChatVision-client PRIVATE Qt6::Quick )
#
# include(GNUInstallDirs) install(TARGETS appChatVision-client BUNDLE
# DESTINATION . LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION
# ${CMAKE_INSTALL_BINDIR} )
