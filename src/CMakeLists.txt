set(Headers 
	"${CMAKE_CURRENT_SOURCE_DIR}/hid.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/bluetooth.h" 
	"${CMAKE_CURRENT_SOURCE_DIR}/notifyicon.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Window.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/worker.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/mingw-unicode-gui.h"
	PARENT_SCOPE)
	
set(Sources 
	"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/hid.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/bluetooth.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/notifyicon.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/worker.cpp"
	PARENT_SCOPE)