# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(CenterSpace)


if (APPLE)
    include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
    SET(EXETYPE MACOSX_BUNDLE)
    SET_SOURCE_FILES_PROPERTIES(MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
    SET_SOURCE_FILES_PROPERTIES(${DEMO_BLEND} PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
    SET (NibFile MainMenu.nib)
endif()

#generate font file to inl file, turn on OGREKIT_COMPILE_SWIG build option.
#add_templates(1 GEN_TEMPLATES_OUT fonts/uifonts.inl ${FONT_TMPL})

set(STYLESHEETS
	Assets/StyleSheets/centerspace.css
)

set(RML
	Assets/RML/MainMenu.rml
)

set(GAME_SRC
	Game/Main.cpp
	GUI/RenderInterfaceOgre3D.cpp
	GUI/RocketRenderer.cpp
	GUI/SystemInterfaceOgre3D.cpp
	GUI/FileInterfaceOgre3D.cpp
	Input/InputListener.cpp
	Managers/GameManager.cpp
	States/GameState.cpp
	States/MainMenuState.cpp
	States/PauseState.cpp
	States/PlayState.cpp
	${STYLESHEETS}
	${RML}
)

set(GAME_HDR
	GUI/RenderInterfaceOgre3D.h
	GUI/RocketRenderer.h
	GUI/SystemInterfaceOgre3D.h
	GUI/FileInterfaceOgre3D.h
	Input/InputListener.h
	Managers/GameManager.h
	States/GameState.h
	States/MainMenuState.h
	States/PauseState.h
	States/PlayState.h
)

set(ALL
    ${GAME_SRC}
    ${GAME_HDR}
)


include_directories(
    .
    "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
    "D:/Projects/Center Space/Ogre/ogre_v1-8-0RC1_windows/include"
    "D:/Projects/Center Space/Ogre/ogre_src_v1-8-0RC1/OgreMain/include"
    "D:/Projects/Center Space/Ogre/ogre_src_v1-8-0RC1/Components/Terrain/include"
    "D:/Projects/Center Space/Ogre/ogre_src_v1-8-0RC1/Components/Paging/include"
    "D:/Projects/Center Space/Ogre/libRocket/Include"
    "D:/Projects/Center Space/Ogre/OgreDependencies_MSVC_20101231/Dependencies/include/OIS"
)

#message(STATUS ${OGREKIT_LIBROCKET_LIBS})

#need for libRocket link
add_definitions(
    -DSTATIC_LIB
    -D_WINDOWS
    -D_DEBUG
    -D_CRT_SECURE_NO_WARNINGS
    -D_CRT_SECURE_NO_DEPRECATE
    -DNOMINMAX
    -DUT_USE_ZLIB
    -DAL_STATIC_LIB
    -DALC_STATIC_LIB
    -DSIZEOF_SHORT=2
    -DSIZEOF_INT=4
    -DSIZEOF_LONG=4
    -DSIZEOF_WCHAR_T=2
    -DSIZEOF_VOID_P=4
    -DSIZEOF_LONG_LONG=8
    -DSIZEOF_SIZE_T=4
    -DSIZEOF_UINT=4
    -DwxSIZE_T_IS_UINT=1
    -D_CRT_SECURE_NO_WARNINGS
    -D_CRT_SECURE_NO_DEPRECATE
    -D_SCL_SECURE_NO_WARNINGS
    -DSTATIC_LIB
    -DCMAKE_INTDIR="Debug"
    )

#use_precompiled_header(${PROJECT_NAME} StdAfx.h StdAfx.cpp)



if (APPLE)   
    add_executable(
        ${PROJECT_NAME}
        MACOSX_BUNDLE
        MainMenu.nib
        ${ALL}
        ${HiddenCMakeLists}
    )
else()

	add_executable(${PROJECT_NAME} ${ALL})
	
	target_link_libraries(
		${PROJECT_NAME}
		debug
		kernel32
		user32
		gdi32
		winspool
		shell32
		ole32
		oleaut32
		uuid
		comdlg32
		advapi32
		opengl32
		glu32
		Comctl32
		Rpcrt4
		"D:/Projects/Center Space/Ogre/ogre_v1-8-0RC1_windows/lib/Debug/OgreMain_d.lib"
		"D:/Projects/Center Space/Ogre/ogre_v1-8-0RC1_windows/lib/Debug/OgreRTShaderSystem_d.lib"
		"D:/Projects/Center Space/Ogre/OgreDependencies_MSVC_20101231/Dependencies/lib/debug/OIS_d.lib"
		"D:/Projects/Center Space/Ogre/ogre_v1-8-0RC1_windows/lib/Debug/OgreTerrain_d.lib"
		"D:/Projects/Center Space/Ogre/ogre_v1-8-0RC1_windows/lib/Debug/OgrePaging_d.lib"
		"D:/Projects/Center Space/Ogre/OgreDependencies_MSVC_20101231/Dependencies/lib/debug/freetype2311_d.lib"
		"D:/Projects/Center Space/Ogre/OgreDependencies_MSVC_20101231/Dependencies/lib/debug/FreeImaged.lib"
		"D:/Projects/Center Space/Ogre/OgreDependencies_MSVC_20101231/Dependencies/lib/debug/zziplibd.lib"
		"D:/Projects/Center Space/Ogre/OgreDependencies_MSVC_20101231/Dependencies/lib/debug/zlibd.lib"
		
	)

    IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
        foreach(bfile ${DEMO_BLEND})
            ADD_CUSTOM_COMMAND(
                TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${bfile} ${CMAKE_CURRENT_BINARY_DIR}/${bfile}
            )
        endforeach()
    ENDIF ()
endif() 